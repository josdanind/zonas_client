#include <WiFi.h>
#include <PubSubClient.h>
#include <functional>

// WiFi y MQTT - Credenciales de inicio
#include "data.h"
#include "callback.h"

// ###################
// ## CONEXIÓN WIFI ##
// ###################

void ConnectWiFi()
{
    // Configuración WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Esperando la conexión
    Serial.print("Estableciendo conexión WiFi ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.printf("\nConectado a: %s\n", ssid);
    Serial.print("Direccion IP: ");
    Serial.println(WiFi.localIP());
}

// ##########################
// ## CONEXIÓN BROKER MQTT ##
// ##########################

WiFiClient mqttEmqx;
PubSubClient client(mqttEmqx);

// Configuración del Broker MQTT
// --> Se establece el servidor y el callback para procesar el mensaje
void InitMqtt()
{
    client.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
    client.setCallback(callback);
}

// Se conecta o reconecta  al Broker MQTT
/// --> Si logra la conexión, se suscribe al topic y publica un mensaje
/// --> No no lo logra, intenta nuevamente después de 2 segundos.
void ConnectMqtt()
{
    unsigned long endTime = millis() + 2000;

    Serial.print("Iniciando conexión con el Broker MQTT");

    while(millis() < endTime)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\n");

    if(client.connect(MQTT_CLIENT_ID, MQTT_CLIENT_USERNAME, MQTT_CLIENT_PASSWORD)){
        Serial.println("Conexión Exitosa!\n");
        client.publish(MQTT_TOPIC, "Huy zonas");
        client.subscribe(MQTT_TOPIC);
        Serial.print("Suscrito al tópico:");
        Serial.println(MQTT_TOPIC);
    } else {
        Serial.print("No se puedo establecer la conexión:  ");
        Serial.print(client.state());
        Serial.print("\nIntentando nuevamente");

        unsigned long endTime = millis() + 2000;

        while(millis() < endTime)
        {
            delay(500);
            Serial.print(".");
        }

        Serial.print("\n\n");
    }
}

// Gestiona la conexión con el Broker MQTT
// --> Si no esta conectado, intenta establecer la conexión.
// --> Si esta conectado, llama al MQTT loop 
void HandleMqtt()
{
    if (!client.connected())
    {
        ConnectMqtt();
    }

    client.loop();
}