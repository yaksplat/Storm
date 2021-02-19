
//audio pins for an M0 board
#define VS1053_RESET   -1 
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin



//LED strip config
#define LED_COUNT_S 192
#define LED_COUNT_E 147
#define LED_COUNT_N 147
#define LED_COUNT_W 188

#define LED_PIN_S    0// 5
#define LED_PIN_E    1// 6
#define LED_PIN_N    11// 9
#define LED_PIN_W    12// 10




#define BRIGHTNESS 255

#define Nflipped  true
#define Wflipped  true
#define Eflipped  false
#define Sflipped  false

//Storm Config
#define DiameterMin 100
#define DiameterMax 800
#define StormSpeed 2500
#define DistanceXMin -700
#define DistanceXMax -500
#define DistanceYMin -500
#define DistanceYMax 500 

//lightning params
#define FlashCountMin 3
#define FlashCountMax 15 
#define FlashDurationMin 1
#define FlashDurationMax 45 
#define NextFlashDelayMin 1
#define NextFlashDelayMax 125 

//Storm intensity Config
#define Cat0Value 0
#define Cat0Bright 20
#define Cat0FlashSizeMin 3
#define Cat0FlashSizeMax 8
#define Cat0FlashVariation 3
#define Cat0LightningIntervalMin 4500
#define Cat0LightningIntervalMax 5500

#define Cat1Value 1
#define Cat1Bright 10
#define Cat1FlashSizeMin 5
#define Cat1FlashSizeMax 18
#define Cat1FlashVariation 5
#define Cat1LightningIntervalMin 4500
#define Cat1LightningIntervalMax 5500

#define Cat2Value 2
#define Cat2Bright 4
#define Cat2FlashSizeMin 12
#define Cat2FlashSizeMax 25
#define Cat2FlashVariation 12
#define Cat2LightningIntervalMin 4500
#define Cat2LightningIntervalMax 5500

#define Cat3Value 3
#define Cat3Bright 3
#define Cat3FlashSizeMin 20
#define Cat3FlashSizeMax 40
#define Cat3FlashVariation 20
#define Cat3LightningIntervalMin 3500
#define Cat3LightningIntervalMax 4500

#define Cat4Value 4
#define Cat4Bright 2
#define Cat4FlashSizeMin 35
#define Cat4FlashSizeMax 80
#define Cat4FlashVariation 35
#define Cat4LightningIntervalMin 3500
#define Cat4LightningIntervalMax 4500

#define Cat5Value 5
#define Cat5Bright 1
#define Cat5FlashSizeMin 175
#define Cat5FlashSizeMax 200
#define Cat5FlashVariation 60
#define Cat5LightningIntervalMin 1500
#define Cat5LightningIntervalMax 3500

#define LightningMovementThresholdMin 50
#define LightningMovementThresholdMax 80
#define LightningMovementMin 1
#define LightningMovementMax 8



