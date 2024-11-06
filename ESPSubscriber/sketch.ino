#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic_solar = "solarPower";
const char* mqtt_topic_battery = "batteryLevel";

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2;  
float solarPower = 0;
float batteryLevel = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}

void setup_wifi() {
  delay(10);
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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32ClientReceiver")) {
      Serial.println("conectado");
      client.subscribe(mqtt_topic_solar);
      client.subscribe(mqtt_topic_battery);
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tente novamente em 5 segundos");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == mqtt_topic_solar) {
    solarPower = message.toFloat();
    Serial.println("Produção Solar recebida: " + String(solarPower) + " W");
  } else if (String(topic) == mqtt_topic_battery) {
    batteryLevel = message.toFloat();
    Serial.println("Nível de Bateria recebido: " + String(batteryLevel) + " %");
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Controle do LED com base nos valores de solarPower e batteryLevel
  if (solarPower > 50 && batteryLevel > 20) {
    digitalWrite(ledPin, HIGH); // Liga o LED para indicar boa produção e carga
  } else {
    digitalWrite(ledPin, LOW); // Desliga o LED se os níveis estão baixos
  }

  delay(1000);
}
