#include "SPIFFS.h"
#include <Arduino.h>
#include <M5Unified.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
// put function declarations here:
//
#define SERVO_PIN 21
#define MOTOR_PIN 22
#define NEOPIXEL_PIN 17
#define NEOPIXEL_NUM 10

Servo futa;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_NUM, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
TaskHandle_t cleadTaskHandle = NULL;
void cleanTask(void *);
void motorTask(void *);
void openServo();
void closeServo();

void setup()
{
  Serial.begin(115200);

  auto cfg = M5.config();

  M5.begin(cfg);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  futa.attach(SERVO_PIN, 500, 2400);

  strip.begin();
  closeServo();

  while (!Serial)
  {
    delay(1);
  }
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  M5.Lcd.drawPngFile(SPIFFS, "/default.png", 0, 0);
}

void onSerialAvailable()
{
  String inp = Serial.readStringUntil('\n');
  Serial.println(inp);

  if (inp.charAt(0) == '0') // on_unko
  {
    Serial.println("on_unko");
    openServo();
    if (cleadTaskHandle != NULL && eTaskGetState(cleadTaskHandle) != eDeleted)
    {
      vTaskDelete(cleadTaskHandle);
    }
    M5.Lcd.drawPngFile(SPIFFS, "/btgs.png");
  }
  if (inp.charAt(0) == '1') // on_clean
  {
    M5.Lcd.drawPngFile(SPIFFS, "/nagare.png");
  }
  if (inp.charAt(0) == '2') // on_clean_done
  {
    closeServo();
    M5.Lcd.drawPngFile(SPIFFS, "/default.png");
    xTaskCreatePinnedToCore(motorTask, "motorTask", 4096, NULL, 1, NULL, 0);
  }
  if (inp.charAt(0) == '3') // reset
  {
    M5.Lcd.drawPngFile(SPIFFS, "/default.png");
  }
}

void loop()
{
  M5.update();
  if (Serial.available())
  {
    onSerialAvailable();
  }
  delay(1);
}

void motorTask(void *)
{
  delay(1000);
  digitalWrite(MOTOR_PIN, HIGH);
  delay(1000);
  digitalWrite(MOTOR_PIN, LOW);
  vTaskDelete(NULL);
}

void cleanTask(void *)
{
  M5.Lcd.drawPngFile(SPIFFS, "/nagare.png");
  delay(1000);
  closeServo();
  M5.Lcd.drawPngFile(SPIFFS, "/default.png");
  xTaskCreatePinnedToCore(motorTask, "motorTask", 4096, NULL, 1, NULL, 0);
  vTaskDelete(NULL);
}

void openServo()
{
  futa.write(0);
}

void closeServo()
{
  futa.write(180);
}
