// Do not remove the include below
#include "suck3.h"

namespace {

const int MAX_NUMBER_OF_MINUTES = 3;

}

//The setup function is called once at startup of the sketch
void setup()
{  // initialize serial communication
	  // Serial.begin(9600);

	  pinMode(triggerPin, OUTPUT);
	  pinMode(ON_BUTTON, OUTPUT);
	  pinMode(OFF_BUTTON, OUTPUT);
	  pinMode(echoPin, INPUT);
	  pinMode(lamp_pin, OUTPUT);

	  lcd.begin(lcd_columns, lcd_row);}

void loop()
{
  if(error_state){
    // Need user intervension.
  }
  else{

    fuel_level = get_fuel_level();
    photoResistorValue = analogRead(photoresistor_pin);

    lcd.setCursor(0, 0);
    lcd.print("R:     ");
    lcd.setCursor(2, 0);
    lcd.print(photoResistorValue);

    lcd.setCursor(0, 1);
    lcd.print("L:      ");
    lcd.setCursor(2, 1);
    lcd.print(fuel_level);

    //Serial.print(fuel_level);
    //Serial.print("fuel_level");
    //Serial.println();

    lcd.setCursor(8, 0);
    lcd.print("S1:     ");
    lcd.setCursor(11, 0);

    if (box_is_empty()){
      lcd.print("Empty");
      turn_light_on();
      last_major_state = EMPTY;

    }
    else if(box_is_full()){
      lcd.print("Full");
      last_major_state = FULL;
      turn_vac_off();//Just to be sure.
      turn_light_off();
    }
    else{
      lcd.print("Normal");
    }

    lcd.setCursor(8, 1);
    lcd.print("Vac:     ");
    lcd.setCursor(12, 1);

    if (last_major_state == EMPTY){
      if(cyclon_is_full(photoResistorValue)){
        lcd.print("OFF");
        turn_vac_off();
      }
      else{
        turn_vac_on();
        lcd.print("ON");
      }
    }
    delay(1000);

		if (vac_is_on()
				&& vac_has_been_on_more_then(
						MAX_NUMBER_OF_MINUTES)) {
      ;
      turn_vac_off();
      error_state = VACONTOLONG;
      // Need restart;
      //Send msg?
    }
  }
}

boolean vac_has_been_on_more_then(int max_vacc_on_in_minutes){

  unsigned long current_time = millis();
  unsigned long elapsed_time = (current_time - vacc_start_time)/1000;

  if(elapsed_time > max_vacc_on_in_minutes){
    return true;
  }

  return false;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

long get_fuel_level(){
  long duration;

  //sending the signal, starting with LOW for a clean signal
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(triggerPin, LOW);

  //setting up the input pin, and receiving the duration in
  //microseconds for the sound to bounce off the object infront

  duration = pulseIn(echoPin, HIGH);

  return microsecondsToCentimeters(duration);
}

boolean box_is_empty(){
  return fuel_level > box_empty_level;
}

boolean box_is_full(){
  return fuel_level < box_full_level;
}

boolean cyclon_is_full(int resistor_value){
  return resistor_value > CYCLON_FULL;
}

void turn_vac_on(){
  if(vac_is_off()){
    digitalWrite(ON_BUTTON, HIGH);
    delay(500);
    digitalWrite(ON_BUTTON, LOW);
    vac_state = VAC_ON;
    vacc_start_time = millis();
  }
}

void turn_vac_off(){
  if(vac_is_on()){
    digitalWrite(OFF_BUTTON, HIGH);
    delay(500);
    digitalWrite(OFF_BUTTON, LOW);
    vac_state = VAC_OFF;
  }
}

boolean vac_is_on(){
  return vac_state == VAC_ON;
}

boolean vac_is_off(){
  return vac_state == VAC_OFF;
}

void turn_light_on()
{
  digitalWrite(lamp_pin, HIGH);
}

void turn_light_off(){
  digitalWrite(lamp_pin, LOW);
}

