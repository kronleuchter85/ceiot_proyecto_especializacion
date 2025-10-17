

#ifndef JOYSTICK_SERVICE_H
#define JOYSTICK_SERVICE_H

#define JOYSTICK_READING_MAX 1
#define JOYSTICK_READING_MIN -1
#define JOYSTICK_READING_NEUTRAL 0


#include <stdio.h>

void joystick_initialize();

void joystick_get_reading(int * read_x , int * read_y);

#endif