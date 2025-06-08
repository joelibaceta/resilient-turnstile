#!/bin/bash

echo "🔧 Setting up your Flask + SocketIO + Arduino Monitor environment..."

# Paso 0: Confirmar conexión de Arduino
echo "🧩 Please connect your Arduino now via USB."
read -p "🔌 Press [Enter] when it's connected..."

# Paso 1: Crear entorno virtual
echo "📦 Installing Python packages and virtualenv..."
sudo apt update
sudo apt install -y python3 python3-pip python3-venv

# Crear entorno virtual si no existe
if [ ! -d "env" ]; then
  python3 -m venv env
fi

source env/bin/activate

# Instalar requerimientos
echo "📥 Installing Python dependencies..."
pip install --upgrade pip
pip install -r requirements.txt

# Paso 2: Detectar automáticamente puerto serial de Arduino
echo "🔍 Detecting Arduino serial port..."
arduino_port=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | head -n 1)

if [ -z "$arduino_port" ]; then
  echo "⚠️ Could not detect an Arduino automatically."
  read -p "👉 Please enter the serial port manually (e.g., /dev/ttyUSB0): " arduino_port
else
  echo "✅ Detected port: $arduino_port"
fi

# Paso 3: Configurar archivo .env
read -p "🛠️ Do you want to configure .env now? (y/n): " confirm
if [[ "$confirm" =~ ^[Yy]$ ]]; then
  read -p "🔐 Enter your Twilio SID: " twilio_sid
  read -p "🔐 Enter your Twilio AUTH TOKEN: " twilio_token
  read -p "📱 Enter your Twilio FROM number: " twilio_from
  read -p "📱 Enter your destination TO number: " twilio_to
  read -p "⏱️ Notification interval in seconds (default: 3600): " notif_interval
  read -p "🚨 Enable notifications? (true/false): " notif_enabled

  notif_interval=${notif_interval:-3600}
  notif_enabled=${notif_enabled:-true}

  cat <<EOF > .env
# Serial config
SERIAL_PORT=$arduino_port
SERIAL_BAUDRATE=115200

# Notification config
ENABLE_NOTIFICATIONS=$notif_enabled
NOTIFICATION_INTERVAL=$notif_interval

# Twilio credentials
TWILIO_ACCOUNT_SID=$twilio_sid
TWILIO_AUTH_TOKEN=$twilio_token
TWILIO_FROM=$twilio_from
TWILIO_TO=$twilio_to
EOF

  echo "✅ .env file created successfully."
else
  echo "❌ Skipping .env configuration."
fi
