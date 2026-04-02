#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* apSsid = "YOUR_AP_NAME";
const char* apPassword = "YOUR_AP_PASSWORD";

ESP8266WebServer server(80);
IPAddress staticIP(192, 168, 4, 1);

const int numRelays = 6;
int relayPins[] = {D1, D2, D3, D4, D5, D6};
bool relayStates[] = {LOW, LOW, LOW, LOW, LOW, LOW};

void handleRoot() {
  String html = "<html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:#0f172a;color:#fff;}";
  html += ".container{display:flex;flex-direction:column;align-items:center;justify-content:center;min-height:100vh;gap:12px;}";
  html += ".button{padding:20px 28px;font-size:18px;border:none;border-radius:12px;cursor:pointer;min-width:220px;}";
  html += ".button-on{background:#16a34a;color:white;}";
  html += ".button-off{background:#dc2626;color:white;}";
  html += "</style>";
  html += "<script>";
  html += "function send(path){var xhr=new XMLHttpRequest();xhr.open('GET',path,true);xhr.send();}";
  html += "function pressRelay(i){send('/press?relay='+i);}";
  html += "function releaseRelay(i){send('/release?relay='+i);}";
  html += "function toggleRelay(i){send('/toggle?relay='+i);location.reload();}";
  html += "</script>";
  html += "</head><body><div class='container'><h1>ESP8266 Relay Controller</h1>";

  for (int i = 0; i < numRelays; i++) {
    String label = (i == 0) ? "Momentary Relay 1" :
                   (i == 1) ? "Momentary Relay 2" :
                   "Toggle Relay " + String(i + 1);
    String stateClass = relayStates[i] ? "button-on" : "button-off";

    if (i < 2) {
      html += "<button class='button " + stateClass + "'";
      html += " onmousedown='pressRelay(" + String(i) + ")' onmouseup='releaseRelay(" + String(i) + ")'";
      html += " ontouchstart='pressRelay(" + String(i) + ")' ontouchend='releaseRelay(" + String(i) + ")'>";
      html += label + "</button>";
    } else {
      html += "<button class='button " + stateClass + "' onclick='toggleRelay(" + String(i) + ")'>";
      html += label + "</button>";
    }
  }

  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void handlePress() {
  if (server.hasArg("relay")) {
    int relayIndex = server.arg("relay").toInt();
    if (relayIndex >= 0 && relayIndex < 2) {
      relayStates[relayIndex] = HIGH;
      digitalWrite(relayPins[relayIndex], relayStates[relayIndex]);
    }
  }
  handleRoot();
}

void handleRelease() {
  if (server.hasArg("relay")) {
    int relayIndex = server.arg("relay").toInt();
    if (relayIndex >= 0 && relayIndex < 2) {
      relayStates[relayIndex] = LOW;
      digitalWrite(relayPins[relayIndex], relayStates[relayIndex]);
    }
  }
  handleRoot();
}

void handleToggle() {
  if (server.hasArg("relay")) {
    int relayIndex = server.arg("relay").toInt();
    if (relayIndex >= 2 && relayIndex < numRelays) {
      relayStates[relayIndex] = !relayStates[relayIndex];
      digitalWrite(relayPins[relayIndex], relayStates[relayIndex]);
    }
  }
  handleRoot();
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);
  WiFi.softAPConfig(staticIP, staticIP, IPAddress(255,255,255,0));
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], relayStates[i]);
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/press", HTTP_GET, handlePress);
  server.on("/release", HTTP_GET, handleRelease);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();
}
