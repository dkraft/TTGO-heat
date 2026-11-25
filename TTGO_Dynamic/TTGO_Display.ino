

void initDisplay(){
  tft.setRotation(3);  //rotate 90 degree
  tft.fillScreen(ST7735_BLACK);
  
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.init();  
  tft.fillScreen(ST7735_BLACK);
  tft.setSwapBytes(true);

  spr.createSprite(WIDTH,HEIGHT); //Width,Height
  spr.fillSprite(ST7735_BLACK);
    
  tft.setTextColor(ST7735_GRAY,ST7735_BLACK);
  tft.drawString("Start:", 90, 0, 4);
  tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString("23:59", 175, 0, 4);

  tft.setTextColor(ST7735_RED,ST7735_BLACK);
  tft.drawString("Temp:", 90, 25, 4);
  tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString("250 F", 175, 25, 4);
  
  tft.setTextColor(ST7735_GRAY,ST7735_BLACK);
  tft.drawString("Time:", 90, 50, 4);
  tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString("23:59", 175, 50, 4);
  
  tft.setTextColor(ST7735_GRAY,ST7735_BLACK);
  tft.drawString("Hold:", 90, 75, 4);
  tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString("148 F", 175, 75, 4);

  tft.setTextColor(ST7735_GREEN,ST7735_BLACK);
  tft.drawString("200F",0,25,4);


}//initDisplay 

void updateDisplay(){


  tft.setTextColor(ST7735_GREEN,ST7735_BLACK);
  tft.drawString(timeClient.getStrHours()+":"+timeClient.getStrMinutes(),0,0,4);

  int n; char h[3]; char m[3];
  n=sprintf(m,"%02d",minute(value[StartTime]));
  n=sprintf(h,"%02d",hour(value[StartTime]));

  if(CurrValue == StartTime)  tft.setTextColor(ST7735_ORANGE,ST7735_BLACK); else
    tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString(String(h) + ":" + m, 175, 0, 4);

  n=sprintf(m,"%03d",value[CookTemp]);
  if(CurrValue == CookTemp)  tft.setTextColor(ST7735_ORANGE,ST7735_BLACK); else
    tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString(String(m), 175, 25, 4);
  
  n=sprintf(m,"%02d",minute(value[CookTime]));
  n=sprintf(h,"%02d",hour(value[CookTime]));
  if(CurrValue == CookTime)  tft.setTextColor(ST7735_ORANGE,ST7735_BLACK); else
    tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString(String(h) + ":" + m, 175, 50, 4);
  
  n=sprintf(m,"%03d",value[HoldTemp]);
  if(CurrValue == HoldTemp)  tft.setTextColor(ST7735_ORANGE,ST7735_BLACK); else
    tft.setTextColor(ST7735_LIME,ST7735_BLACK);
  tft.drawString(String(m), 175, 75, 4);

}

void filltheSprite(){
  uint16_t n = random(10,100);
  // Draw 100 random colour pixels at random positions in sprite
  while (n--)
  {
    //uint16_t colour = random(0x10000); // Returns colour 0 - 0xFFFF
    uint16_t colour = ST7735_RED; // Returns colour 0 - 0xFFFF
    int16_t x = random(WIDTH);        // Random x coordinate
    int16_t y = random(HEIGHT);       // Random y coordinate
    spr.drawPixel( x, y, colour);      // Draw pixel in sprite
  }
  // Now push the sprite to the TFT at position 0,0 on screen
  spr.pushSprite(0,135-HEIGHT);
  spr.fillSprite(ST7735_BLACK);
}
