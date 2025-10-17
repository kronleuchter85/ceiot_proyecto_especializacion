
#ifndef ROBOT_POSITION_STATE_H
#define ROBOT_POSITION_STATE_H

typedef enum{

    MOVING_FORWARD,
    MOVING_BACKWARD,
    ROTATE_RIGHT,
    ROTATE_LEFT,
    REST

} robot_position_t;

robot_position_t robot_position_state_get_action_by_name(char *);
robot_position_t robot_position_state_get_action_by_coordinates(float command_x , float command_y);

void robot_position_state_update(robot_position_t);

robot_position_t robot_position_state_get();

#endif