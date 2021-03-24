#include <Arduino.h>
#include <MyThread.h>
#include <WiFiConfigManager.h>
int on_off_global = 0;
BLYNK_WRITE(V0){
  wifi_setup();
}
BLYNK_WRITE(V1){
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  Serial.print("color: ");Serial.println((long)r*256*256+g*256+b);
  if(on_off_global == 1)
    display_all((long)r*256*256+g*256+b);
  else
    return;
  //xQueueSend(_Blynk_mess_color,&sw,(TickType_t)10);
}
BLYNK_WRITE(V2){
  on_off_global = param.asInt();
  Serial.print("Button : ");Serial.println(on_off_global);
  if(on_off_global == 1){
    display_all((uint32_t)random(0x0000ff,0xffffff));
    
    }
  else{
    //vTaskSuspendAll();
    //vTaskDelete(CONTROL_TASK);
    //String sw = "OFF ALL";
    //xQueueSend(_Blynk_mess,&sw,(TickType_t)10);
    clear_all();
  }
}
BLYNK_WRITE(V3){
  String sw = param.asString();
  Serial.println(sw);
  if(on_off_global == 1){
    xQueueSend(_Blynk_mess,&sw,(TickType_t)10);
  }
}
void setup()
{
  // put your setup code here, to run once:

  begin_setup();
  wifi_begin();
  //Blynk.begin(auth,ssid,pass);
}

void loop()
{
  WiFi_Process();
}