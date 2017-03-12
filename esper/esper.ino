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

  s += "<body>";
  s += "<h1> WAT Control Interface </h1>";

  s += "<form>";
  
  s += "First Stop: <br>";
  s += "<select id = \"firststop\">";
  s += "<option value = \"/redstart\"> RED </option>";
  s += "<option value = \"/bluestart\"> BLUE </option>";
  s += "<option value = \"/greenstart\"> GREEN </option>";
  s += "</select>";
  
  s += "<br>";
  s += "<br>";

  s += "Time stopped at first stop (seconds): <br>";
  s += "<select id = \"time\">";
  s += "<option value = \"/5sec\"> 5 </option>";
  s += "<option value = \"/10sec\"> 10 </option>";
  s += "<option value = \"/30sec\"> 30 </option>";
  s += "</select>";

  s += "<br>";
  s += "<br>";
  
  s += "Final Destination: <br>";
  s += "<select id = \"destination\">";
  s += "<option value = \"/redend\"> RED</option>";
  s += "<option value = \"/blueend\"> BLUE </option>";
  s += "<option value = \"/greenend\"> GREEN </option>";
  s += "</select>";

  s += "<script>";
  s += "function sendRequest() {";
  s += "var ar = document.getElementById(\"firststop\");";
  s += "var at = ar.options[ar.selectedIndex].value;";
  s += "var br = document.getElementById(\"destination\");";
  s += "var bt = br.options[br.selectedIndex].value;";
  s += "var cr = document.getElementById(\"time\");";
  s += "var ct = cr.options[cr.selectedIndex].value;";
  s += "var send = at + bt + ct;";
  s += "return send;";
  s += "}";
  s += "</script>";
  
  s += "<br>";
  s += "<br>";
  s += "<input type=\"button\" onclick=\"window.location.href=sendRequest();\" value = \"Launch\"/>";
  s += "</form>";
  s += "</body>";

  s += "</html>\n";
  
  String request = client.readStringUntil('\r');
  client.flush();

  // sends 
  /*
  if(request.indexOf("/red/red") != -1) {
    Serial.print("1");
  }
  if(request.indexOf("/red/blue") != -1) {
    Serial.print("2");
  }
  if(request.indexOf("/red/green") != -1) {
    Serial.print("3");
  }
  if(request.indexOf("/green/green") != -1) {
    Serial.print("4");
  }
  if(request.indexOf("/green/red") != -1) {
    Serial.print("5");
  }
  if(request.indexOf("/green/blue") != -1) {
    Serial.print("6");
  }
  if(request.indexOf("/blue/blue") != -1) {
    Serial.print("7");
  }
  if(request.indexOf("/blue/red") != -1) {
    Serial.print("8");
  }
  if(request.indexOf("/blue/green") != -1) {
    Serial.print("9");
  }
  */
  Serial.println(request);

  // A simple hash map for 3x3x3 choices
  int serialSend = 0;
  if(request.indexOf("/redstart") != -1) {
    serialSend += 1;
  }
  if(request.indexOf("/bluestart") != -1) {
    serialSend += 2;
  }
  if(request.indexOf("/greenstart") != -1) {
    serialSend += 3;
  }
  if(request.indexOf("/redend") != -1) {
    serialSend += 11;
  }
  if(request.indexOf("/blueend") != -1) {
    serialSend += 15;
  }
  if(request.indexOf("/greenend") != -1) {
    serialSend += 19;
  }
  if(request.indexOf("5sec") != -1) {
    serialSend += 20;
  }
  if(request.indexOf("10sec") != -1) {
    serialSend += 40;
  }
  if(request.indexOf("30sec") != -1) {
    serialSend += 60;
  }
  
  char serialSendChar = serialSend;
  Serial.println(serialSendChar);
  
  // Sends the HTML file made from string s to the client
  client.print(s);
  delay(10);
}

