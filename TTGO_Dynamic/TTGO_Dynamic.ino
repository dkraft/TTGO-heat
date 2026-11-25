
#include <Wire.h>
#include <Button2.h>
#include <TFT_eSPI.h> // ST7789
TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object

#include <SPI.h>
#include <Preferences.h>
Preferences preferences;
#include <OneWire.h>
#include <DallasTemperature.h> //https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <PID_v2.h>
#include <PWMOutESP32.h>
PWMOutESP32 pwm(6,1); // resolution / frequency 2^7 = 128, 2^6 = 64 (doesn't matter here - just for scaling graph..)
int PWM; // max pwm value for output range

#include "TTGO_Display.h"
#include <max6675.h>
#define thermoDO  26
#define thermoCS  25
#define thermoCLK 33
MAX6675 thermo(thermoCLK,thermoCS,thermoDO);

#include <User_Setups/Setup25_TTGO_T_Display.h>
#include <EncoderButton.h>
//EncoderButton eb1(<ENCODER_PIN1>, <ENCODER_PIN2>, <SWITCH_PIN>);
#define DS1 13
#define DS2 15
#define DS3 12
#define SSR 27

//#define ONE_WIRE_BUS 22
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature DS18b20(&oneWire );
//DeviceAddress temp1Address;

float temp1;
uint32_t updateTime = 0;       // time for next update in rotary handlers
unsigned long value[4]; // StartTime,Temp,CookTime,HoldTemp
int updateTemp; //  milliseconds to update temp
char ValName[4][10] = {"StartTime","Temp","CookTime","HoldTemp"} ;
#define StartTime 0 //for readability indexing values
#define CookTemp  1
#define CookTime  2
#define HoldTemp  3
int CurrValue = 0; //which variable we're working on



#if !( defined(ESP8266) ||  defined(ESP32) )
  #error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

#define NTP_DBG_PORT                Serial

// Debug Level from 0 to 4
#define _NTP_LOGLEVEL_              0

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <NTPClient_Generic.h>          // https://github.com/khoih-prog/NTPClient_Generic

#if (ESP32)
  #include <WiFi.h>
#elif (ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <WiFiUdp.h>

#include "secrets.h"

WiFiUDP ntpUDP;

// NTP server
//char timeServer[] = "time.nist.gov";
char timeServer[] = "pool.ntp.org";

#define TIME_ZONE_OFFSET_HRS            (-5)
#define NTP_UPDATE_INTERVAL_MS          360000L // 3600000L millisecond  = 1 hour

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, timeServer, (3600 * TIME_ZONE_OFFSET_HRS), NTP_UPDATE_INTERVAL_MS);

int WiFiStatus = WL_IDLE_STATUS;

void IRAM_ATTR DSInterrupt () { //ISR DS1 Rotary Encoder 
  //Serial.println(read_rotary());
  //Serial.println(millis()-updateTime);
  noInterrupts();
  //rotarytest();
  if((millis()-updateTime)>3) {//filter noise
      updateTime = millis(); // Next update time

  //Serial.print(String(read_rotary())+":");
      
      //if (digitalRead(DS1) == digitalRead(DS2)){ //determine direction
      if (digitalRead(DS2)){ //determine direction
        value[CurrValue]++;
        if((CurrValue == StartTime) || (CurrValue == CookTime)) value[CurrValue] += 599;
      } else {//if (digitalRead(DS1) != digitalRead(DS2)){ 
        value[CurrValue]--;
        if((CurrValue == StartTime) || (CurrValue == CookTime)) value[CurrValue] -= 599;
  }//else

  updateTime = millis(); // Next update time
  
 //Serial.print(ValName[CurrValue]); Serial.print(": ");Serial.println(value[CurrValue]);
  
  interrupts();
    
  }//if debounced

}//DSInterrupt

void  DSButton () {    //ISR
  if((millis()-updateTime)>150){

    CurrValue++;
    if (CurrValue>3) CurrValue=0;
    Serial.print("Value: "); Serial.println(ValName[CurrValue]);
    
  }//if debounced
  updateTime = millis(); // Next update time

}//DSButton

float plot[360];

