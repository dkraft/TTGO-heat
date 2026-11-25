

//double Kp = 2, Ki = 0, Kd = 0;
//PID_v2 H1_PID(Kp, Ki, Kd, PID::Direct);
//const int WindowSize = 500;
//unsigned long WindowStartTime;
//int H1Temp;
//int H1SetPoint = 100;

double Setpoint, Input, Output;    //Define PID variables we'll be connecting to

//float Kp = 16.16;    //User inserts PID values obtaines from Autotune here
//float Ki = 0.14;    //
//float Kd = 480.10;  //
float Kp = 15;    //User inserts PID values obtaines from Autotune here
float Ki = 0.2;    //
float Kd = 10.0;  //

PID myPID(&Input, &Output, &Setpoint,Kp, Ki, Kd, DIRECT);  //Specify the links and initial tuning parameters


int WindowSize = 5000;    // I have found for espresso machines that a 1000ms window does well
unsigned long windowStartTime;  // used in calculating PID output pulse length
double timeNowMins;                // used for logging time in Serial Window

void doPID()  {
  Setpoint = value[CookTemp];
  myPID.SetOutputLimits(0, PWM);
  Input = temp1; // PID input is measured averaged temp
  myPID.SetMode(AUTOMATIC);
  myPID.Compute(); //PID calculation
  
  /************************************************
  * turn the output pin on/off based on pid output
  ************************************************/
  unsigned long now = millis();
  //while(millis() - windowStartTime > WindowSize){    //time to shift the Relay Window
  //  windowStartTime += WindowSize;
  //  }//while shifting
    
  if(Output > .01)
    pwm.analogWrite(SSR,Output);
  else 
    pwm.analogWrite(SSR,0);

  Serial.print("Setpoint:");
  Serial.print(Setpoint);    // uncomment these lines if
  Serial.print("\t");      // logging data to  
  Serial.print("Input:");
  Serial.print(Input);    // uncomment these lines if
  Serial.print("\t");      // logging data to  
  Serial.print("Output:");
  timeNowMins = millis() / 60000.0;    //Serial 
  Serial.println(Output);        // Window
}//doPID

void updateHeater(){
  filltheSprite();
  if ( (hour(value[StartTime])*60 + minute(value[StartTime]) + hour(value[CookTime])*60 + minute(value[CookTime]))
    >= (timeClient.getHours()*60 + timeClient.getMinutes())  )
      
    if ( (hour(value[StartTime])*60 + minute(value[StartTime])) <= (timeClient.getHours()*60 + timeClient.getMinutes()) ){
      spr.fillSprite(ST7735_RED);
      doPID();
      }//if time to light the fire
    
      else {
        spr.fillSprite(ST7735_BLACK);
        digitalWrite(SSR,LOW); 
        //Serial.println("---Heater OFF---");
      }
    
}//updateHeater

void updateTempF() {
  int n; char h[3]; char m[3]; float tempAVG =0.0;
  // average readings over a second
  for (int i=0;i<2;i++){
    temp1 = thermo.readFahrenheit(); delay(100);
    tempAVG += temp1;
  }
  temp1 = tempAVG / 2;
  
//  Serial.println("THERMO temp1: " + String(temp1));
  
  n=sprintf(m,"%03.f",temp1);
  if(temp1 <= value[CookTemp])
    tft.setTextColor(ST7735_RED,ST7735_BLACK);
    else
      tft.setTextColor(ST7735_GREEN,ST7735_BLACK);
  tft.drawString(String(m),0,25,4);
}
