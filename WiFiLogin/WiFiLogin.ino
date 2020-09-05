/***********************************************************************
Description : ESP32/8266 Wifi login/smartconfig example                         
Date        : 09_03_20
Author      : mcalyer

************************************************************************/

#include "WiFi.h"           // ESP32 Creates instance of WifiClass named Wifi (extern WiFiClass WiFi)
//#include <ESP8266WiFi.h>  // ESP8266 Creates instance of WifiClass named Wifi (extern WiFiClass WiFi)

#include <Preferences.h>    // Save/Restore ssid , passphrase 
Preferences prefs;

bool wifi_login(void)
{
  // Login Time out = 10 seconds
  int connect_counter = 20;
  
  // Get Stored SSIS , passphrase
  prefs.begin("_neo", false); 
  String _ssid = prefs.getString("ssid" , "none");
  String _pw   = prefs.getString("pw"   , "none");
  prefs.end(); 
  
  if(_ssid == "none" || _pw == "none")
  {
    return false;
  }
  if(WL_CONNECT_FAILED == WiFi.begin(_ssid.c_str() , _pw.c_str()))
  {
    return false;
  }
  while (WiFi.status() != WL_CONNECTED)
  {    
    delay(500);    
    connect_counter = connect_counter - 1;
    if(connect_counter == 0) 
    {
      return false;
    }          
  }
  return true;
}


bool wifi_smart_config(void)
{  
  
  // Set Time out = 120 seconds
  int connect_counter = 240;
  
  //Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);
  if(!WiFi.beginSmartConfig())
  {    
    return false;
  }

  //Wait for SmartConfig packet from mobile  
  while (!WiFi.smartConfigDone())
  {
    delay(500);   
    connect_counter = connect_counter - 1;
    if(connect_counter == 0) 
    {
      return false;
    }   
  }            
  
  // Store SSID , passphrase
  String _ssid = WiFi.SSID(); 
  String _pw   = WiFi.psk();
  
  prefs.begin("_neo", false); 
  size_t err_ssid = prefs.putString("ssid" , _ssid);
  size_t err_pw   = prefs.putString("pw"   , _pw);
  prefs.end();

  if (0 == err_ssid || 0 == err_pw)
  {
    return false;
  }

  return true;
}


/******************* Setup *****************************************/
void setup() 
{
  Serial.begin(115200);
  delay(4000);

  // WiFi Client Connect
  if(wifi_login())
  {
    Serial.println("WiFi Login Success !");  
  }
  else if(wifi_smart_config())
  {
    Serial.println("WiFi SmartConfig Success !");   
  }
  else
  {
    Serial.println("WiFi Connect Failed !");   
    delay(1000);
    ESP.restart();
  }
   
  // Connected  
  delay(1000);  
  Serial.println(WiFi.SSID().c_str());
  Serial.println(WiFi.psk().c_str());
  Serial.println(WiFi.localIP());
 
} // End of Setup  

/******************* Loop *****************************************/

void loop() 
{
  
  delay(1000);
  
}  // End of Loop
