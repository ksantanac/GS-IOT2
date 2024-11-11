#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "mqtt.eclipseprojects.io";

WiFiClient espClient;
PubSubClient client(espClient);

const int potPin = 34;    // Pino para o potenciômetro (produção solar)
const int batteryPin = 35; // Pino para simular o nível de bateria

const char* mqtt_topic_solar = "solarPower";
const char* mqtt_topic_battery = "batteryLevel";

float solarPower = 0;
float batteryLevel = 0;

void setup() {
  Serial.begin(115200);
  pinMode(potPin, INPUT);
  pinMode(batteryPin, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setKeepAlive(15);
  client.setSocketTimeout(15);
}

void setup_wifi() {
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void publishData() {
  int potValue = analogRead(potPin);
  solarPower = map(potValue, 0, 4095, 0, 100);

  int batteryValue = analogRead(batteryPin);
  batteryLevel = map(batteryValue, 0, 4095, 0, 100);

  String solarPowerStr = String(solarPower);
  String batteryLevelStr = String(batteryLevel);

  client.publish(mqtt_topic_solar, solarPowerStr.c_str());
  client.publish(mqtt_topic_battery, batteryLevelStr.c_str());

  Serial.println("Dados publicados:");
  Serial.println("Produção Solar: " + solarPowerStr + " W");
  Serial.println("Nível de Bateria: " + batteryLevelStr + " %");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32ClientPublisher")) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tente novamente em 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  publishData();
  client.loop();
  delay(2000);
}
