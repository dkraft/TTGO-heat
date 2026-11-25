  #define WIDTH 240 //sprite
  #define HEIGHT 25 //sprite

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI        19
#define TFT_SCLK        18
#define TFT_CS          5
#define TFT_DC          16
#define TFT_RST         23

#define TFT_BL          4  // Display backlight control pin
//#define ADC_EN          14
//#define ADC_PIN         34

//#define BUTTON_A_PIN  35
//#define BUTTON_B_PIN  0

//Button2 buttonA = Button2(BUTTON_A_PIN);
//Button2 buttonB = Button2(BUTTON_B_PIN);


//int btnCick = false;

// supported colors
#define ST7735_BLACK   0x0000
#define ST7735_GRAY    0x8410
#define ST7735_WHITE   0xFFFF
#define ST7735_RED     0xF800
#define ST7735_ORANGE  0xEC80
#define ST7735_YELLOW  0xFFE0  
#define ST7735_LIME    0x07FF
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_BLUE    0x001F
#define ST7735_MAGENTA 0xF81F
#define ST7735_PINK    0xF8FF
#define ST7735_CHYN    0x2D05
