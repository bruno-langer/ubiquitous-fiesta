#include <WebServer.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPDMX.h>

Preferences prefs;

const char *ssid = "esp";
const char *password = "12345678";
WebServer server(80);

#define UNIVERSE_LENGTH 512

DMXESPSerial dmx;

void setup()
{
  dmx.init();
  prefs.begin("dados", false);
  prefs.end();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  server.on("/setColor", setColor);
  server.on("/data", readData);
  server.begin();

}

void loop()
{
  dmx.update();
  server.handleClient();
}



void setColor()
{
  String universe = server.arg("universe");
  for (int i = 1; i <= universe.length() + 1; i += 2) {
    dmx.write(i / 2, strtol(universe.substring(i, i + 2).c_str(), NULL, 16));
  }

  // Serial.println(strtol(.c_str(),NULL,16));
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", "ok");
}

void readData()
{

  String sentData = "";
  for (int i = 1; i <= UNIVERSE_LENGTH + 1; i++) {
    const uint8_t channelData = dmx.read(i);
    if(channelData < 10){
      sentData.concat("0" + String(channelData, HEX));
      }
      else{
        sentData.concat(String(channelData, HEX));
      }
    
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", sentData);
}
