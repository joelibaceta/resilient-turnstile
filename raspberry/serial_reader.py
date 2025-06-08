# serial_reader.py
"""
This module provides a generator function to read serial data from a microcontroller
(e.g., Arduino) connected to the system. If the serial port is not available,
it simulates data for development and testing purposes.

Configuration:
- SERIAL_PORT: The serial port to connect to (default: /dev/cu.usbmodem1101)
- SERIAL_BAUDRATE: The baud rate for the connection (default: 115200)
These values are loaded from environment variables using os.getenv().
"""

import serial
import time
import os

# Load serial configuration from environment variables
SERIAL_PORT = os.getenv('SERIAL_PORT', '/dev/cu.usbmodem1101')
SERIAL_BAUDRATE = int(os.getenv('SERIAL_BAUDRATE', '115200'))

def serial_data_generator(port=SERIAL_PORT, baudrate=SERIAL_BAUDRATE):
    """
    Generator that yields decoded lines from a serial port.

    Parameters:
        port (str): Serial port to connect to.
        baudrate (int): Baud rate for serial communication.

    Yields:
        str: A line of decoded text received via the serial port.
             If the serial connection fails, simulated data is yielded instead.
    """
    try:
        print(f"[INFO] Trying to open serial port {port} at {baudrate} baud...")
        ser = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)  # Allow time for the microcontroller to initialize
        print(f"[INFO] Port {port} opened successfully.")

        while True:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8', errors='replace').strip()
                print(f"[SERIAL] {line}")
                yield line

    except serial.SerialException as e:
        print(f"[ERROR] Could not open serial port: {e}")
        print("[INFO] Entering simulation mode with fake data...")

        # Simulated fallback data (useful for UI testing)
        while True:
            fake_data = "METRIC,ACEL,0.01,0.02,9.80\nMETRIC,TEMP,24.5\nMETRIC,HUMIDITY,45.0"
            print(f"[SIMULATED] {fake_data}")
            yield fake_data
            time.sleep(1)