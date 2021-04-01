#include <MyThread.h>

xTaskHandle _task_off = NULL;
xTaskHandle TEST_FROM_BLYNK = NULL;
xTaskHandle CONTROL_TASK = NULL;
xTaskHandle _di_len = NULL;
xTaskHandle _di_xuong = NULL;
xQueueHandle _Blynk_mess = NULL;
xQueueHandle _task_queue = NULL;

void Di_len(void *parameter)
{
    Font_to_Back();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    //Font_to_Back_off();
    //Back_to_font_off();
    int i = 1;
    xQueueSend(_task_queue, &i, (TickType_t)10);
    _di_len = NULL;
    vTaskDelete(_di_len);
}
void Di_xuong(void *parameter)
{
    Back_to_font();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    //Back_to_font_off();
    //Font_to_Back_off();
    int i = 1;
    xQueueSend(_task_queue, &i, (TickType_t)10);
    _di_xuong = NULL;
    vTaskDelete(_di_xuong);
}
void OFF_ALL(void *parameter)
{
    off_font_to_mid();
    vTaskDelay(500 / portTICK_PERIOD_MS);
    _task_off = NULL;
    vTaskDelete(_task_off);
}
extern int on_off_global;
void control_led(void *parameter)
{
    unsigned long point_time = millis();
    while (1)
    {
        int time = 0;
        if(Serial.available()){
            char c = Serial.read();
            if(c == '1'){
                point_time = millis();
            }
        }
        if (_task_queue != NULL)
        {
            if (xQueueReceive(_task_queue, &time, (TickType_t)10) == pdPASS)
            {
                if (time == 1)
                {
                    point_time = millis();
                }
                
            }
        }
        if (millis() - point_time > Time_delay)
        {
            // task off
            if (_task_off == NULL)
            {
                point_time = millis();
                xTaskCreate(OFF_ALL, "OFF", 1000, NULL, 2, &_task_off);
            }
            else
            {
            }
        }
        if(on_off_global == 1){
        if (digitalRead(sensor1) == 0)
        {
            // Task đi lên
            delay(100);
            if (digitalRead(sensor1)==0)
            {
                point_time = millis();
                if (_task_off != NULL)
                {
                    _task_off = NULL;
                    vTaskDelete(_task_off);
                }
                if (_di_len == NULL)
                {
                    /* code */
                    xTaskCreate(Di_len, "Dilen", 1000, NULL, 2, &_di_len);
                }
                else
                {
                    //nothing
                }
            }
        }
        if (digitalRead(sensor2) == 0)
        {
            // Task đi xuống
            point_time = millis();
            delay(100);
            if (digitalRead(sensor2)==0)
            {
                if (_task_off != NULL)
                {
                    _task_off = NULL;
                    vTaskDelete(_task_off);
                }
                if (_di_xuong == NULL)
                {
                    xTaskCreate(Di_xuong, "Dixuong", 1000, NULL, 2, &_di_xuong);
                }
                else
                {
                }
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
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    CauThang.setBrightness(30);
    CauThang.show();
    xTaskCreate(control_led, "Control Led", 5000, NULL, 2, &CONTROL_TASK);
    _Blynk_mess = xQueueCreate(10, sizeof(String));
    _task_queue = xQueueCreate(10, sizeof(int));
}