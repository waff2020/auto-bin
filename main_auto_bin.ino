#define maxH 50
#define cap_time 3
#define open_angle 10
#define close_angle 155

#define debug 0

#define trigPin 3
#define echoPin 2
#define sensorVCC 4
#define MOSFETpin 6
#define servoPin 5

#include "LowPower.h"
#include <Servo.h>
Servo servo;

long duration;
byte distance;
unsigned long open_timer;
boolean is_open;

void setup() {
  Serial.begin(9600);
  if (debug) Serial.println("system start");
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, 0);
  pinMode(MOSFETpin, OUTPUT);
  digitalWrite(MOSFETpin, 0);
  pinMode(echoPin, INPUT);

  pinMode(sensorVCC, OUTPUT);
  digitalWrite(sensorVCC, 0);

  open_cap();
}

void open_cap() {
  if (debug) Serial.println("open");
  digitalWrite(MOSFETpin, 1);
  delay(1);
  servo.write(open_angle); 
  delay(1000);
  digitalWrite(MOSFETpin, 0);
  is_open = 1;
}

void close_cap() {
  if (debug) Serial.println("close");
  digitalWrite(MOSFETpin, 1);
  delay(1);
  servo.write(close_angle);
  delay(1000);
  digitalWrite(MOSFETpin, 0);
  is_open = 0;
}

void loop() {
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);

  digitalWrite(sensorVCC, 1);
  measure();
  distance = measure();
  digitalWrite(sensorVCC, 0);
  if (debug) {
    Serial.println(distance);
    delay(500);
  }

  if (distance > 5 && distance < maxH) {
    open_timer = 0;
    if (!is_open) {
      open_cap();
    }
  } else {
    if (is_open) {
      open_timer++;
      if (open_timer > cap_time) {
        close_cap();
      }
    }
  }
}

byte measure() {
  digitalWrite(trigPin, 0);
  delayMicroseconds(5);
  digitalWrite(trigPin, 1);
  delayMicroseconds(15);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, HIGH);
  return (duration / 2) / 29.1;
}
