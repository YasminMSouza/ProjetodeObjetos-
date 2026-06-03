#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// CONFIGURAÇÕES DO WI-FI (dados somente de exemplo)
const char* ssid = "Marii's Galaxy S24plus";
const char* password = "Mari123"; // <--- Coloque a senha do seu Wi-Fi aqui

// CONFIGURAÇÕES DO MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "pet/casinha/temperatura";

// SEUS DADOS DE AUTENTICAÇÃO (dados somente de exemplo)
const char* mqtt_user = "murilofranciscon";
const char* mqtt_pass = "12456";

// CONFIGURAÇÕES DOS SENSORES
#define DHTPIN 15
#define DHTTYPE DHT11
#define TRIG 32
#define ECHO 35

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(100);
  Serial.println();
  Serial.print("Conectando em: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função de conexão com Usuário e Senha
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT com autenticação...");
    
    String clientId = "ESP32Client-";
    clientId += String(random(0, 0xffff), HEX);
    
    // Enviando ID, Usuário e Senha para o Broker
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("conectado com sucesso!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(DHTPIN, INPUT_PULLUP);
  
  dht.begin();
  digitalWrite(TRIG, LOW);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura dos Sensores
  float temperatura = dht.readTemperature();

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracao = pulseIn(ECHO, HIGH, 30000);
  float distancia = (duracao * 0.034) / 2;

  // Montagem do JSON
  String jsonPayload = "{";
  
  jsonPayload += "\"temperatura\":";
  if (isnan(temperatura)) {
    jsonPayload += "null";
  } else {
    jsonPayload += String(temperatura, 1);
  }
  
  jsonPayload += ",\"distancia\":";
  if (duracao == 0) {
    jsonPayload += "null";
  } else {
    jsonPayload += String(distancia, 1);
  }
  
  jsonPayload += "}";

  // Envio
  Serial.print("Enviando dados: ");
  Serial.println(jsonPayload);
  
  client.publish(mqtt_topic, jsonPayload.c_str());

  delay(3000);
}