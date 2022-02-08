#include <WebServer.h>
#include <WiFi.h>
#include <Preferences.h>

Preferences prefs;

const char *ssid = "esp";
const char *password = "12345678";
WebServer server(80);


byte buffer[512];

void setup()
{
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
  server.begin();

}

void loop()
{

  server.handleClient();
}



void setColor()
{
  String universe = server.arg("universe");
    for (int i = 0; i < universe.length(); i+=2){
    Serial.println(strtol(universe.substring(i,i+2).c_str(),NULL,16));
}
  
 // Serial.println(strtol(.c_str(),NULL,16));
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", "ok");
}
