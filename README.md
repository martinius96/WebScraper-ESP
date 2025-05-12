# Overenie EČV s využitím mikrokontrolérov
* **v roku 2025 nie sú zdrojové kódy kompatibilné s MINV stránkou. Zmenil sa parameter v HTML formulári a tiež riadny výpisu sú iné, než na aké reagoval pôvodný parser**
#
* Systém je postavený na WiFi platformách ESP32, ESP8266 (Espressif Systems)
* **Len pre edukačné účely! Zdroj dát o kradnutých vozidlách: https://www.hakasystem.eu/kradeze-automobilov**
* Overenie EČV sa realizuje skrz POST request na stránku https://www.minv.sk/?odcudzene-mot-vozidla
* EČV je priradené ku parametru ec, ktorý očakáva backend (napr: ec=LL000ZZ)

<p align="center">
  <img src="https://i.imgur.com/XgJYPC8.png" />
</p>

* Výpis o pátraní po vozidle spolu s informáciami o vozidle sú dostupné v HTML kóde
* Informácie sú scrapované zo zdrojového HTML kódu webstránky, ktorá je na request odoslaná webserverom (server response)
* Scraper hľadá informácie v tabuľke medzi párovými &lt;TD&gt; &lt;/TD&gt; tagmi, ak sa na danom riadku nachádza aj popis kategórie
* Vykoná sa orezanie informácie medzi > a < (text medzi tagmi --> OSOBNÉ VOZIDLO, RENAULT a ďalšie parametre...)

<p align="center">
  <img src="https://i.imgur.com/r6PuXMz.png" />
</p>

* Scraper prechádza HTML kód webstránky riadok po riadku
* Možnosť overovať vozidlá, prípojné zariadenia, aj špeciálne značky (napr. kategória C a F)
* **Vstup EČV sa realizuje cez UART monitor (115200 baud/s)**
* Možnosť modifikácie zdrojového kódu pre mikrokontróler na zadanie VIN (potrebný kľúč vin v requeste)
* **Možnosť zadať viac EČV súčasne s oddeľovaním cez znak \n, za posledné EČV je znak vložený automaticky** (viz obrázok nižšie)
* Z hľadiska buffra UART zbernice možno hľadať naraz u ESP32 32 EČV, u ESP8266 16 EČV (za predpokladu 7 znakových EČV + ukončovacie znaky \n)   
# Príklad výstupu pre príklady UART:

<p align="center">
  <img src="https://i.imgur.com/dHOXFNc.png" />
</p>
