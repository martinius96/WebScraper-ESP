//Web scraper sa autorizuje cez HTTP Auth Basic ako normálny user
//Vie vyparsovať dáta z tabuľky HTML kódu webrozhrania: https://i.imgur.com/9P2Emih.png
//Použité pre rozhranie pre tepelné čerpadlo AirMaster od MasterThermu

#include <WiFi.h>
const char* ssid = "NAZOV_WIFI"; //SSID vasej WiFi siete
const char* password = "HESLO_WIFI"; //HESLO vasej WiFi siete
const char* host = "10.0.0.50"; //IP adresa / domena webservera
String auth_data = "bWFzdGVydGhlcm06YWJjZA=="; //base64 pre: mastertherm:abcd --> vypocet na https://www.base64encode.org/
String url = "/http/index.html"; // cesta za hostom (10.X.X.X). Napr. /mastertherm/abcd/index.html

WiFiClient client;
unsigned long timer = 0;
const char terminator_start[] = ">";
const char terminator_end[] = "<";
void setup() {
  Serial.begin(115200); //rychlost seriovej linky
  Serial.println();
  Serial.print("Pripajanie na WiFi siet: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //pripoj sa na wifi siet s heslom
  while (WiFi.status() != WL_CONNECTED) { //pokial sa nepripojime na wifi opakuj pripajanie a spustaj funkcie pre ovladanie v offline rezime
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi pripojene s IP:");
  Serial.println(WiFi.localIP());

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if ((millis() - timer) >= 60000 || timer == 0) {
    timer = millis();
    client.stop();
    if (client.connect(host, 80)) {
      Serial.println("Pripojenie uspesne, nacitavam data MasterThermu");
      client.print(String("GET ") + url + " HTTP/1.0\r\n" + "Host: " + host + "\r\n" + "Authorization: Basic " + auth_data + "\r\n" + "User-Agent: ESP32\r\n" + "Connection: close\r\n\r\n");
      while (1) {
        String line = client.readStringUntil('\n'); //HTTP HEADER
        //  Serial.println(line);

        if (line.indexOf("<td class=\"value\">") > 0) {
          char str[100];
          char* parameter;

          line.toCharArray(str, line.length());

          parameter = strtok(str, terminator_start);
          parameter = strtok(NULL, terminator_end);

          if (parameter != NULL) {
            Serial.print("Parsed number: ");
            Serial.println(parameter);
          }
        }
        if (line.indexOf("<p>Carel S.p.A., Brugine (PD) - Italy. All rights reserved.</p>") > 0) { //END CONNECTION
          Serial.println("END TERMINAL");
          client.stop();
          break;
        }
      }
    } else if (!client.connect(host, 80)) {
      Serial.println("Pripojenie zlyhalo");
    }
  }
}
