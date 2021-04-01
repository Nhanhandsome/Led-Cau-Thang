#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#define PIN_OUT 4
extern int so_led_1_bac;
extern int so_bac;
//extern uint16_t time_off[so_bac];
extern uint16_t Num_led;
extern Adafruit_NeoPixel CauThang;
void display_with_color(int g,int r,int b);
void display_all(uint32_t color);
void clear_all(void);
void display_step(unsigned int step,uint32_t color);
void clear_step(unsigned int step);
void display_1_pixel_on_step(unsigned int pixel,unsigned int step,uint32_t color);
void clear_1_pixel_on_step(unsigned int pixel,unsigned int step);
void Shift_left(void);
void Shift_right(void);
void Back_to_font(void);
void Back_to_font_off(void);
void Font_to_Back_off(void);
void Font_to_Back(void);
void Ring(void);
void off_font_to_mid(void);

