void updateNTP(){
  static unsigned long wifiStatus_timeout   = 0;
  static unsigned long updateStatus_timeout = 0;

  #define WIFI_CHECK_INTERVAL       3600000L
  #define UPDATE_INTERVAL           60000L

  if ((millis() > wifiStatus_timeout) || (wifiStatus_timeout == 0))
  {
    if (WiFi.status() != WiFiStatus)
    {
      WiFiStatus = WiFi.status();
  
      if (WiFiStatus == WL_CONNECTED)
        Serial.println("Changed, WiFiStatus = Connected");
      else if (WiFiStatus == WL_DISCONNECTED)
        Serial.println("Changed, WiFiStatus = Disconnected");
      else if (WiFiStatus == WL_IDLE_STATUS)
        Serial.println("Changed, WiFiStatus = Idle");
      else if (WiFiStatus == WL_NO_SSID_AVAIL)
        Serial.println("Changed, WiFiStatus = lost WiFi SSID");
      else if (WiFiStatus == WL_CONNECTION_LOST)
        Serial.println("Changed, WiFiStatus = lost WiFi connection");
      else
      {
        Serial.print("Changed, WiFiStatus = "); Serial.println(WiFiStatus);
      }
    }
   
    wifiStatus_timeout = millis() + WIFI_CHECK_INTERVAL;
  }
  
  // Send update request every UPDATE_INTERVAL seconds: we don't need to send update request frequently
  if ((millis() > updateStatus_timeout) || (updateStatus_timeout == 0)) {
    timeClient.update();
    
//  Serial.println(String("StartTime : ") + (hour(value[StartTime])*60 + minute(value[StartTime])) );
//  Serial.println(String("EndTime: ") + (hour(value[StartTime])*60 + minute(value[StartTime]) + hour(value[CookTime])*60 + minute(value[CookTime])) ) ;
//  Serial.println(String("NOW: ") + (timeClient.getHours()*60 + timeClient.getMinutes()));
//  DS18b20.requestTemperatures();
//  temp1 = DS18b20.getTempFByIndex(0);
//  if (temp1 != DEVICE_DISCONNECTED_C)
//    Serial.println("temp1: " + String(temp1)); else
//      Serial.println("*** Error on temp1 ***");



  
    if (timeClient.updated())
//      Serial.println("------UPDATED------"); else Serial.println("******NOT UPDATED******");
//      Serial.println("LOC : " + timeClient.getFormattedTime());
//      Serial.println("LOC : " + timeClient.getFormattedDateTime());
//      Serial.println("LOC EPOCH : " + String(timeClient.getEpochTime()));
//  
//      Serial.println(String("LOC : ") + timeClient.getHours() + ":" + timeClient.getMinutes() + ":" + timeClient.getSeconds() + " " +
//                   timeClient.getDoW() + " " + timeClient.getDay() + "/" + timeClient.getMonth() + "/" + timeClient.getYear() + " or " +
//                   timeClient.getDay() + " " + timeClient.getMonthStr() + " " + timeClient.getYear());
      // With leading 0
//      Serial.println(String("LOC : ") + timeClient.getStrHours() + ":" + timeClient.getStrMinutes() + ":" + timeClient.getStrSeconds() + " " +
//                   timeClient.getDoW() + " " + timeClient.getDay() + "/" + timeClient.getMonth() + "/" + timeClient.getYear() + " or " +
//                   timeClient.getDay() + " " + timeClient.getMonthStr() + " " + timeClient.getYear());
    
    updateStatus_timeout = millis() + UPDATE_INTERVAL;
  }
  

}//updateStatus
