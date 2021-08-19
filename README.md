# OVERENIE EČV s využitím mikrokontrolérov
* Systém je postavený na WiFi platformách ESP32, ESP8266 (Espressif Systems)
* **Len pre edukačné účely!**
* Overenie EČV - skrz HTTP POST požiadavku na stránku http://www.minv.sk/?odcudzene-mot-vozidla
* Výpis o možnom pátraní po vozidle spolu s informáciami o vozidle
* Informácie sú scrapované zo zdrojového HTML kódu webstránky
* Scraper hľadá informácie v tabuľke medzi párovými <td> </td> tagmi
* Možnosť overovať vozidlá, prípojné zariadenia, aj špeciálne značky (napr. kategória C a F)
* **Vstup EČV cez UART monitor (115200 baud/s)**
* Možnosť modifikácie zdrojového kódu pre mikrokontróler na zadanie VIN (iný argument requestu, viz. HTML form)
* Možnosť zadať viac EČV súčasne s oddeľovaním cez znak \n, za posledné EČV je znak vložený automaticky
# Príklad výstupu pre príklady UART:
![HAKA - Výstup UART](https://i.imgur.com/zoDQNhr.png)
