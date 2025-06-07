#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Arduino Uno doesn't use reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();  // En Arduino Uno usa A4 (SDA) y A5 (SCL)

  // OLED INIT
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("❌ OLED no detectado"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("OLED OK 😊"));
  display.display();

  delay(1000);

  // MPU INIT
  mpu.initialize();
 
  Serial.println(F("✅ MPU6050 conectado"));
  display.println(F("MPU OK 😊"));
  display.display();
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  Serial.print("Acel: ");
  Serial.print("X="); Serial.print(ax);
  Serial.print(" Y="); Serial.print(ay);
  Serial.print(" Z="); Serial.println(az);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("X="); display.println(ax);
  display.print("Y="); display.println(ay);
  display.print("Z="); display.println(az);
  display.display();

  // Giroscopio
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  Serial.print("Giro: ");
  Serial.print("X="); Serial.print(gx);
  Serial.print(" Y="); Serial.print(gy);
  Serial.print(" Z="); Serial.println(gz);

  delay(1000);
}