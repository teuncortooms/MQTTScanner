#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "Led.h"

// Update these with values suitable for your network.
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *mqtt_server = "192.168.2.85";
const char *mqtt_user = "admin1";
const char *mqtt_pass = "admin1";

// pins and millis
Led led(2);
const int ldrPin = 36;
long previousMillis = 0;
int timeBetweenReading = 3000;

// pubsub
WiFiClient espClient;
PubSubClient client(espClient);

void connectToWifi();
void publishLdrReading();
void connectToBroker();

void setup()
{
  Serial.begin(9600);
  connectToWifi();
  client.setServer(mqtt_server, 1883);
}

void loop()
{
  if (millis() - previousMillis > timeBetweenReading)
  {
    publishLdrReading();
    previousMillis = millis();
  }
}

void connectToWifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
}

void publishLdrReading(){
  if (!client.connected())
  {
    connectToBroker();
  }

  char msg[8];
  sprintf(msg, "%i", analogRead(ldrPin));
  client.publish("scanner", msg);
}

void connectToBroker()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp32", mqtt_user, mqtt_pass))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}