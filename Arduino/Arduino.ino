/*|-----------------------------------------------------------------|*/
/*|OVERENIE ŠPZ - Arduino (Ethernet Wiznet W5100 - W5500)           |*/
/*|Používať nastavenie Serial monitoru Nový riadok (NL - New Line)! |*/
/*|Author: martinius96                                              |*/
/*|Len pre edukačné účely!                                          |*/
/*|-----------------------------------------------------------------|*/

#include <SPI.h>
#include <Ethernet.h> //Ethernet Wiznet W5100 - zakomentovat ak sa nepouziva    
//#include <Ethernet2.h> //Ethernet Wiznet W5500 - zakomentovat ak sa nepouziva   
//#include <Ethernet3.h> //Ethernet WIZ850io / USR-ES1 (Wiznet W5500 V2) - zakomentovat ak sa nepouziva   

byte mac[] = { 0xAA, 0xBB, 0xCC, 0xAA, 0xBB, 0xCC };
char serverName[] = "www.minv.sk"; //doména Ministerstva vnútra SR
String my_datas; //Hľadané EČV --> Možno hľadať aj C značky
int serverPort = 80;
EthernetClient client;
String url = "/?odcudzene-mot-vozidla";
const char terminator1[2] = ">";
const char terminator2[2] = "<";
void setup() {
  Serial.begin(115200);
  Ethernet.begin(mac);
  Serial.println(F("IP adresa Ethernet shieldu:"));
  Serial.println(Ethernet.localIP());
  delay(2000);
  Serial.println("Ready");
  while (Serial.available() <= 0) {
    Serial.println(F(" ZADAJ EVC V TVARE: BAXXXZZ "));
    delay(1000);
  }
}

void send_datas() {
  String data = "ec=" + my_datas;
  if (client.connect(serverName, serverPort)) {
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)serverName);
    client.println("User-Agent: ArduinoEthernetWiznet");
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
  if (Serial.available() > 0) {
    my_datas = Serial.readStringUntil('\n');
    send_datas();
  }
}
