/*|-----------------------------------------------------------------|*/
/*|OVERENIE ŠPZ - ESP8266 (Generic / NodeMCU, Wemos D1 Mini)        |*/
/*|Používať nastavenie Serial monitoru Nový riadok (NL - New Line)! |*/
/*|Author: martinius96                                              |*/
/*|Len pre edukačné účely!                                          |*/
/*|-----------------------------------------------------------------|*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
const char * ssid = "MENO_WIFI";
const char * password = "HESLO_WIFI";

const char* host = "www.minv.sk";
String my_datas; //Hľadané EČV --> Možno hľadať aj C značky
const int serverPort = 443; //https port
WiFiClientSecure client;
String url = "/?odcudzene-mot-vozidla";
const char terminator1[2] = ">";
const char terminator2[2] = "<";
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));
  Serial.println(F("WiFi uspesne pripojene"));
  Serial.println(F("IP adresa: "));
  Serial.println(WiFi.localIP());
  Serial.println(F("Ready"));
  client.setInsecure();
  while (Serial.available() <= 0) {
    Serial.println(F(" ZADAJ EVC V TVARE: BAXXXZZ "));
    delay(1000);
  }
}
void send_datas() {
  String data = "ecv=" + my_datas;
  if (client.connect(host, serverPort)) {
    //  Serial.println("Pripojenie uspesne");
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)host);
    client.println(F("User-Agent: ESP32"));
    client.println(F("Connection: close"));
    client.println(F("Content-Type: application/x-www-form-urlencoded;"));
    client.print(F("Content-Length: "));
    client.println(data.length());
    client.println();
    client.println(data);
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      // Serial.println(line);
      if (line.indexOf("Podmienke vyhovuje: 1 záznam") > 0) {
        Serial.println("Po vozidle s EČV " + my_datas + " je vyhlásené pátranie!");
      }
      if (line.indexOf("Podmienke vyhovuje: 0 záznamov") > 0) {
        Serial.println("Po vozidle s EČV " + my_datas + " nie je vyhlásené pátranie");
      }
      if (line.indexOf("Druh vozidla: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print(F("Druh vozidla: "));  Serial.println(parameter);
      }
      if (line.indexOf("Značka: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print(F("Značka vozidla: "));  Serial.println(parameter);
      }
      if (line.indexOf("Obchodný názov: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print(F("Model vozidla: "));  Serial.println(parameter);
      }
      if (line.indexOf("Farba: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print(F("Farba vozidla: "));  Serial.println(parameter);
      }
      if (line.indexOf("VIN: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print(F("VIN kód: "));  Serial.println(parameter);
      }
    }
    Serial.println();
  } else {
    Serial.println(F("Problem s pripojenim na stránku Ministerstva vnútra Slovenskej republiky!"));
  }
  client.stop();
}
void loop() {
  if (Serial.available() > 0) { //ak sa v UART buffri objavia dáta
    my_datas = Serial.readStringUntil('\n'); //nacitaj ECV po \n (možnosť zadať viac sucasne napr: PP100AA\nPP101AA - za posledné EČV je vlozeny znak \n automaticky)
    send_datas();
  }
}
