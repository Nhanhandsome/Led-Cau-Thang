#include <FreeRTOS.h>
#include <Arduino.h>
#include <CauThang.h>
//extern xTaskHandle timer;
#define sensor1 13
#define sensor2 15
#define Time_delay 20000
extern xTaskHandle CONTROL_TASK;
extern xQueueHandle _Blynk_mess;
extern xQueueHandle _Blynk_mess_color;
void xTaskTimer(void *parameter);
void control_led(void *parameter);
void xshiftleft(void *parameter);
void xshiftright(void *parameter);
void begin_setup();
void test_from_Blynk(void *parameter);
void Di_len(void *parameter);
void Di_xuong(void *parameter);
