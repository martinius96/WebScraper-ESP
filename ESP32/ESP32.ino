/*|------------------------------------------------------------|*/
/*|OVERENIE ŠPZ - ESP32 (DevKit)                               |*/
/*|Používať nastavenie Serial monitoru bez návratového vozíka! |*/
/*|Author: martinius96                                         |*/
/*|Len pre edukačné účely                                      |*/
/*|------------------------------------------------------------|*/
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

const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n" \
                           "MIIFaTCCA1GgAwIBAgIJAJK4iNuwisFjMA0GCSqGSIb3DQEBCwUAMFIxCzAJBgNV\n" \
                           "BAYTAlNLMRMwEQYDVQQHEwpCcmF0aXNsYXZhMRMwEQYDVQQKEwpEaXNpZyBhLnMu\n" \
                           "MRkwFwYDVQQDExBDQSBEaXNpZyBSb290IFIyMB4XDTEyMDcxOTA5MTUzMFoXDTQy\n" \
                           "MDcxOTA5MTUzMFowUjELMAkGA1UEBhMCU0sxEzARBgNVBAcTCkJyYXRpc2xhdmEx\n" \
                           "EzARBgNVBAoTCkRpc2lnIGEucy4xGTAXBgNVBAMTEENBIERpc2lnIFJvb3QgUjIw\n" \
                           "ggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQCio8QACdaFXS1tFPbCw3Oe\n" \
                           "NcJxVX6B+6tGUODBfEl45qt5WDza/3wcn9iXAng+a0EE6UG9vgMsRfYvZNSrXaNH\n" \
                           "PWSb6WiaxswbP7q+sos0Ai6YVRn8jG+qX9pMzk0DIaPY0jSTVpbLTAwAFjxfGs3I\n" \
                           "x2ymrdMxp7zo5eFm1tL7A7RBZckQrg4FY8aAamkw/dLukO8NJ9+flXP04SXabBbe\n" \
                           "QTg06ov80egEFGEtQX6sx3dOy1FU+16SGBsEWmjGycT6txOgmLcRK7fWV8x8nhfR\n" \
                           "yyX+hk4kLlYMeE2eARKmK6cBZW58Yh2EhN/qwGu1pSqVg8NTEQxzHQuyRpDRQjrO\n" \
                           "QG6Vrf/GlK1ul4SOfW+eioANSW1z4nuSHsPzwfPrLgVv2RvPN3YEyLRa5Beny912\n" \
                           "H9AZdugsBbPWnDTYltxhh5EF5EQIM8HauQhl1K6yNg3ruji6DOWbnuuNZt2Zz9aJ\n" \
                           "QfYEkoopKW1rOhzndX0CcQ7zwOe9yxndnWCywmZgtrEE7snmhrmaZkCo5xHtgUUD\n" \
                           "i/ZnWejBBhG93c+AAk9lQHhcR1DIm+YfgXvkRKhbhZri3lrVx/k6RGZL5DJUfORs\n" \
                           "nLMOPReisjQS1n6yqEm70XooQL6iFh/f5DcfEXP7kAplQ6INfPgGAVUzfbANuPT1\n" \
                           "rqVCV3w2EYx7XsQDnYx5nQIDAQABo0IwQDAPBgNVHRMBAf8EBTADAQH/MA4GA1Ud\n" \
                           "DwEB/wQEAwIBBjAdBgNVHQ4EFgQUtZn4r7CU9eMg1gqtzk5WpC5uQu0wDQYJKoZI\n" \
                           "hvcNAQELBQADggIBACYGXnDnZTPIgm7ZnBc6G3pmsgH2eDtpXi/q/075KMOYKmFM\n" \
                           "tCQSin1tERT3nLXK5ryeJ45MGcipvXrA1zYObYVybqjGom32+nNjf7xueQgcnYqf\n" \
                           "GopTpti72TVVsRHFqQOzVju5hJMiXn7B9hJSi+osZ7z+Nkz1uM/Rs0mSO9MpDpkb\n" \
                           "lvdhuDvEK7Z4bLQjb/D907JedR+Zlais9trhxTF7+9FGs9K8Z7RiVLoJ92Owk6Ka\n" \
                           "+elSLotgEqv89WBW7xBci8QaQtyDW2QOy7W81k/BfDxujRNt+3vrMNDcTa/F1bal\n" \
                           "TFtxyegxvug4BkihGuLq0t4SOVga/4AOgnXmt8kHbA7v/zjxmHHEt38OFdAlab0i\n" \
                           "nSvtBfZGR6ztwPDUO+Ls7pZbkBNOHlY667DvlruWIxG68kOGdGSVyCh13x01utI3\n" \
                           "gzhTODY7z2zp+WsO0PsE6E9312UBeIYMej4hYvF/Y3EMyZ9E26gnonW+boE+18Dr\n" \
                           "G5gPcFw0sorMwIUY6256s/daoQe/qUKS82Ail+QUoQebTnbAjn39pCXHR+3/H3Os\n" \
                           "zMOl6W8KjptlwlCFtaOgUxLMVYdh84GuEEZhvUQhuMI9dM9+JDX6HAcOmz0iyu8x\n" \
                           "L4ysEr3vQCj8KWefshNPZiTEUxnpHikV7+ZtsH8tZ/3zbBt1RqPlShfppNcL\n" \
                           "-----END CERTIFICATE-----\n";


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
