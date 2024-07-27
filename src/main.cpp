#include "SPIFFS.h"
#include <Arduino.h>
#include <M5Unified.h>
#include <ESP32Servo.h>
// put function declarations here:
//
#define SERVO_PIN 21
#define MOTOR_PIN 22
Servo futa;

void setup()
{
  Serial.begin(115200);

  auto cfg = M5.config();

  M5.begin(cfg);
  SPIFFS.begin(true);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  M5.Lcd.drawPngFile(SPIFFS, "/mayo.png", 0, 0);
  futa.attach(SERVO_PIN, 500, 2400);

  while (!Serial)
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

// void motorTask(void *)
// {
//   int motorState = LOW;
//   while (1)
//   {
//     M5.update();
//     if (M5.BtnA.wasPressed())
//     {
//       Serial.println("Button A was pressed");
//       motorState = !motorState;
//       digitalWrite(MOTOR_PIN, motorState);
//     }
//     delay(100);
//   }
// }
