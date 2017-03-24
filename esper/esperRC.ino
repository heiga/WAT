#include <ESP8266WiFi.h>

// Wifi hotspot name and password
char ssid[] = "ESP8266_NETWORK";
char pass[] = "genericpassword";

// button dimensions to fit a 720p phone screen
char buttonfullwidth[] = "345";
char buttonsmallwidth[] = "57";
char buttonheight[] = "36";

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
  s += "<input type=\"button\" onclick=\"location.href='Automata';\" ";
  s += "value = \"Switch to Autonomous mode\" style=\"height:36px; width:200px\"/>";
  s += "<br>";
  s += "<br>";
  
  // Inputs that use href to send a request

  // Forward buttons
  s += "<input type=\"button\" onclick=\"location.href='/forward/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Forward forever\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/forward/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Forward 1m\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/forward/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Forward 25cm\"/>";
  s += "<br>";

  s += "<br>";

  // Left buttons
  s += "<input type=\"button\" onclick=\"location.href='/left/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\"<<<\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/left/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\"<<\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/left/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\"<\"/>";

  // Right buttons
  s += "<input type=\"button\" onclick=\"location.href='/right/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\">\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/right/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\">>\"/>";

  s += "<input type=\"button\" onclick=\"location.href='/right/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonsmallwidth; 
  s += "px\" value=\">>>\"/>";
  s += "<br>";

  s += "<br>";

  // backwards buttons
  s += "<input type=\"button\" onclick=\"location.href='/backward/1';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Backward 25cm\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/backward/2';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Backward 1m\"/>";
  s += "<br>";

  s += "<input type=\"button\" onclick=\"location.href='/backward/3';\" ";
  s += "style=\"height:";
  s += buttonheight;
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"Backward forever\"/>";
  s += "<br>";

  s += "<br>";

  // stop button
  s += "<input type=\"button\" onclick=\"location.href='/stop';\" ";
  s += "style=\"height:";
  s += "80";
  s += "px;width:";
  s += buttonfullwidth; 
  s += "px\" value=\"STOP\"/>";
  s += "<br>";


  /*
  s += "<input type=\"button\" onclick=\"location.href='/status=ccw';\" ";
  s += "style=\"height:80px;width:200px\" value=\"COUNTER-CLOCKWISE\" />";
  s += "<br>";
  s += "<br>";
  s += "<input type=\"button\" onclick=\"location.href='/status=stop';\" ";
  s += "style=\"height:80px;width:200px\" value=\"STOP\"/>";
  */
  s += "</html>\n";

  // Read request from the user
  String request = client.readStringUntil('\r');
  client.flush();

  // Get "status" from the client 
  // and send a corresponding code via serial to the DE2/DE0
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
}
