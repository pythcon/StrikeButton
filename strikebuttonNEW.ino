/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
int unlockPin = D1;

char ssid[] = "SSID";           // SSID of your home WiFi
char pass[] = "PASSWORD";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,1,100);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/
  pinMode(ledPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);
  digitalWrite(ledPin,HIGH);
  digitalWrite(unlockPin,HIGH);
}

void loop () {
  client.connect(server, 80);   // Connection to the server

  if (digitalRead(unlockPin)==LOW){
    client.println("Unlock\r");
    Serial.println("Unlock");
  }else{
    client.println("Lock\r");
    Serial.println("Lock");
  }
  digitalWrite(ledPin,LOW);
  client.flush();
  delay(2000);
  ESP.deepSleep(0);

  
}
