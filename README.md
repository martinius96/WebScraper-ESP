# OVERENIE EČV s využitím mikrokontrolérov
* Systém je postavený na WiFi platformách ESP32, ESP8266 (Espressif Systems)
* **Len pre edukačné účely! Zdroj dát o kradnutých vozidlách: https://www.hakasystem.eu/kradeze-automobilov**
* Overenie EČV sa realizuje skrz HTTPS POST požiadavku na stránku http://www.minv.sk/?odcudzene-mot-vozidla
* EČV je priradené ku parametru ec, ktorý očakáva backend (napr: ec=LL000ZZ)
#
![HTML kód - parameter pre EČV](https://i.imgur.com/XgJYPC8.png)
* Výpis o pátraní po vozidle spolu s informáciami o vozidle
* Informácie sú scrapované zo zdrojového HTML kódu webstránky, ktorá je na request odoslaná webserverom (server response)
* Scraper hľadá informácie v tabuľke medzi párovými <td> </td> tagmi, ak sa na danom riadku nachádza aj popis kategórie
* Vykoná sa orezanie informácie medzi > a < (text medzi tagmi --> OSOBNÉ VOZIDLO, RENAULT a ďalšie parametre...)
#
![Výstup HTML zdrojový kód](https://i.imgur.com/r6PuXMz.png)
* Scraper prechádza HTML kód webstránky riadok po riadku
* Možnosť overovať vozidlá, prípojné zariadenia, aj špeciálne značky (napr. kategória C a F)
* **Vstup EČV cez UART monitor (115200 baud/s)**
* Možnosť modifikácie zdrojového kódu pre mikrokontróler na zadanie VIN (potrebný kľúč v requeste vin)
* **Možnosť zadať viac EČV súčasne s oddeľovaním cez znak \n, za posledné EČV je znak vložený automaticky** (viz obrázok nižšie)
* Z hľadiska buffra UART zbernice možno hľadať naraz u ESP32 31 EČV, u ESP8266 15 EČV   
# Príklad výstupu pre príklady UART:
![Výstup UART - hľadané vozidlá](https://i.imgur.com/dHOXFNc.png)
