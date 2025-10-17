

#include <driver/i2c.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "HD44780.h"

#define DISPLAY_SDA_PIN  18
#define DISPLAY_SCL_PIN  19
// #define DISPLAY_SDA_PIN  21
// #define DISPLAY_SCL_PIN  22

#define DISPLAY_LCD_ADDR 0x27
#define DISPLAY_LCD_COLS 16
#define DISPLAY_LCD_ROWS 2


void display_service_init(){
    LCD_init(DISPLAY_LCD_ADDR, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, DISPLAY_LCD_COLS, DISPLAY_LCD_ROWS);
}


void display_service_print(char * first_line , char * second_line){
   
    LCD_clearScreen();
    LCD_home();
    LCD_setCursor(0, 0);
    LCD_writeStr(first_line);
    
    LCD_setCursor(0, 1);
    LCD_writeStr(second_line);
        
}