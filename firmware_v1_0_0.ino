#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include <PubSubClient.h>
const char* mqtt_server = "xx.xx.xx.xx";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    Serial.begin(115200);
    WiFiManager wm;

    bool res;

    res = wm.autoConnect("ap","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        client.setServer(mqtt_server, 9883);
    }

}

void reconnect() {  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");    
    if (client.connect("Client")) {
      Serial.println("connected");      
      //client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_pub(){
  client.publish("topic", "test");
  Serial.println("published");  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 30000)
  {
    lastMsg = now;
    mqtt_pub();
  }
}
