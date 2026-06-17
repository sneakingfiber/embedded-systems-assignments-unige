#include <xc.h>
#include <p33EP512MU810.h>
#include "lights.h"

void left_side_lights_init(){
    ANSELBbits.ANSB8 = 0;
    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8   = 0;
}
void right_side_lights_init(){
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1   = 0;
}
//RG1 low intensity lights 
void  low_intensity_lights_init(){
    TRISGbits.TRISG1 = 0;
    LATGbits.LATG1   = 0;
} 
void low_intensity_lights(light_state state){
    LATGbits.LATG1 = (state == ON) ? 1 : 0;
}
void left_side_lights(light_state state){
    LATBbits.LATB8 = (state == ON) ? 1 : 0;
}
void right_side_lights(light_state state){
    LATFbits.LATF1 = (state == ON) ? 1 : 0;
} 
void left_side_lights_toggle(){
    LATBbits.LATB8 = !LATBbits.LATB8;
}
void right_side_lights_toggle(){
    LATFbits.LATF1 = !LATFbits.LATF1;
}