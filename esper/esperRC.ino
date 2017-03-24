#include <ESP8266WiFi.h>

// Wifi hotspot name and password
char ssid[] = "ESP8266_NETWORK";
char pass[] = "genericpassword";

// button dimensions to fit a 720p phone screen
char buttonfullwidth[] = "345";
char buttonsmallwidth[] = "57";
char buttonheight[] = "36";

char tosend = 0;

WiFiServer server(80);

void createWifi(char* ssid, char* pass) {
  //Serial.println("Starting Network");
  //Serial.println(ssid);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  // This function creates the wifi network
  WiFi.softAP(ssid, pass);
  //Serial.println("Success!");
}

void setup() {
  // Set the serial to communicate at the same baud rate 
  // as your DE0/DE2
  Serial.begin(57600);
  createWifi(ssid, pass);
  server.begin();
  // Server stuff from
  // http://www.esp8266.com/viewtopic.php?f=32&t=8329&start=4
  delay(500);
  
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    //Serial.println("Something wrong with client");
    return;
  }

  // Make an HTML to send to client
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Connect-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  s += "<h1> WAT RC Mode </h1>";
  s += "<input type=\"button\" onclick=\"location.href='/';\" ";
  s += "value = \"Switch to Autonomous mode\" style=\"height:36px; width:200px\"/>";
  s += "<br>";
  s += "<br>";
  
  // Inputs that use href to send a request

  // Forward buttons
  s += "<input type=\"button\" onclick=\"location.href='/RC/forward/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Forward forever\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/forward/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Forward 1m\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/forward/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Forward 25cm\"/>";
  s += "<br>";

  s += "<br>";

  // Left buttons
  s += "<input type=\"button\" onclick=\"location.href='/RC/left/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\"<<<\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/left/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\"<<\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/left/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\"<\"/>";

  // Right buttons
  s += "<input type=\"button\" onclick=\"location.href='/RC/right/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\">\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/right/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\">>\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/right/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\">>>\"/>";
  s += "<br>";

  s += "<br>";

  // backwards buttons
  s += "<input type=\"button\" onclick=\"location.href='/RC/backward/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Backward 25cm\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/backward/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Backward 1m\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/RC/backward/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Backward forever\"/>";
  s += "<br>";

  s += "<br>";

  // stop button
  s += "<input type=\"button\" onclick=\"location.href='/RC/stop';\" ";
  s += "style=\"height:";
  s += "80";
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"STOP\"/>";
  s += "<br>";

  s += "</html>\n";

  // Read request from the user
  String request = client.readStringUntil('\r');
  client.flush();

 
  // Get "status" from the client 
  // and send a corresponding code via serial to the DE2/DE0

  tosend = 0;
  
  // RC indicator = 0b 1100 0000
  tosend = tosend | 192;

  // stop indicator = 0b XXXX X000
  if(request.indexOf("/stop") != -1) {
    // do nothing, keep it at 0
  }
  // forward indicator = 0b XXXX X001
  if(request.indexOf("/forward") != -1) {
    tosend = tosend | 1;
  }
  // reverse indicator = 0b XXXX X010
  if(request.indexOf("/backward") != -1) {
    tosend = tosend | 2;
  }
  // left indicator = 0b XXXX X011
  if(request.indexOf("/left") != -1) {
    tosend = tosend | 3;
  }
  // right indicator = 0b XXXX X100
  if(request.indexOf("/right") != -1) {
    tosend = tosend | 4;
  }
  
  // increment 1 indicator = 0b XX00 1XXX
  if(request.indexOf("/1") != -1) {
    tosend = tosend | 8;
  }
  // increment 2 indicator = 0b XX01 0XXX
  if(request.indexOf("/2") != -1) {
    tosend = tosend | 16;
  }
  // increment 3 indicator = 0b XX01 1XXX
  if(request.indexOf("/3") != -1) {
    tosend = tosend | 24;
  }

  Serial.print(tosend);
  
  client.print(s);
  delay(10);
}
