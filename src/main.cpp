#include "SPIFFS.h"
#include <Arduino.h>
#include <M5Unified.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
// put function declarations here:
//
#define SERVO_PIN 21
#define MOTOR_PIN 22
#define NEOPIXEL_PIN 16
#define NEOPIXEL_NUM 10

Servo futa;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_NUM, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
TaskHandle_t cleadTaskHandle = NULL;
void cleanTask(void *);

void setup()
{
  Serial.begin(115200);

  auto cfg = M5.config();

  M5.begin(cfg);
  SPIFFS.begin(true);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  futa.attach(SERVO_PIN, 500, 2400);

  strip.begin();

  while (!Serial)
  {
    delay(1);
  }
  M5.Lcd.drawPngFile(SPIFFS, "/default.png");
}

void onSerialAvailable()
{
  String inp = Serial.readStringUntil('\n');

  if (inp.charAt(0) == '0') // on_unko
  {
    Serial.println("on_unko");
    if (cleadTaskHandle != NULL && eTaskGetState(cleadTaskHandle) != eDeleted)
    {
      vTaskDelete(cleadTaskHandle);
    }
    M5.Lcd.drawPngFile(SPIFFS, "/btgs.png");
  }
  if (inp.charAt(0) == '1') // on_clean
  {
    xTaskCreatePinnedToCore(cleanTask, "cleanTask", 4096, NULL, 1, &cleadTaskHandle, 0);
    Serial.println("on_clean");
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

void cleanTask(void *)
{
  M5.Lcd.drawPngFile(SPIFFS, "/nagare.png");
  delay(1000);
  M5.Lcd.drawPngFile(SPIFFS, "/default.png");
  vTaskDelete(NULL);
}
