// Do not remove the include below
#include "suck3.h"

//The setup function is called once at startup of the sketch
void setup() {  // initialize serial communication
//Serial.begin(9600);

	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(VAC_ON_BUTTON_PIN, OUTPUT);
	pinMode(VAC_OFF_BUTTON_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
	pinMode(LAMP_PIN, OUTPUT);
	pinMode(PHOTORESISTOR_PIN, INPUT);

	lcd.begin(lcd_columns, lcd_row);
}

static Event get_next_event() {

	photoResistorValue = analogRead(PHOTORESISTOR_PIN);
	fuel_level = get_fuel_level();

	switch (state) {
	case ST_NORMAL:
		if (box_is_empty()) {
			return EV_BOX_EMPTY;
		}
		return EV_NO_EVENT;
		break;
	case ST_CYCLONE_FILLING:
		if (vac_has_been_on_more_then(MAX_NUMBER_OF_SECONDS)) {
			return EV_VAC_TO_LONG;
		}

		if (box_is_full()) {
			return EV_BOX_FULL;
		}

		if (cyclone_is_full()) {
			return EV_CYCLONE_FULL;
		}

		return EV_NO_EVENT;
		break;
	case ST_CYCLONE_EMPTENING:
		if (box_is_full()) {
			return EV_BOX_FULL;
		}

		if (cyclone_is_empty()) {
			return EV_CYCLONE_EMPTY;
		}

		return EV_NO_EVENT;
		break;
	default:
		return EV_NO_EVENT;
	}
}

static State box_empty() {
	turn_vac_on();
	return ST_CYCLONE_FILLING;
}

static State box_full() {
	turn_vac_off();
	return ST_NORMAL;
}

static State cycone_full() {
	turn_vac_off();
	return ST_CYCLONE_EMPTENING;
}

static State cyclone_empty() {
	turn_vac_on();
	return ST_CYCLONE_FILLING;
}

static State vac_to_long(){
	turn_vac_off();
	return ST_ERR;
}

boolean vac_has_been_on_more_then(int max_vacc_on_in_seconds) {

	unsigned long current_time = millis();
	unsigned long elapsed_time = (current_time - vacc_start_time) / 1000;

	if (elapsed_time > max_vacc_on_in_seconds) {
		return true;
	}

	return false;
}

long microsecondsToCentimeters(long microseconds) {
	return microseconds / 29 / 2;
}

long get_fuel_level() {
	long duration;

	//sending the signal, starting with LOW for a clean signal
	digitalWrite(TRIGGER_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(TRIGGER_PIN, LOW);

	//setting up the input pin, and receiving the duration in
	//microseconds for the sound to bounce off the object infront

	duration = pulseIn(ECHO_PIN, HIGH);

	return microsecondsToCentimeters(duration);
}

boolean box_is_empty() {
	return fuel_level > BOX_EMPTY_LEVEL;
}

boolean box_is_full() {
	return fuel_level < BOX_FULL_LEVEL;
}

boolean cyclone_is_full() {
	return photoResistorValue > CYCLONE_FULL;
}

boolean cyclone_is_empty() {
	return photoResistorValue < CYCLONE_FULL;
}

void turn_vac_on() {
	if (vac_is_off()) {
		digitalWrite(VAC_ON_BUTTON_PIN, HIGH);
		delay(500);
		digitalWrite(VAC_ON_BUTTON_PIN, LOW);
		vac_state = VAC_ON;
		vacc_start_time = millis();
	}
}

void turn_vac_off() {
	if (vac_is_on()) {
		digitalWrite(VAC_OFF_BUTTON_PIN, HIGH);
		delay(500);
		digitalWrite(VAC_OFF_BUTTON_PIN, LOW);
		vac_state = VAC_OFF;
	}
}

boolean vac_is_on() {
	return vac_state == VAC_ON;
}

boolean vac_is_off() {
	return vac_state == VAC_OFF;
}

//void turn_light_on() {
//	digitalWrite(LAMP_PIN, HIGH);
//}
//
//void turn_light_off() {
//	digitalWrite(LAMP_PIN, LOW);
//}

static String get_state_string() {
	switch (state) {
	case ST_CYCLONE_EMPTENING:
		return "Emptening Cyclo";
		break;
	case ST_CYCLONE_FILLING:
		return "Filling Cyclone";
		break;
	case ST_NORMAL:
		return "Normal";
		break;
	default:
		return "Err";
	}
}

void updateLCD() {
	if (state != ST_ERR) {
	lcd.setCursor(0, 0);
	lcd.print("R:     ");
	lcd.setCursor(2, 0);
	lcd.print(photoResistorValue);

	lcd.setCursor(7, 0);
	lcd.print("L:     ");
	lcd.setCursor(9, 0);
	lcd.print(fuel_level);

	lcd.setCursor(13, 0);
	lcd.print("V:    ");
	lcd.setCursor(15, 0);
	lcd.print(vac_state);

	lcd.setCursor(0, 1);
	lcd.print("S:               ");
	lcd.setCursor(2, 1);
	lcd.print(get_state_string());
	}else{
		lcd.setCursor(0, 0);
		lcd.print("ERR:Vac on long.");
		lcd.setCursor(0, 1);
		lcd.print("Max time(sec):");
		lcd.print(MAX_NUMBER_OF_SECONDS);

	}

//	Serial.print("photoResistorValue:");
//	Serial.print(photoResistorValue);
//	Serial.print(" ");
//	Serial.print("fuel_level:");
//	Serial.print(fuel_level);
//	Serial.print(" ");
//	Serial.print("state:");
//	Serial.print(get_state_string());
//	Serial.println();

}

void loop() {

	int i;

	state = ST_NORMAL;
	while (state != ST_TERM) {
		delay(1000);
		event = get_next_event();
		for (i = 0; i < TRANS_COUNT; i++) {
			if ((state == trans[i].state) || (ST_ANY == trans[i].state)) {
				if ((event == trans[i].event) || (EV_ANY == trans[i].event)) {
					state = (trans[i].fn)();
					break;
				}
			}
		}
		updateLCD();
	}
}
