#include <ESP8266WiFi.h>

// Wifi networks name and password
char ssid[] = "ESP8266_NETWORK";
char pass[] = "genericpassword";

WiFiServer server(80);

// Creates wifi network
void createWifi(char* ssid, char* pass) {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
}

void setup() {
  // baud rate for the Nano is set to 57600
  Serial.begin(57600);
  
  createWifi(ssid, pass);

  // Server stuff from
  // http://www.esp8266.com/viewtopic.php?f=32&t=8329&start=4
  server.begin();

  delay(500);
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Connect-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  s += "<h1> WAT Control Interface </h1>";

  s += "<form>";
  s += "First Stop: <br>"
  s += "<select id = \"firststop\">"
  s += "<option value1 = \"/red\"> RED </option>"
  s += "<option value1 = \"/blue\"> BLUE </option>"
  s += "<option value1 = \"/green\"> GREEN </option>"
  s += "</select>"
  s += "<br>"
  s += "<br>"
  s += "Final Destination: <br>"
  s += "<select id = \"destination\">"
  s += "<option value2 = \"/red\"> RED</option>"
  s += "<option value2 = \"/blue\"> BLUE </option>"
  s += "<option value2 = \"/green\"> GREEN </option>"
  s += "</select>"
  s += "<br>"
  s += "<br>"
  s += "<button class=\"button\">Launch</a>"
  s += "</form>"

  s += "</html>\n";

  String request = client.readStringUntil('\r');
  client.flush();

  // sends 
    if(request.indexOf("/redgreen") != -1) {
    Serial.print("1");
  }
    if(request.indexOf("/redblue") != -1) {
    Serial.print("2");
  }
    if(request.indexOf("/greenred") != -1) {
    Serial.print("3");
  }
    if(request.indexOf("/greenblue") != -1) {
    Serial.print("4");
  }
    if(request.indexOf("/bluered") != -1) {
    Serial.print("5");
  }
      if(request.indexOf("/bluegreen") != -1) {
    Serial.print("6");
  }
  
  // Sends the HTML file made from string s to the client
  client.print(s);
  delay(10);
}

