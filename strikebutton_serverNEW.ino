/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
int redPin = D0;
int greenPin = D6;
int triggerPin = D1;
bool unlocked = false;

char ssid[] = "SSID";               // SSID of your home WiFi
char pass[] = "PASSWORD";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 1, 100);            // IP address of the server
IPAddress gateway(192,168,1,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(redPin, HIGH);
    Serial.print(".");
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
  }

  server.begin();
  
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/
  pinMode(ledPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(triggerPin, LOW);
  digitalWrite(ledPin,HIGH);
  Serial.println("Ready");
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      Serial.println("client connected");
      if (client.readStringUntil('\r').equals("Lock")){
        if (unlocked){
          digitalWrite(triggerPin, HIGH);
          digitalWrite(triggerPin, LOW);
          digitalWrite(redPin, HIGH);
          digitalWrite(greenPin, LOW);
        }else{
          Serial.println("Lock");
          digitalWrite(triggerPin, HIGH);
          digitalWrite(triggerPin, LOW);
          digitalWrite(redPin, LOW);
          digitalWrite(greenPin, HIGH);
          delay(5000);
          digitalWrite(triggerPin, HIGH);
          digitalWrite(triggerPin, LOW);
          digitalWrite(redPin, HIGH);
          digitalWrite(greenPin, LOW);
        }
      }
      if (client.readStringUntil('\r').equals("Unlock")){
        Serial.println("Unlock");
        digitalWrite(triggerPin, HIGH);
        digitalWrite(triggerPin, LOW);
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
    }
    client.flush();
    client.stop();                // tarminates the connection with the client
    }
  }
}
