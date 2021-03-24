#include <MyThread.h>

xTaskHandle SHIFTLEFT;
xTaskHandle SHIFTRIGHT;
xTaskHandle TEST_FROM_BLYNK;
xTaskHandle CONTROL_TASK;
xTaskHandle _di_len = NULL;
xTaskHandle _di_xuong = NULL;
xQueueHandle _Blynk_mess = NULL;
void Di_len(void *parameter){
    Font_to_Back();
    vTaskDelay(5000/portTICK_PERIOD_MS);
    //Font_to_Back_off();
    Back_to_font_off();
    _di_len = NULL;
    vTaskDelete(_di_len);
    
}
void Di_xuong(void *parameter){
    Back_to_font();
    vTaskDelay(5000/portTICK_PERIOD_MS);
    //Back_to_font_off();
    Font_to_Back_off();
    _di_xuong = NULL;
    vTaskDelete(_di_xuong);
    
}
void xshiftleft(void *parameter)
{
    Shift_left();
    clear_all();
    delay(2000);
    vTaskDelete(SHIFTLEFT);
}
void xshiftright(void *parameter)
{
    Shift_right();
    clear_all();
    delay(2000);
    vTaskDelete(SHIFTRIGHT);
}

void control_led(void *parameter)
{
    while (1)
    {
        if (_Blynk_mess != NULL)
        {
            String string;
            if (xQueueReceive(_Blynk_mess, &string, (TickType_t)10) == pdPASS)
            {
                    if (string == "play")
                    {
                        xTaskCreate(test_from_Blynk, "test", 2000, NULL, 2, &TEST_FROM_BLYNK);
                    }
                    else                  
                    {
                        vTaskDelete(TEST_FROM_BLYNK);
                        clear_all();
                    }
            }
        }
        if(Serial.available()){
            char c = Serial.read();
            if(c == 'a'){
                if(_di_len == NULL){
                    Serial.println("Di len");
                    xTaskCreate(Di_len,"di len",1000,NULL,2,&_di_len);
                }
            }
            else if(c == 'b'){
                if(_di_xuong == NULL){
                    Serial.println("Di xuong");
                    xTaskCreate(Di_xuong,"di xuong",1000,NULL,2,&_di_xuong);
                    }
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void test_from_Blynk(void *parameter)
{
    for (;;)
    {
        display_all(random(0x0000ff, 0xffffff));
        delay(2000);
        clear_all();
        delay(200);
        Shift_left();
        delay(2000);
        clear_all();
        delay(500);
        Shift_right();
        delay(2000);
        clear_all();
        delay(500);
        Back_to_font();
        delay(1000);
        clear_all();
        delay(500);
        Font_to_Back();
        delay(1000);
        clear_all();
        delay(500);
        Ring();
        delay(2000);
        clear_all();
        delay(500);
    }
}
void begin_setup()
{
    Serial.begin(112500);
    CauThang.begin();
    pinMode(sensor1,INPUT);
    pinMode(sensor2,INPUT);
    CauThang.setBrightness(30);
    CauThang.show();
    xTaskCreate(control_led, "Control Led", 2000, NULL, 2, &CONTROL_TASK);
    _Blynk_mess = xQueueCreate(10, sizeof(String));
}