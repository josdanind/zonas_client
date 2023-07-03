#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "utils.hpp"
#include <ArduinoJson.h>

uint8_t alarm_pin  = 2;
bool alarm_status = false;

void setup() {
  //Velocidad de comunicación
  Serial.begin(115200);
  pinMode(alarm_pin, OUTPUT);
  digitalWrite(alarm_pin, LOW);

  // Conexión WiFi
  ConnectWiFi();

  // Conexión Broker MQTT
  InitMqtt();
}

void loop() {
  HandleMqtt();

  if (alarm_status) {
    digitalWrite(alarm_pin, HIGH);

  }
  else {
    digitalWrite(alarm_pin, LOW);
  }
}