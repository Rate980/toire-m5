#include "SPIFFS.h"
#include <Arduino.h>
#include <M5Unified.h>
#include <ESP32Servo.h>
// put function declarations here:
//
#define SERVO_PIN 21
#define MOTOR_PIN 22
Servo futa;

void motorTask(void *);

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  auto cfg = M5.config();

  M5.begin(cfg);
  SPIFFS.begin(true);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  M5.Lcd.drawPngFile(SPIFFS, "/mayo.png", 0, 0);
  futa.attach(SERVO_PIN, 500, 2400);

  xTaskCreatePinnedToCore(
      motorTask,   /* Task function. */
      "motorTask", /* name of task. */
      1024,        /* Stack size of task */
      NULL,        /* parameter of the task */
      1,           /* priority of the task */
      NULL,        /* Task handle to keep track of created task */
      0);          /* pin task to core 0 */

  // ledcSetup(0, 50, 8);
  // ledcAttachPin(SERVO_PIN, 0);

  while (!Serial || !Serial2)
  {
    delay(1);
  }
}

void loop()
{
  futa.write(0);
  delay(1000);
  futa.write(90);
  delay(1000);
  futa.write(180);
  delay(1000);
}

void motorTask(void *)
{
  int motorState = LOW;
  while (1)
  {
    M5.update();
    if (M5.BtnA.wasPressed())
    {
      Serial.println("Button A was pressed");
      motorState = !motorState;
      digitalWrite(MOTOR_PIN, motorState);
    }
    delay(100);
  }
}
