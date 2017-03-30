#include <ESP8266WiFi.h>

// Wifi networks name and password
char ssid[] = "WATwork";
char pass[] = "ece492w2017";

// button dimensions to fit a 720p phone screen
char buttonfullwidth[] = "345";
char buttonsmallwidth[] = "57";
char buttonheight[] = "36";

char redcode = 0;
char greencode = 1;
char bluecode = 2;

// character to send to the DE0
char tosend;

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

  // starts the WiFi network
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

  String request = client.readStringUntil('\r');
  client.flush();
  
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Connect-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  // set view appropriate for phone
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

  s += "<body>";

  // RC MODE
  if(request.indexOf("/RC") != -1) {   
    s += "<h1> WAT RC Mode </h1>";
    s += "<input type=\"button\" onclick=\"location.href='/autonomous';\" ";
    s += "value = \"Switch to Autonomous mode\" style=\"height:36px; width:200px\"/>";
    s += "<br>";
    s += "<br>";
    
    // Forward buttons
    s += "<input type=\"button\" onclick=\"location.href='/RC/forward/i3';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonfullwidth; 
    s += "px\" value=\"FORWARD FOREVER\"/>";
    s += "<br>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/forward/i2';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonfullwidth; 
    s += "px\" value=\"FORWARD MEDIUM\"/>";
    s += "<br>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/forward/i1';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonfullwidth; 
    s += "px\" value=\"FORWARD SHORT\"/>";
    s += "<br>";
  
    s += "<br>";
  
    // Left buttons
    s += "<input type=\"button\" onclick=\"location.href='/RC/left/i3';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonsmallwidth; 
    s += "px\" value=\"<<<\"/>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/left/i2';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonsmallwidth; 
    s += "px\" value=\"<<\"/>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/left/i1';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonsmallwidth; 
    s += "px\" value=\"<\"/>";
  
    // Right buttons
    s += "<input type=\"button\" onclick=\"location.href='/RC/right/i1';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonsmallwidth; 
    s += "px\" value=\">\"/>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/right/i2';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonsmallwidth; 
    s += "px\" value=\">>\"/>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/right/i3';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonsmallwidth; 
    s += "px\" value=\">>>\"/>";
    s += "<br>";
  
    s += "<br>";
  
    // backwards buttons
    s += "<input type=\"button\" onclick=\"location.href='/RC/backward/i1';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonfullwidth; 
    s += "px\" value=\"BACKWARD SHORT\"/>";
    s += "<br>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/backward/i2';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonfullwidth; 
    s += "px\" value=\"BACKWARD MEDIUM\"/>";
    s += "<br>";
  
    s += "<input type=\"button\" onclick=\"location.href='/RC/backward/i3';\" ";
    s += "style=\"height:";
    s += buttonheight;
    s += "px;width:";
    s += buttonfullwidth; 
    s += "px\" value=\"BACKWARD FOREVER\"/>";
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
  }

  // Autonomous Mode
  else {
    s += "<h1> WAT Control Interface </h1>";
    s += "<input type=\"button\" onclick=\"location.href='/RC';\" value = \"Switch to RC mode\"";
    s += "style=\"height:36px; width:200px; vertical-align:middle\"/>";
    s += "<H2> Autonomous Mode </H2>";
    s += "<form>";
    
    s += "First Stop: <br>";
    s += "<select id = \"firststop\">";
    s += "<option value = \"/redstart\"> RED </option>";
    s += "<option value = \"/bluestart\"> BLUE </option>";
    s += "<option value = \"/greenstart\"> GREEN </option>";
    s += "</select>";  
    s += "<br>";
    s += "<br>";
  
    s += "Second Stop: <br>";
    s += "<select id = \"secondstop\">";
    s += "<option value = \"/redmid\"> RED </option>";
    s += "<option value = \"/bluemid\"> BLUE </option>";
    s += "<option value = \"/greenmid\"> GREEN </option>";
    s += "</select>";
    s += "<br>";
    s += "<br>";
    
    s += "Final Destination: <br>";
    s += "<select id = \"destination\">";
    s += "<option value = \"/redend\"> RED </option>";
    s += "<option value = \"/blueend\"> BLUE </option>";
    s += "<option value = \"/greenend\"> GREEN </option>";
    s += "</select>";
    s += "<br>";
    s += "<br>";
  
    s += "Time stopped at each stop (seconds): <br>";
    s += "<select id = \"time\">";
    s += "<option value = \"/5sec\"> 5 </option>";
    s += "<option value = \"/10sec\"> 10 </option>";
    s += "<option value = \"/30sec\"> 30 </option>";
    s += "</select>";
  
    s += "<script>";
    s += "function sendRequest() {";
    s += "var ar = document.getElementById(\"firststop\");";
    s += "var at = ar.options[ar.selectedIndex].value;";
    s += "var dr = document.getElementById(\"secondstop\");";
    s += "var dt = dr.options[dr.selectedIndex].value;";
    s += "var br = document.getElementById(\"destination\");";
    s += "var bt = br.options[br.selectedIndex].value;";
    s += "var cr = document.getElementById(\"time\");";
    s += "var ct = cr.options[cr.selectedIndex].value;";
    s += "var send = \"/autonomous\" + at + dt + bt + ct;";
    s += "return send;";
    s += "}";
    s += "</script>";
    
    s += "<br>";
    s += "<br>";
    s += "<input type=\"button\" onclick=\"window.location.href=sendRequest();\" ";
    s += "value = \"Launch\" style=\"height:80px; width:200px\" />";
    
    s += "</form>";
  }
  
  s += "</body>";
  s += "</html>\n";


  // Character code to send to the DE0
  // The composition of the character code is detailed in the included report
  tosend = 0;
  
  // RC Mode
  if(request.indexOf("/RC") != -1) {
    // RC indicator = 0b 1100 0000
    tosend = tosend | 192;
  }
  // stop indicator = 0b 11XX X000
  if(request.indexOf("/stop") != -1) {
    // keep tosend at 0
  }
  // forward indicator = 0b 11XX X001
  if(request.indexOf("/forward") != -1) {
    tosend = tosend | 1;
  }
  // reverse indicator = 0b 11XX X010
  if(request.indexOf("/backward") != -1) {
    tosend = tosend | 2;
  }
  // left indicator = 0b 11XX X011
  if(request.indexOf("/left") != -1) {
    tosend = tosend | 3;
  }
  // right indicator = 0b 11XX X100
  if(request.indexOf("/right") != -1) {
    tosend = tosend | 4;
  } 
  // increment 1 indicator = 0b 1100 1XXX
  if(request.indexOf("/i1") != -1) {
    tosend = tosend | 8;
  }
  // increment 2 indicator = 0b 1101 0XXX
  if(request.indexOf("/i2") != -1) {
    tosend = tosend | 16;
  }
  // increment 3 indicator = 0b 1101 1XXX
  if(request.indexOf("/i3") != -1) {
    tosend = tosend | 24;
  }
  
  // Autonomous Mode
  // 5 second indicator = 0b XXXX not(11)XX XX01
  if(request.indexOf("/5sec") != -1) {
    tosend = tosend | 1;
  }
  // 10 second indicator = 0b XXXX not(11)XX XX10
  if(request.indexOf("/10sec") != -1) {
    tosend = tosend | 2;
  }
  // 15 second indicator = 0b XXXX not(11)XX XX11
  if(request.indexOf("/30sec") != -1) {
    tosend = tosend | 3;
  }
  // redstart = 0b not(11)XX XX00
  if(request.indexOf("/redstart") != -1) {
    tosend = tosend | (redcode << 2);
  }
  // greenstart = 0b not(11)XX XX01
  if(request.indexOf("/greenstart") != -1) {
    tosend = tosend | (greencode << 2);
  }
  // bluestart = 0b not(11)XX XX10
  if(request.indexOf("/bluestart") != -1) {
    tosend = tosend | (bluecode << 2);
  }
  // redmid = 0b not(11)XX 00XX
  if(request.indexOf("/redmid") != -1) {
    tosend = tosend | (redcode << 4);
  }
  // greenmid = 0b not(11)XX 01XX
  if(request.indexOf("/greenmid") != -1) {
    tosend = tosend | (greencode << 4);
  }
  // bluemid = 0b not(11)XX 10XX
  if(request.indexOf("/bluemid") != -1) {
    tosend = tosend | (bluecode << 4);
  }
  // redend = 0b not(11)00 XXXX
  if(request.indexOf("/redend") != -1) {
    tosend = tosend | (redcode << 6);
  }
  // greenend = 0b not(11)01 XXXX
  if(request.indexOf("/greenend") != -1) {
    tosend = tosend | (greencode << 6);
  }
  // blueend = 0b not(11)10 XXXX
  if(request.indexOf("/blueend") != -1) {
    tosend = tosend | (bluecode << 6);
  }
  if(tosend == 0) {
    if(request.indexOf("/autonomous") != -1) {
      // trigger stop if switching to autonomous mode from RC mode
      tosend = 192;
    }
  }
  
  if(tosend != 0) {
    Serial.print(tosend);
  }
  //Serial.println("new line every loop");
  
  // Sends the HTML file made from string s to the client
  client.print(s);
  delay(100);
}

