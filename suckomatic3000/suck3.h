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

typedef struct {
	int state;
	int event;
	int (*fn)(void);
} tTransition;

#define ST_ANY -1
#define ST_NORMAL  0
#define ST_CYCLONE_FILLING  1
#define ST_CYCLONE_EMPTENING 2
#define ST_TERM 3

int state;
int event;

#define EV_ANY              -1
#define EV_CYCLONE_FULL		5000
#define EV_CYCLONE_EMPTY		5001
#define EV_BOX_FULL			5002
#define EV_BOX_EMPTY		5003
#define EV_NO_EVENT 5004

//Events
static int box_empty(void);
static int cycone_full(void);
static int cyclone_empty(void);
static int box_full(void);
static int self_trans(void){return state;};

//Helpers
static int get_next_event(void);
static void updateLCD(void);
static String get_state_string(void);

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

tTransition trans[] = {
		{ ST_NORMAL, EV_BOX_EMPTY, &box_empty },
		{ ST_CYCLONE_FILLING, EV_BOX_FULL, &box_full },
		{ ST_CYCLONE_FILLING, EV_CYCLONE_FULL, &cycone_full },
		{ ST_CYCLONE_FILLING, EV_NO_EVENT, &self_trans },
		{ ST_CYCLONE_EMPTENING, EV_CYCLONE_EMPTY, &cyclone_empty },
		{ ST_CYCLONE_EMPTENING, EV_BOX_EMPTY, &box_full },
		{ ST_CYCLONE_EMPTENING, EV_NO_EVENT, &self_trans },

};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))


//Levels
const int BOX_EMPTY_LEVEL = 65;
const int BOX_FULL_LEVEL = 45;
const int CYCLONE_FULL = 600;

//Digital pins
const int ZERO_PIN = 0;
const int LAMP_PIN = 1;
const int VAC_ON_BUTTON_PIN = 2;
const int VAC_OFF_BUTTON_PIN = 3;
const int ECHO_PIN = 12;
const int TRIGGER_PIN = 13;

//Analog pins
const int PHOTORESISTOR_PIN = 1;

long fuel_level = 0;
int photoResistorValue = 0;

const int VAC_OFF = 0;
const int VAC_ON = 1;

//States
//const int EMPTY = 1;
//const int FULL = 0;
//int last_major_state = EMPTY;
//const int CLEAR = 0;
//const int VACONTOLONG = 1;
//
//int error_state = CLEAR;

//Vac control

int vac_state = VAC_OFF;

unsigned long vacc_start_time = 0;


//LCD
const int lcd_rs = 8;
const int lcd_enable = 9;
const int lcd_d4 = 4;
const int lcd_d5 = 5;
const int lcd_d6 = 6;
const int lcd_d7 = 7;
const int lcd_columns = 16;
const int lcd_row = 2;


LiquidCrystal lcd(lcd_rs, lcd_enable, lcd_d4, lcd_d5, lcd_d6, lcd_d7);



//Do not add code below this line
#endif /* _suck3_H_ */
