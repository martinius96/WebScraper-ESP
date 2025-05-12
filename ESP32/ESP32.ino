/*|-----------------------------------------------------------------|*/
/*|OVERENIE ŠPZ - ESP32 (DevKit)                                    |*/
/*|Používať nastavenie Serial monitoru Nový riadok (NL - New Line)! |*/
/*|Author: martinius96                                              |*/
/*|Len pre edukačné účely!                                          |*/
/*|-----------------------------------------------------------------|*/

//Mozno vyskusat v on-line simulátore Wokwi: https://wokwi.com/projects/344494550030484052

#include <WiFi.h>
#include <WiFiClientSecure.h>
const char * ssid = "MENO_WIFI";
const char * password = "HESLO_WIFI";

const char* host = "www.minv.sk";
String my_datas; //Hľadané EČV --> Možno hľadať aj C značky
const int serverPort = 443; //http port
WiFiClientSecure client;
String url = "/?odcudzene-mot-vozidla";
const char terminator1[2] = ">";
const char terminator2[2] = "<";

const static char* test_root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIGAjCCA+qgAwIBAgIRCBeSUjZo9chQAAAAAAAAAAMwDQYJKoZIhvcNAQELBQAw
UjELMAkGA1UEBhMCU0sxEzARBgNVBAcTCkJyYXRpc2xhdmExEzARBgNVBAoTCkRp
c2lnIGEucy4xGTAXBgNVBAMTEENBIERpc2lnIFJvb3QgUjIwHhcNMTQxMDAyMTIx
OTA2WhcNMjkwOTI4MTIxOTA2WjBlMQswCQYDVQQGEwJTSzETMBEGA1UEBwwKQnJh
dGlzbGF2YTETMBEGA1UECgwKRGlzaWcgYS5zLjEsMCoGA1UEAwwjQ0EgRGlzaWcg
UjJJMiBDZXJ0aWZpY2F0aW9uIFNlcnZpY2UwggEiMA0GCSqGSIb3DQEBAQUAA4IB
DwAwggEKAoIBAQCyUskVGBrkooW8zWUYsR8WKNA5baWkD6PKuMXL3mi9lt/u0W1J
5b2FIjtWJt6PBZlMGcnzCQSEC8Ao/9HZQQwwAttLgmcdBSwyIenDMuxiM1WbnZ+5
kmUnKjHs+LaROq3vBCIzdGDQGoJACEFsJuFpDGQXrVDtlzhhRlmM0bsyJRJwp6iL
31OCnGBz/sj5/rugnWFB9XNwUSFcafzTLwlo8OI/gIhhXQSdk8WjrFvQdG1TiqkI
ojm9Mz5a6pzfN9NncZbGV4Xa1cV/oMG4fZAqu7+X7tlVPPDF3/7UKzSRGydrShWc
4tPOORPevcT6/XQnpqGrB89sxvtkm2lCsvrDAgMBAAGjggG+MIIBujCBgwYIKwYB
BQUHAQEEdzB1MDcGCCsGAQUFBzABhitodHRwOi8vcm9vdGNhcjItb2NzcC5kaXNp
Zy5zay9vY3NwL3Jvb3RjYXIyMDoGCCsGAQUFBzAChi5odHRwOi8vd3d3LmRpc2ln
LnNrL3Jvb3RjYXIyL2NlcnQvcm9vdGNhcjIuZGVyMB0GA1UdDgQWBBQkpbw6KGIV
QjL5MFrJ8T7koQmosTAfBgNVHSMEGDAWgBS1mfivsJT14yDWCq3OTlakLm5C7TAS
BgNVHRMBAf8ECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBBjB1BgNVHR8EbjBsMDSg
MqAwhi5odHRwOi8vY2RwMS5kaXNpZy5zay9yb290Y2FyMi9jcmwvcm9vdGNhcjIu
Y3JsMDSgMqAwhi5odHRwOi8vY2RwMi5kaXNpZy5zay9yb290Y2FyMi9jcmwvcm9v
dGNhcjIuY3JsMDkGA1UdIAQyMDAwLgYEVR0gADAmMCQGCCsGAQUFBwIBFhhodHRw
Oi8vd3d3LmRpc2lnLnNrL2Nwcy8wHAYDVR0RBBUwE4ERc3ByYXZhY2FAZGlzaWcu
c2swDQYJKoZIhvcNAQELBQADggIBAEoFQYsKbtRjEdbWeqZHhJF8cRcg66HUY0zD
OksWZpGzTOL7K7PGaJVRrMxIiIiG3hFU/CYWQlVk3NjiUo1T3m02TgkWfcXsyEZ2
MVXpImSl5fgsDfI/5/INCGh39YkN5Y91yE0bDrJUa/uTadpS3fd4vKzmhy5qRsl2
QFt29QDu7p/NUi5rojns1fCqzPserZyvciAG/hYS8t4C3GIOK2rwn09nZK+du5Gq
uSX89KaVKM6UySqT+jIXzyhTwnXbEabFGSMVlmW/fKlGGLhET6jYPUnGje/b0qPv
GmuvYpNC7ztCHcnNdfGLpB4L6Puxtn5xcYTswh1D8raQvArDFoqPwOQr2XggqU+t
0ktu6IKIfInhB4WjGgAMr3aGyoNqKSv9Qzbh8vkdIe7+lwIBievK/+04q+Fsi5JB
zTAUumLQZhS9eXd+065GF/0O90D5fS0Y5Wc7naMQ7/o9T+9hIxC4BIm+2g0KtL43
SPF+jdqrLjUfe7/mWyZIvEKqlk6X6CUjRx9fVc01uO5gUyLHMSoWAtIyGBtmDUVC
7exu4q8BHUDMQdrZyhJOJ9TbN2Xi+z8BhpqosGPe7UTtRB3/UmMsbEgbfIIb5mMw
nx6TtnF5r+FLiew8BeQcD7+7768rw0+LaddwiKlmGBjYvNy+e1cSlJl1ZBXtHzlj
57JOD9u0
-----END CERTIFICATE-----
)EOF";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.setCACert(test_root_ca);
  Serial.println("");
  Serial.println("WiFi uspesne pripojene");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ready");
  while (Serial.available() <= 0) {
    Serial.println(F(" ZADAJ EVC V TVARE: BAXXXZZ "));
    delay(1000);
  }
}
void send_datas() {
  String data = "ec=" + my_datas;
  if (client.connect(host, serverPort)) {
    Serial.println(F("Pripojenie na stranku MinvSR uspesne"));
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
      if (line.indexOf("Podmienke vyhovuje: 1 záznam") > 0) {
        Serial.println("Po vozidle s EČV " + my_datas + " je vyhlásené pátranie!");
      }
      if (line.indexOf("Podmienke vyhovuje: 0 záznamov") > 0) {
        Serial.println("Po vozidle s EČV " + my_datas + " nie je vyhlásené pátranie");
      } if (line.indexOf("Druh vozidla: &nbsp;") > 0) {
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