void init_plot(){
  randomSeed(analogRead(0)); // Initialize random number generator
  // Initialize the plot array with random values between 0 and 2
  for (int i = 0; i < 360; i++) {
    plot[i] = random(0, 2000) / 1000.0; // random values from 0 to 2
  }
}
void drawCircularImage() {
  int centerX = tft.width() / 2;
  int centerY = tft.height() / 2;
  int baseRadius = min(centerX, centerY) - 10; // Base radius for the circle

  tft.drawCircle(centerX, centerY, baseRadius, TFT_WHITE);

  for (int i = 0; i < 360; i++) {
    float angle = i * PI / 180; // Convert to radians
    int radius = baseRadius * plot[i]; // Adjust radius based on plot data
    int x = centerX + radius * cos(angle);
    int y = centerY + radius * sin(angle);

    tft.drawPixel(x, y, TFT_GREEN); // Plot each point
  }
}

void setup(){
  // This odd speed will show ESP8266 boot diagnostics too
  Serial.begin(74880);
  delay(500);
  Serial.begin(115200);
  while (!Serial && millis() < 5000); Serial.println("#");
  
  preferences.begin("values",false);
  //preferences.clear();
  
  for (int i=0;i<4;i++)  {
    value[i] = preferences.getULong(ValName[i],0);
    Serial.print(String(ValName[i]) + ": ");Serial.println(value[i]);
  }
      
  Serial.println("\nStarting ESP_NTPClient_Advanced on " + String(ARDUINO_BOARD));
  Serial.println(NTPCLIENT_GENERIC_VERSION);
  Serial.println("Connecting to: " + String(WLAN_SSID));
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while ( WiFi.status() != WL_CONNECTED )
  {
    delay ( 1500 );
    Serial.print ( "." );
  }

  WiFiStatus = WL_CONNECTED;

  Serial.print(F("\nESP_NTPClient_Advanced started @ IP address: "));
  Serial.println(WiFi.localIP());

  timeClient.begin();
  updateNTP();
  //value[StartTime] = timeClient.getEpochTime();

  Serial.println("\nUsing NTP Server " + timeClient.getPoolServerName());
 
  initDisplay(); 

  //pinMode(ONE_WIRE_BUS,INPUT_PULLUP);
  delay(500);
  //DS18b20.begin();
  //oneWire.reset_search();delay(500);
  //Serial.print("Found ");
  //Serial.println(DS18b20.getDeviceCount(), DEC);
  //DS18b20.requestTemperatures();delay(100);


  //if(DS18b20.getAddress(temp1Address,0)) Serial.println("temp1: " + String(temp1)); else
      //Serial.println("*** Error on temp1 ***");
  //float  temp1 = DS18b20.getTempFByIndex(0); delay(100);
  //Serial.print("temp1 Resolution: "); Serial.println(DS18b20.getResolution(temp1Address), DEC);
  float temp1 = thermo.readFahrenheit(); delay(100);
  Serial.println("THERMO temp1: " + String(temp1));
  pinMode(SSR,OUTPUT);
  Serial.print("PWM Max Output ");
  Serial.println(pwm.getMaxPWMValue());
  
  PWM = pwm.getMaxPWMValue();
  pinMode(DS1, INPUT_PULLUP);//Digital Pot A
  pinMode(DS2, INPUT_PULLUP);//Digital Pot B
  pinMode(DS3, INPUT_PULLUP);//Digital Pot Pushbutton

  
  delay(100); // wait for R-C filter to charge
  digitalWrite(SSR,LOW);
  attachInterrupt(digitalPinToInterrupt(DS1), DSInterrupt, HIGH);
  attachInterrupt(digitalPinToInterrupt(DS3), DSButton, HIGH);
     
}//setup


void loop()
{
  if(millis() - updateTemp >= 5000) {
    updateTempF();
    updateTemp = millis();
    saveValues();
    updateHeater();

  }
  
  updateDisplay();

  updateNTP();
  
  }//loop


void saveValues(){
      for (int i=0;i<4;i++) 
      if (!preferences.putULong(ValName[CurrValue],value[CurrValue])) 
        Serial.println ("*** Whoa! failed to write: " + String(ValName[CurrValue]));
    
    for (int i=0;i<4;i++)  {
      unsigned long tempvalue = preferences.getULong(ValName[i],0);
      //Serial.print(String(ValName[i]) + ": ");Serial.println(tempvalue);
    }
    //Serial.print("StartTime: ");Serial.print(hour(value[StartTime]));Serial.print(" CurrentTime: ");
    //Serial.println(timeClient.getHours());
}
