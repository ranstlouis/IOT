#include <ESP8266WiFi.h>
const char* ssid="Verizon-SM-G920V-C3DC";  
const char* pass="ejuo652(";
#define DCPin D2
WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(DCPin,OUTPUT);
  
  connecttoWiFi();
  server.begin();
}
void connecttoWiFi(){
  //Connect to Wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(500);
  WiFi.disconnect();
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("SSID: ");
  Serial.println (WiFi.SSID());
  Serial.println("Successfully connected!!!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  // put your main code here, to run repeatedly:
  
    WiFiClient client = server.available();
    if (client) {
      String currentLine = "";      
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          //Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              client.print("<br>");
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            }
            else {      // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          }
          else if (c != '\r') {    // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
          if (currentLine.endsWith("GET /go")) {
          // Turns the Relay on
              digitalWrite(DCPin,HIGH);
              delay(2000);
              digitalWrite(DCPin,LOW);      
          }
          else {
            digitalWrite(DCPin,LOW);                       
          }
        }
      }
  }
}
