#include <ESP8266WiFi.h>
const char* ssid="Verizon-SM-G920V-C3DC";  
const char* pass="ejuo652(";
#define ledPin D7

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  
  Serial.print("Connecting to..");
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
  
  Serial.print("IP Address allotted to NodeMcu ESP: ");
  Serial.println(WiFi.localIP());
  
  //Start LED off
  pinMode(ledPin, OUTPUT);              
  digitalWrite(ledPin, LOW); 
  server.begin();
   
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();

  if (client) {                             // if you get a client,
   // Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

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

        // Check to see if the client request was "GET /on" or "GET /off":
        if (currentLine.endsWith("GET /on")) {
          digitalWrite(ledPin, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /off")) {
          digitalWrite(ledPin, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("client disconnected");
  }
}
