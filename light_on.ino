#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid="Verizon-SM-G920V-C3DC";  
const char* pass="ejuo652(";
#define ledPin_gr D7
#define ledPin_yellow  D6
#define IFTTT_API_KEY "bXWIK4PRc9gw59mlUzG04G"
WiFiServer server(80);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  connecttoWiFi();
  //Start LED off
  pinMode(ledPin_gr, OUTPUT);              
  digitalWrite(ledPin_gr, LOW);
  pinMode(ledPin_yellow, OUTPUT);              
  digitalWrite(ledPin_yellow, LOW); 
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
void sendtrigger(String event_name) {
  HTTPClient http;
  String url = "http://maker.ifttt.com/trigger/" + event_name + "/with/key/" + String(IFTTT_API_KEY);
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST("title=foo&body=bar&userId=1");
  http.writeToStream(&Serial);
  http.end();
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

        // Check to see if the client request was for green LED on or off:
        if (currentLine.endsWith("GET /gr_on")) {
          if(digitalRead(ledPin_gr) != HIGH){
            sendtrigger("gr_lighton");
            Serial.println("");
          }
            digitalWrite(ledPin_gr, HIGH);               // Turns the Green LED on
          
        }
        if (currentLine.endsWith("GET /gr_off")) {
          if(digitalRead(ledPin_gr) != LOW){
            
            Serial.println("");
          }
          digitalWrite(ledPin_gr, LOW);                // Turns the Green LED off
        }
        // Check to see if the client request was for yellow LED on or off:
        if (currentLine.endsWith("GET /yellow_on")) {
          if(digitalRead(ledPin_yellow) != HIGH){
            sendtrigger("yellow_lighton");
            Serial.println("");
          }
            digitalWrite(ledPin_yellow, HIGH);               // Turns the yellow LED on
          
        }
        if (currentLine.endsWith("GET /yellow_off")) {
          if(digitalRead(ledPin_yellow) != LOW){
            Serial.println("");
          }
          digitalWrite(ledPin_yellow, LOW);                // Turns the yellow LED off
        }
      }
      }
  }
}
