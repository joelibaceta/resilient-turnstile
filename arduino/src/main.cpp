#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include <DHT.h>

#include "sensors/Accelerometer.h"
#include "sensors/TempHumiditySensor.h"
#include "sensors/ImpactSensor.h"
#include "buzzer/Buzzer.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHTPIN 8
#define DHTTYPE DHT11
#define BUZZER_PIN 7  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


bool showAccel = true;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  initBuzzer(7);
  buzzerClick(); 

  delay(500);

  initAccelerometer();
  initTempHumiditySensor(8);
  initImpactSensor(11);

  buzzerDoubleBeep();

  // OLED INIT
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("❌ OLED no detectado"));
    while (true);
  }
     // Asegura que se apague

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("OLED OK"));
  display.display();
  delay(1000);

  // MPU INIT
  Serial.println(F("✅ MPU6050 conectado"));
  display.println(F("MPU OK"));
  display.display();
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);

  if (detectImpact()) {
    Serial.println("EVENT,impact_detected");
    buzzerAlert();
    delay(1000);   
    return;
  }

  if (!isVertical()) {
    buzzerAlert();  // Reproduce los pitidos de alerta si no está en vertical
    Serial.println("EVENT,not_vertical");
  }


  if (showAccel) {
    float ax, ay, az;
    readAccel(ax, ay, az);
    float gx, gy, gz;
    readGyro(gx, gy, gz);

    Serial.println("METRIC,ACEL," + String(ax, 2) + "," + String(ay, 2) + "," + String(az, 2));
    Serial.println("METRIC,GYRO," + String(gx, 2) + "," + String(gy, 2) + "," + String(gz, 2));

    display.print("X="); display.println(ax);
    display.print("Y="); display.println(ay);
    display.print("Z="); display.println(az);
  } else {
    float t = readTemperature();
    float h = readHumidity();

    Serial.println(String("METRIC,TEMP,") + String(t, 2));
    Serial.println(String("METRIC,HUMIDITY,") + String(h, 2));

    if (isnan(t) || isnan(h)) {
      display.println("DHT Fail");
    } else {
      display.print("T: "); display.print(t); display.println(" C");
      display.print("H: "); display.print(h); display.println(" %");
    }
  }

  display.display();
  showAccel = !showAccel;  // alternar cada loop
  delay(1000);
}