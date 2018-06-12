/* Controlling RGB LED with the Teensy 3.6, Part 2 -- Basic Analog inputs */

#define LED_red_pin A9 //Analog PWM Pin
#define LED_green_pin A8 //Analog PWM Pin
#define LED_blue_pin A7 //Analog PWM Pin

/*
const int LED_red_pin = 23; //PWM Pin
const int LED_green_pin = 22; //PWM Pin
const int LED_blue_pin = 21; //PWM Pin
*/

//2^8 bit color operation
uint8_t red_level = 100;
uint8_t green_level = 100;
uint8_t blue_level = 100;

void setup() {
  pinMode(LED_green_pin, OUTPUT);
  pinMode(LED_blue_pin, OUTPUT);
  pinMode(LED_red_pin, OUTPUT);
}

void loop() {
  analogWrite(LED_red_pin, red_level); 
  
  analogWrite(LED_green_pin, green_level); 

  analogWrite(LED_blue_pin, blue_level);
}
