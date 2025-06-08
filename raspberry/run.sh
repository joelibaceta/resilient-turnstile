#!/bin/bash

echo "🚀 Starting Antenna Monitor App..."

# Verificar que exista el archivo .env
if [ ! -f .env ]; then
  echo "❌ .env file not found. Please run setup.sh first."
  exit 1
fi

# Cargar variables de entorno
export $(grep -v '^#' .env | xargs)

# Verificar que esté configurada la variable SERIAL_PORT
if [ -z "$SERIAL_PORT" ]; then
  echo "❌ SERIAL_PORT not defined in .env"
  exit 1
fi

# Verificar entorno virtual
if [ ! -d "env" ]; then
  echo "❌ Python virtual environment not found. Run setup.sh first."
  exit 1
fi

# Activar entorno virtual
source env/bin/activate

# Esperar a que el puerto serial esté disponible
echo "🔌 Waiting for Arduino on port $SERIAL_PORT..."
until [ -e "$SERIAL_PORT" ]; do
  echo "⏳ Arduino not found on $SERIAL_PORT. Please connect it and press Enter to retry."
  read -p ""
done

echo "✅ Arduino detected on $SERIAL_PORT"

# Mostrar configuración clave
echo ""
echo "🧾 Configuration:"
echo "  ➤ SERIAL_PORT: $SERIAL_PORT"
echo "  ➤ BAUDRATE: ${BAUDRATE:-115200}"
echo "  ➤ NOTIFICATIONS_ENABLED: ${NOTIFICATIONS_ENABLED:-false}"
echo "  ➤ TWILIO_PHONE_FROM: ${TWILIO_PHONE_FROM:-(not set)}"
echo "  ➤ TWILIO_PHONE_TO: ${TWILIO_PHONE_TO:-(not set)}"
echo ""

# Confirmar con el usuario antes de lanzar
read -p "🎯 Ready to launch the app? (Y/n): " confirm
if [[ "$confirm" =~ ^[Nn]$ ]]; then
  echo "❌ Aborted."
  exit 1
fi

# Lanzar app
echo "🌐 Access your app at: http://0.0.0.0:9000"
python app.py