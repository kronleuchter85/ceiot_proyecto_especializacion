
#include "joystick_service.h"
#include "robot_position_state.h"
#include <string.h>

robot_position_t position_state = REST;


robot_position_t robot_position_state_get_action_by_name(char * cmd){

    robot_position_t action;

    if(!strcmp(cmd ,"MOVING_BACKWARD")){
        action = MOVING_BACKWARD;
    }else if( !strcmp(cmd ,"MOVING_FORWARD")){
        action = MOVING_FORWARD;
    }else if(!strcmp(cmd ,"ROTATE_RIGHT")){
        action = ROTATE_RIGHT;
    }else if(!strcmp(cmd ,"ROTATE_LEFT")){
        action = ROTATE_LEFT;
    }else{
        action = REST;
    }

    return action;
}

robot_position_t robot_position_state_get_action_by_coordinates(float command_x , float command_y){
    robot_position_t action;

    if(command_x == JOYSTICK_READING_MAX && command_y == JOYSTICK_READING_NEUTRAL){
        action = MOVING_BACKWARD;
    }else if( command_x == JOYSTICK_READING_MIN && command_y == JOYSTICK_READING_NEUTRAL){
        action = MOVING_FORWARD;
    }else if(command_x == JOYSTICK_READING_NEUTRAL && command_y == JOYSTICK_READING_MAX){
        action = ROTATE_LEFT;
    }else if(command_x == JOYSTICK_READING_NEUTRAL && command_y == JOYSTICK_READING_MIN){
        action = ROTATE_RIGHT;
    }else{
        action = REST;
    }
    return action;
}

void robot_position_state_update(robot_position_t action){
    position_state = action;
}

robot_position_t robot_position_state_get(){
    return position_state;
}