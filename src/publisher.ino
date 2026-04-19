#include <WiFi.h>
#include <PubSubClient.h>

// --- Configuración de Red WiFi ---
const char* ssid = "Tenda_E98DD0_5G";
const char* password = "Di Mi Nombre E98";

// --- Configuración MQTT ---

const char* mqtt_server = "192.168.0.137"; 
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Bucle hasta que estemos conectados
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    // Intentar conectar con un ID de cliente aleatorio
    String clientId = "ESP32Client-";
    clientId += String(random(0, 0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado al broker Mosquitto");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Mantiene viva la conexión con el broker

  unsigned long now = millis();
  // Publicar un mensaje cada 5 segundos (5000 ms)
  if (now - lastMsg > 5000) {
    lastMsg = now;
    value++;
    
    // Crear el mensaje a enviar
    String payload = "Mensaje #" + String(value) + " desde ESP32";
    
    Serial.print("Publicando en tópico 'prueba/esp32': ");
    Serial.println(payload);
    
    // Publicar en el tópico
    client.publish("prueba/esp32", payload.c_str());
  }
}