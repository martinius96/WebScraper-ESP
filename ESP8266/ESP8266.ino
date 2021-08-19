/*|------------------------------------------------------------|*/
/*|OVERENIE ŠPZ - ESP8266 (Generic / NodeMCU, Wemos D1 Mini)   |*/
/*|Používať nastavenie Serial monitoru bez návratového vozíka! |*/
/*|Author: martinius96                                         |*/
/*|Len pre edukačné účely                                      |*/
/*|------------------------------------------------------------|*/
#include <ESP8266WiFi.h>

const char * ssid = "MENO_WIFI";
const char * password = "HESLO_WIFI";

const char* host = "www.minv.sk";
String my_datas; //Hľadané EČV --> Možno hľadať aj C značky
const int serverPort = 80; //http port
WiFiClient client;
String url = "/?odcudzene-mot-vozidla";
const char terminator1[2] = ">";
const char terminator2[2] = "<";
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi uspesne pripojene");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ready");
  while (Serial.available() <= 0) {
    Serial.println(" ZADAJ EVC V TVARE: BAXXXZZ ");
    delay(1000);
  }
}
void send_datas() {
  String data = "ec=" + my_datas;
  if (client.connect(host, serverPort)) {
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)host);
    client.println("User-Agent: ESP32");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line.indexOf("Podmienke vyhovuje: 1 záznam") > 0) {
        Serial.println("Po vozidle s EČV " + my_datas + " je vyhlásené pátranie! Kontaktujte políciu na čísle 158!");
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
        Serial.print("Druh vozidla: ");  Serial.println(parameter);
      }
      if (line.indexOf("Značka: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print("Značka vozidla: ");  Serial.println(parameter);
      }
      if (line.indexOf("Obchodný názov: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print("Model vozidla: ");  Serial.println(parameter);
      }
      if (line.indexOf("Farba: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print("Farba vozidla: ");  Serial.println(parameter);
      }
      if (line.indexOf("VIN: &nbsp;") > 0) {
        line = client.readStringUntil('\n');
        char str[100];
        char* parameter;
        line.toCharArray(str, line.length());
        parameter = strtok(str, terminator1);
        //Serial.println(parameter);
        parameter = strtok(NULL, terminator2);
        Serial.print("VIN kód: ");  Serial.println(parameter);
      }
    }
    Serial.println();
  } else {
    Serial.println("Problem s pripojenim na stránku Ministerstva vnútra Slovenskej republiky!");
  }
  client.stop();
}
void loop() {
  if (Serial.available() > 0) { //ak sa v UART buffri objavia dáta
    my_datas = Serial.readStringUntil('\n'); //nacitaj ECV po \n (možnosť zadať viac sucasne napr: PP100AA\nPP101AA - za posledné EČV je vlozeny znak \n automaticky)
    send_datas();
  }
}
