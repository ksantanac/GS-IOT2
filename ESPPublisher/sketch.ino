#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// Conexão com o Wi-Fi do Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Conexão com o servidor do Broker
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic_solar = "solarPower";
const char* mqtt_topic_battery = "batteryLevel";

WiFiClient espClient;
PubSubClient client(espClient);

const int dhtPin = 15;
DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(dhtPin, DHTesp::DHT22);  

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    client.connect("ESP32ClientPublisher");
  }
  client.loop();

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  // Simulando os valores de produção solar e nível de bateria
  float solarPower = data.temperature * 5;  // Potência solar simulada
  float batteryLevel = data.humidity;       // Nível de bateria simulada

  String solarPowerStr = String(solarPower, 2);
  String batteryLevelStr = String(batteryLevel, 2);

  Serial.println("Publicando Produção Solar: " + solarPowerStr + " W");
  Serial.println("Publicando Nível de Bateria: " + batteryLevelStr + " %");

  // Publica os valores simulados nos tópicos MQTT
  client.publish(mqtt_topic_solar, solarPowerStr.c_str());
  client.publish(mqtt_topic_battery, batteryLevelStr.c_str());

  delay(2000); // Ajuste o delay conforme necessário
}
