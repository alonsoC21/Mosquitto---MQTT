import paho.mqtt.client as mqtt
import time

# --- Configuración MQTT ---
broker = "192.168.0.137"  
puerto = 1883
topico = "prueba/esp32"

# Crear el cliente
cliente = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

print(f"Conectando al broker en {broker}...")
cliente.connect(broker, puerto, 60)

contador = 0

try:
    # Bucle infinito publicando cada 5 segundos
    while True:
        contador += 1
        payload = f"Mensaje simulado #{contador} desde script Python"
        
        # Publicar el mensaje
        cliente.publish(topico, payload)
        print(f"Publicando en '{topico}': {payload}")
        
        time.sleep(5)
except KeyboardInterrupt:
    print("\nDesconectando...")
    cliente.disconnect()