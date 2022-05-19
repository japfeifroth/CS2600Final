#include <WiFi.h>
#include <ArduinoMqttClient.h>

#define LED_BUILTIN  2
#define LED_BUILTIN  0
const char *ssid_Router     = ""; //Enter the router name
const char *password_Router = ""; //Enter the router password.

WiFiClient wclient;
MqttClient mqttClient(wclient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "LED";
char c;
int ledpin[] = {2,0,4,5};
int a[20];

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Setup start");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_Router, password_Router);
  Serial.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup End");

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
  pinMode(ledpin[0], OUTPUT);
  pinMode(ledpin[1], OUTPUT);
  pinMode(ledpin[2], OUTPUT);
  pinMode(ledpin[3], OUTPUT);
}

void loop() {
  a: int messageSize = mqttClient.parseMessage();
  if(messageSize > 0 && messageSize < 5)
  {
  for(int i = 0; i < messageSize ; i++)
  {
    Serial.print("char ");
    Serial.print(i + 1);
    digitalWrite(ledpin[i], HIGH);
    Serial.println();
    delay(1000);
  }

  Serial.println();
  for(int i = messageSize - 1; i >= 0; i--)
  {
    digitalWrite(ledpin[i], LOW);
    delay(1000);
  }
 }
 else
 {
  goto a;
 }
     mqttClient.beginMessage(topic);
     mqttClient.print("The message ");
     mqttClient.print("\"");
     while (mqttClient.available()) {
     mqttClient.print((char)mqttClient.read());
    }
    mqttClient.print("\"");
    mqttClient.print(" has ");
    mqttClient.print(messageSize);
    mqttClient.print(" characters");
    mqttClient.endMessage();
     while (mqttClient.available()) {
     Serial.print((char)mqttClient.read());
    }  
}

