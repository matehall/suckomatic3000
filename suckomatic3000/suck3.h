// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _suck3_H_
#define _suck3_H_
#include "Arduino.h"
//add your includes for the project suck3 here
#include <LiquidCrystal.h>


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project suck3 here

const int box_empty_level = 65;
const int box_full_level = 45;

//ultrasonic sound
const int triggerPin = 13;
const int echoPin = 12;

long fuel_level = 0;


//States
const int EMPTY = 1;
const int FULL = 0;
int last_major_state = EMPTY;
const int CLEAR = 0;
const int VACONTOLONG = 1;

int error_state = CLEAR;

//Vac control
const int ON_BUTTON = 2;
const int OFF_BUTTON = 3;
const int VAC_OFF = 0;
const int VAC_ON = 1;
const int CYCLON_FULL = 600;
int vac_state = VAC_OFF;

unsigned long vacc_start_time = 0;

//Cyclon level control
int photoResistorValue = 0;
const int lamp_pin = 11;


const int photoresistor_pin = 0;

const int lcd_rs = 8;
const int lcd_enable =9;
const int lcd_d4 = 4;
const int lcd_d5 = 5;
const int lcd_d6 = 6;
const int lcd_d7 = 7;
const int lcd_columns =16;
const int lcd_row = 2;




LiquidCrystal lcd(lcd_rs, lcd_enable, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

long get_fuel_level();
boolean box_is_empty();
boolean box_is_full();
void turn_vac_off();
void turn_light_off();
void turn_vac_on();
boolean vac_is_off();
void turn_light_on();
boolean vac_is_on();
boolean cyclon_is_full(int);
boolean vac_has_been_on_more_then(int);

//Do not add code below this line
#endif /* _suck3_H_ */
