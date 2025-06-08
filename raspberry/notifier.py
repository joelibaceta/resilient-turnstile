import os
import time
from twilio.rest import Client
from dotenv import load_dotenv

load_dotenv()

class Notifier:
    """
    Notifier es una clase para enviar alertas SMS utilizando Twilio, con control de cooldown para evitar envíos repetidos.

    Atributos:
        send_sms (bool): Indica si el envío de SMS está habilitado, según la variable de entorno SEND_SMS_ALERTS.
        cooldown (int): Tiempo de espera (en segundos) entre envíos de SMS para el mismo evento.
        last_sms_time (float): Marca de tiempo del último SMS enviado.
        twilio_sid (str): SID de la cuenta de Twilio, obtenido de la variable de entorno TWILIO_ACCOUNT_SID.
        twilio_token (str): Token de autenticación de Twilio, obtenido de la variable de entorno TWILIO_AUTH_TOKEN.
        twilio_from (str): Número de teléfono de origen para los SMS, obtenido de la variable de entorno TWILIO_FROM.
        twilio_to (str): Número de teléfono de destino para los SMS, obtenido de la variable de entorno TWILIO_TO.
        client (Client): Cliente de Twilio para enviar mensajes SMS (solo si send_sms es True).

    Métodos:
        __init__():
            Inicializa la configuración de Twilio y las variables de control de envío de SMS.

        notify_if_needed(event_type):
            Envía una alerta SMS si el evento lo requiere y ha pasado el tiempo de cooldown.
            Actualmente, solo responde al evento "ANTENNA_FALL".

        _send_sms(msg):
            Envía un SMS con el mensaje especificado usando Twilio, si el envío está habilitado.
            Maneja errores de envío e imprime mensajes de estado.
    """
    def __init__(self):
        self.send_sms = os.getenv("SEND_SMS_ALERTS", "False") == "True"
        self.cooldown = 3600  # segundos
        self.last_sms_time = 0

        self.twilio_sid = os.getenv("TWILIO_ACCOUNT_SID")
        self.twilio_token = os.getenv("TWILIO_AUTH_TOKEN")
        self.twilio_from = os.getenv("TWILIO_FROM")
        self.twilio_to = os.getenv("TWILIO_TO")

        if self.send_sms:
            self.client = Client(self.twilio_sid, self.twilio_token)

    def notify_if_needed(self, event_type):
        """
        Sends an SMS notification if the specified event type requires it and the cooldown period has elapsed.

        Parameters:
            event_type (str): The type of event to check for notification. Currently, only "ANTENNA_FALL" triggers an SMS.

        Behavior:
            - If the event_type is "ANTENNA_FALL" and the cooldown period has passed since the last SMS,
              sends an alert SMS and updates the last notification time.
            - If the cooldown period has not passed, logs a message indicating that the SMS was recently sent.

        Note:
            The cooldown period and last notification time are managed by the instance attributes `self.cooldown` and `self.last_sms_time`.
        """
        now = time.time()

        if event_type == "ANTENNA_FALL":
            if now - self.last_sms_time >= self.cooldown:
                self._send_sms("⚠️ Alerta: La antena ha caído del eje vertical.")
                self.last_sms_time = now
            else:
                print("[NOTIFIER] SMS ya enviado recientemente. Esperando cooldown.")

    def _send_sms(self, msg):
        """
        Sends an SMS message using the configured Twilio client.

        Parameters:
            msg (str): The message content to be sent via SMS.

        Behavior:
            - If SMS sending is disabled (`self.send_sms` is False), logs a message and returns.
            - Attempts to send the SMS using Twilio API with the configured sender and recipient numbers.
            - Logs a success message if the SMS is sent.
            - Catches and logs any exceptions that occur during the sending process.
        """
        if not self.send_sms:
            print("[NOTIFIER] Envío de SMS desactivado.")
            return

        try:
            self.client.messages.create(
                body=msg,
                from_=self.twilio_from,
                to=self.twilio_to
            )
            print("[NOTIFIER] SMS enviado.")
        except Exception as e:
            print(f"[NOTIFIER] Error al enviar SMS: {e}")