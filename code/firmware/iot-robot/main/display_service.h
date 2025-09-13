
#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H


#define DISPLAY_SDA_PIN  18
#define DISPLAY_SCL_PIN  19
// #define DISPLAY_SDA_PIN  21
// #define DISPLAY_SCL_PIN  22

#define DISPLAY_LCD_ADDR 0x27
#define DISPLAY_LCD_COLS 16
#define DISPLAY_LCD_ROWS 2

void display_service_init();
void display_service_print(char * first_line , char * second_line);


#endif