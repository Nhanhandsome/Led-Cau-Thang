#include<CauThang.h>
uint16_t Num_led  = so_led_1_bac*so_bac;
Adafruit_NeoPixel CauThang = Adafruit_NeoPixel(Num_led,PIN_OUT,NEO_GRB);
int so_led_1_bac = 5;
int so_bac = 6;
void display_with_color(int r,int g,int b){
    CauThang.Color(r,g,b);
    CauThang.show();
}
void display_all(uint32_t color){
  for(uint16_t i = 0;i<Num_led;i++)
  {
    CauThang.setPixelColor(i,color);
  }
  CauThang.show();
}
void clear_all(){
    CauThang.clear();
    CauThang.show();
}
void display_step(unsigned int step,uint32_t color){
    for(uint16_t i = 0;i<so_led_1_bac;i++){
        CauThang.setPixelColor(step*so_led_1_bac+i,color);
    }
    CauThang.show();
}
void clear_step(unsigned int step){
    for(uint16_t i = 0;i<so_led_1_bac;i++){
        CauThang.setPixelColor(step*so_led_1_bac+i,0);
    }
    CauThang.show();        
}
void display_1_pixel_on_step(unsigned int pixel,unsigned int step,uint32_t color){
    CauThang.setPixelColor(step*so_led_1_bac + pixel,color);
    CauThang.show();
}
void clear_1_pixel_on_step(unsigned int pixel,unsigned int step){
    CauThang.setPixelColor(step*so_led_1_bac + pixel,0);
    CauThang.show();
}
void Shift_left(void){
    for(uint16_t i = 0; i<so_led_1_bac;i++){
        for(uint16_t j = 0;j<so_bac;j++){
            if (j%2==0)
            {
                display_1_pixel_on_step(i,j,random(0x0000aa,0xffffff));
            }
            else{
                display_1_pixel_on_step(so_led_1_bac-i-1,j,random(0x0000aa,0xffffff));
            }
        }
        CauThang.show();
        delay(100);
    }
}
void Shift_right(void){
    for(uint16_t i = 0; i<so_led_1_bac;i++){
        for(uint16_t j = 0;j<so_bac;j++){
            if (j%2==0)
            {
                display_1_pixel_on_step(so_led_1_bac-i-1,j,random(0x0000aa,0xffffff));
            }
            else{
                display_1_pixel_on_step(i,j,random(0x0000aa,0xffffff));
            }
        }
        CauThang.show();
        delay(100);
    }   
}
void Back_to_font(void){
    for(uint16_t i = 0; i<so_bac;i++){
        display_step(i,0xff0000);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void Font_to_Back(void){
    for(uint16_t i = 0;i<so_bac;i++){
        display_step(so_bac-1-i,0x0000ff);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void Back_to_font_off(void){
    for(uint16_t i = 0;i<so_bac;i++){
        display_step(i,0x000000);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void Font_to_Back_off(void){
    for(uint16_t i = 0;i<so_bac;i++){
        display_step(so_bac-1-i,0x000000);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void Ring(void){
    for(uint16_t i = 0; i<so_bac*so_led_1_bac;i++){
        CauThang.setPixelColor(i,random(0x0000aa,0xffffff));
        CauThang.show();
        delay(10);
    }
}