#include <ESP8266WiFi.h>

char ssid[] = "ESP8266_NETWORK";
char pass[] = "genericpassword";

WiFiServer server(80);

void createWifi(char* ssid, char* pass) {
  //Serial.println("Starting Network");
  //Serial.println(ssid);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  //Serial.println("Success!");
  /*
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Trying to connect...");
  }

  Serial.println("Successfully Set Up");
  Serial.println("IP:");
  Serial.println(WiFi.localIP());
  */
}

void setup() {
  Serial.begin(57600);
  createWifi(ssid, pass);
  //Serial.println("Ehh Limao");
  server.begin();
  // Server stuff from
  // http://www.esp8266.com/viewtopic.php?f=32&t=8329&start=4
  //Serial.print("Use this URL to connect: ");
  //Serial.print("http://");
  //Serial.print(WiFi.softAPIP());
  //Serial.println("/");
  delay(500);
  
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    //Serial.println("Something wrong with client");
    return;
  }
 
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Connect-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  s += "<h1> WAT Control Interface Webpage </h1>";
  
  s += ("Click <a href=\"/status=cw\">here</a> to spin motors clockwise<br>");
  s += ("Click <a href=\"/status=ccw\">here</a> to spin motors counter-clockwise<br>");
  s += ("Click <a href=\"/status=stop\">here</a> to stop motors<br>");

  s += "</html>\n";

  String request = client.readStringUntil('\r');
  //Serial.print("Client request read: ");
  //Serial.println(request);
  client.flush();

  
  if(request.indexOf("/status=cw") != -1) {
    Serial.print("1");
  }
  if(request.indexOf("/status=ccw") != -1) {
    Serial.print("2");
  }
  if(request.indexOf("/status=stop") != -1) {
    Serial.print("3");
  }
  
  
  client.print(s);
  delay(10);
  //Serial.println("Client disconnected");
  // server.send(200, "text/plain", "hello from ESP9266!");
}

