#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid="Verizon-SM-G920V-C3DC";  
const char* pass="ejuo652(";
#define ledPin 13
#define buttonPin 16
#define IFTTT_API_KEY "bXWIK4PRc9gw59mlUzG04G"
#define IFTTT_EVENT_NAME "pushbutton"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while(!Serial) { 
  }
  Serial.println(" ");// print an empty line before and after Button Press    
  Serial.println("Button Pressed");
  Serial.println(" ");// print an empty line
  delay(500);
  
  //Connect to Wifi
  connecttoWiFi();
  
  //Send trigger
  sendtrigger();
  
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin,LOW);
  
  ESP.deepSleep(buttonPin);
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
  Serial.println("SSID: ");
  Serial.println (WiFi.SSID());
  Serial.println("Successfully connected!!!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
void sendtrigger() {
  HTTPClient http;
  String url = "http://maker.ifttt.com/trigger/" + String(IFTTT_EVENT_NAME) + "/with/key/" + String(IFTTT_API_KEY);
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST("title=foo&body=bar&userId=1");
  http.writeToStream(&Serial);
  http.end();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("This shouldn't get printed");
}
