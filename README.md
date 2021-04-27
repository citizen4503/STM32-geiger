# STM32-geiger
Projekt vznikl pro seznámení s STM32 Cortex M3. Zařízení počítá celkový počet pulsů z trubice STS-5. Díky využití vnitřního RTC se přepočítává počet pulsů za vteřinu (CPS) a za minutu (CPM).
Jelikož nemám k dispozici kvalitní osciloskop, nemám naměřené přesné délky pulsů. Každý příchozí impuls jde na vstup externího přerušení, zpracuje se funkce obsluhy přerušení a jde se od znova. Předpokládám, že při vyšším vybuzení trubice dojde ke značnému zkreslení naměřených pulsů, protože neřeším dobu měření.
Zapojení je dovybaveno displejem a dvěma tlačítky pro uživatelský interface, který nabízí nulování času, a resetování naměřených hodnot. Taky by nebylo od věci nahradit anodový rezistor nějakým výkonovým, alespoň na 2W z důvodu narůstajícího proudu při zvyšujícím se počtu pulsů. Zákmit tlačítek je vyřešen softwarově. Průměrný počet naměřených pulsů za minutu přirozeného pozadí cca 13.

<hr />

![IMG_20210414_144256_959](https://user-images.githubusercontent.com/67713227/116217670-27146a80-a74a-11eb-9e31-9ce30d069ded.jpg)

<hr />

LCD
Bluepill  |  LCD  |  code
---  |  ---  |  --- 
`PA9`  |  `RST`  |  `LCD_RST`
`PA10`  |  `CE`  |  `LCD_CE`
`PA11`  |  `DC`  |  `LCD_DC`
`PA12`  |  `DIN`  |  `LCD_DIN`
`PA15`  |  `CLK`  |  `LCD_CLK`
`PB3`  |  `LIGHT`  |  `LCD_LIGHT`

Buttons
Bluepill  |  Button  |  Pull up/down
---  |  ---  |  ---
`PB14`  |  `BTN1`  |  `pull-down`
`PB15`  |  `BTN2`  |  `pull-down`
