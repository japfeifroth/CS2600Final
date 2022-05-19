
#include <WiFi.h>
#include <ArduinoMqttClient.h>

#define LED_BUILTIN  2
const char *ssid_Router     = "Froth2020 2.4 GHz"; //Enter the router name
const char *password_Router = "Pfeifroth2020"; //Enter the router password.

WiFiClient wclient;
MqttClient mqttClient(wclient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "LED";
char c;

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
  mqttClient.onMessage(onMqttMessage);

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
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  mqttClient.poll();
  int input;
  scanf("%d", &input);
}
void onMqttMessage(int messageSize) {

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    c = (char)mqttClient.read();
    Serial.print(c);
  }
  Serial.println();
    if(c == '1')
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    mqttClient.beginMessage(topic);
    mqttClient.print("ON");
    mqttClient.endMessage();
  }
  if(c == '2')
  {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    mqttClient.beginMessage(topic);
    mqttClient.print("OFF");
    mqttClient.endMessage();
  }
}
