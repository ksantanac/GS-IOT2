#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "mqtt.eclipseprojects.io";

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_topic_solar = "solarPower";
const char* mqtt_topic_battery = "batteryLevel";

const int ledPin = 2; // Pino D2 para o LED

float solarPower = 0;
float batteryLevel = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED inicialmente apagado

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

void callback(char* topic, byte* message, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  
  // Atualizar valores com base no tópico recebido
  if (String(topic) == mqtt_topic_solar) {
    solarPower = msg.toFloat();
    Serial.print("Produção Solar: ");
    Serial.println(solarPower);
  } else if (String(topic) == mqtt_topic_battery) {
    batteryLevel = msg.toFloat();
    Serial.print("Nível de Bateria: ");
    Serial.println(batteryLevel);
  }

  // Controle do LED
  if (solarPower > 50 && batteryLevel > 20) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32ClientSubscriber")) {
      Serial.println("Conectado");
      client.subscribe(mqtt_topic_solar);
      client.subscribe(mqtt_topic_battery);
      Serial.println("Inscrito nos tópicos: solarPower e batteryLevel");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
