/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

String input;
String code = "13255";
String unlockCode = "13256";
int counter;

char ssid[] = "TheHub";           // SSID of your home WiFi
char pass[] = "sigmaalphaepsilon";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,1,120);       // the fix IP address of the server
WiFiClient client;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {D0,D1,D2,D3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {D4,D5,D6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  input = "";
}
void loop(){
  char key = keypad.getKey();
  if (counter == 1000000){
    input = "";
    Serial.println("Counter Reset");
    counter = 0;
    goToSleep();
  }
  if (key){
    counter = 0;
    Serial.println(key);
    if (key != '#'){
      input += key;
    }
    else{
      if (input.equals(code)){
        Serial.println("Success.");
        client.connect(server, 80);   // Connection to the server
        client.println("Lock\r");
        goToSleep();
      }else if (input.equals(unlockCode)){
        Serial.println("Unlock");
        client.connect(server, 80);   // Connection to the server
        client.println("Unlock\r");
        goToSleep();
      }
      else{
        input = "";
        Serial.println("Incorrect Code.");
      }
    }
  }
  counter++;
}

void goToSleep(){
  Serial.println("Sleeping");
  ESP.deepSleep(0);  
}

