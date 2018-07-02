/* Controlling RGB LED with the Teensy 3.6, Part 1 -- Basic Digital inputs */

#define LED_green_pin 3 //Digital PWM Pin
#define LED_blue_pin 4 //Digital PWM Pin
#define LED_red_pin 2 //Digital PWM Pin

/*
const int LED_green_pin = 3; //PWM Pin
const int LED_blue_pin = 4; //PWM Pin
const int LED_read_pin = 2; //PWM Pin
*/

void setup() {
  pinMode(LED_green_pin, OUTPUT);
  pinMode(LED_blue_pin, OUTPUT);
  pinMode(LED_red_pin, OUTPUT);
}

void loop() {
  digitalWrite(LED_green_pin, HIGH); //LED on
  delay(500);
  digitalWrite(LED_green_pin, LOW); //LED off

  digitalWrite(LED_blue_pin, HIGH); //LED on
  delay(500);
  digitalWrite(LED_blue_pin, LOW); //LED off

  digitalWrite(LED_red_pin, HIGH); //LED on
  delay(500);
  digitalWrite(LED_red_pin, LOW); //LED off
}
