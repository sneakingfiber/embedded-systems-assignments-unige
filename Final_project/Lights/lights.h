#ifndef LIGHTS_H
#define LIGHTS_H

#include <xc.h>
#include <p33EP512MU810.h>
typedef enum light_state{
    OFF=0,
    ON
}light_state;   

void left_side_lights_init();
void right_side_lights_init();
void low_intensity_lights_init();
void low_intensity_lights(light_state state);
void left_side_lights(light_state state);
void right_side_lights(light_state state);
void left_side_lights_toggle();
void right_side_lights_toggle();

#endif // LIGHTS_H