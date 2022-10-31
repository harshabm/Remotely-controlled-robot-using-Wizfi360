/*
 WizFi360 example: WebClient

 This sketch connects to google website using a WizFi360 module to
 perform a simple web search.
*/

#include "WizFi360.h"
#include <SPI.h>
#include <PubSubClient.h>

// setup according to the device you use
#define WIZFI360_EVB_PICO

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#if defined(ARDUINO_MEGA_2560)
SoftwareSerial Serial1(6, 7); // RX, TX
#elif defined(WIZFI360_EVB_PICO)
SoftwareSerial Serial2(6, 7); // RX, TX
#endif
#endif
/* Baudrate */
#define SERIAL_BAUDRATE   115200
#if defined(ARDUINO_MEGA_2560)
#define SERIAL1_BAUDRATE  115200
#elif defined(WIZFI360_EVB_PICO)
#define SERIAL2_BAUDRATE  115200
#endif


/* Wi-Fi info */
char ssid[] = "Harsha";       // your network SSID (name)
char pass[] = "Natsu6699";   // your network password
IPAddress server(192,168,1,33);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
// Initialize the Ethernet client object
WiFiClient ethClient;
PubSubClient client(ethClient);


int VRx = A0;
int VRy = A1;
int SW = 2;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      }return;
  }
}


void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_BAUDRATE);
  // initialize serial for WizFi360 module
#if defined(ARDUINO_MEGA_2560)
  Serial1.begin(SERIAL1_BAUDRATE);
#elif defined(WIZFI360_EVB_PICO)
  Serial2.begin(SERIAL2_BAUDRATE);
#endif
  // initialize WizFi360 module
#if defined(ARDUINO_MEGA_2560)
  WiFi.init(&Serial1);
#elif defined(WIZFI360_EVB_PICO)
  WiFi.init(&Serial2);
#endif

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    
  }
  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  Serial.println();
  Serial.println("Starting connection to MQTT server...");
  // if you get a connection, report back via serial
  client.setServer(server, 1883);
  client.setCallback(callback);
  delay(1500);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
}

    
void loop() {
 xPosition = analogRead(VRx);
 yPosition = analogRead(VRy);
 SW_state = digitalRead(SW);
 mapX = map(xPosition, 0, 1023, -512, 512);
 mapY = map(yPosition, 0, 1023, -512, 512);

 if(mapX > 500){
 Serial.print("X: ");
 Serial.println(mapX);
 Serial.println("Right");
 client.publish("outTopic","R");
 }
  
 if(mapX < -500){
 Serial.print("X: ");
 Serial.println(mapX);
 Serial.println("Left");
 client.publish("outTopic","L");
 }

 if(mapY < -500){
 Serial.print(" | Y: ");
 Serial.println(mapY);
 Serial.println("Front");
 client.publish("outTopic","F");
 }

 if(mapY > 500){
 Serial.print(" | Y: ");
 Serial.println(mapY);
 Serial.println("Back");
 client.publish("outTopic","B");
 }
  
 Serial.print(" | Button: ");
 Serial.println(SW_state);
 delay(10); 
 if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
