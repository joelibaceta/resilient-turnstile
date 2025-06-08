# app.py

from flask import Flask, render_template
from flask_socketio import SocketIO
from serial_reader import serial_data_generator 
from dotenv import load_dotenv
import threading
import eventlet

# Apply monkey patching for async compatibility
eventlet.monkey_patch()

# Load environment variables from .env file
load_dotenv()

# Initialize Flask app and SocketIO
app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

# Instantiate the notifier handler
try:
    from notifier import Notifier
    notifier = Notifier()
except ImportError:
    print("[ERROR] Notifier module not found. SMS alerts will not be available.")
    notifier = None


def background_serial_thread():
    """
    Continuously reads data from the serial port and emits it via WebSocket.
    If an 'EVENT' of antenna fall is detected, triggers the notifier with throttling logic.
    """
    for data in serial_data_generator():
        print(f"[EMIT] {data}")

        if data.startswith("EVENT"):
            socketio.emit('event_data', data)
            if "not_vertical" in data:
                if notifier:
                    print("[NOTIFIER] Antenna fall detected, sending SMS alert.")
                    # Notify if the antenna is not vertical
                    notifier.notify_if_needed("ANTENNA_FALL")
        elif data.startswith("METRIC"):
            socketio.emit('metric_data', data)
        else:
            socketio.emit('unknown_data', data)  # For debugging purposes

@app.route('/')
def index():
    """
    Render the main dashboard page.
    """
    return render_template('index.html')

if __name__ == '__main__':
    # Start the serial reading thread in the background
    thread = threading.Thread(target=background_serial_thread)
    thread.daemon = True
    thread.start()

    # Run the Flask app with WebSocket support
    socketio.run(app, host='0.0.0.0', port=9000)