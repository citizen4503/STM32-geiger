# GAMA32

- STM32 ARM Cortex M3
- Zařízení počítá celkový počet pulsů z trubice STS-5. Díky využití vnitřního RTC se přepočítává počet pulsů za vteřinu (CPS) a za minutu (CPM).
- Každý příchozí impuls jde na vstup externího přerušení, zpracuje se funkce obsluhy přerušení a jde se od znova.
- Zapojení je dovybaveno displejem a dvěma tlačítky pro uživatelský interface, který nabízí nulování času, a resetování naměřených hodnot.
- Nahradit anodový rezistor alespoň 2W z důvodu narůstajícího proudu při zvyšujícím se počtu pulsů.
- Zákmit tlačítek je vyřešen softwarově.
- Průměrný počet naměřených pulsů za minutu přirozeného pozadí cca 13.

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

<hr />

![IMG_20210414_144256_959](https://user-images.githubusercontent.com/67713227/116217670-27146a80-a74a-11eb-9e31-9ce30d069ded.jpg)
