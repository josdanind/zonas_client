#include <Arduino.h>
#include <ArduinoJson.h>

extern bool alarm_status;

void callback(char *topic, byte *payload, unsigned int length) {
    // Parsear JSON
    StaticJsonDocument<200> messageMqtt;
    DeserializationError error = deserializeJson(messageMqtt, payload, length);

    if (error) {
        Serial.print("Error al analizar el JSON: ");
        Serial.println(error.c_str());
        return;
    }

    const String state = messageMqtt["pinState"];
    const String by = messageMqtt["from"];

    alarm_status = (state == "ON") ? true : alarm_status ;
    alarm_status = (state == "OFF") ? false : alarm_status ;

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message sent by: ");
    Serial.println(by);
    Serial.print("Alarm status: ");
    Serial.println(state + "\n");
}