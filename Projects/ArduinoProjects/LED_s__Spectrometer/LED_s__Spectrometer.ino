#include <cstdint> //n C++11, the C99 header file stdint.h has been included as cstdint 


//RGB LED controll Variables - Preporcessor definitions
#define LED_UV_pin 16 
#define LED_red_pin 23 //Analog PWM pin
#define LED_green_pin 22 //Analog PWM Pin
#define LED_blue_pin 21 //Analog PWM Pin
#define Button_pin 3 //Digital PWM Pin

enum LED : std::uint8_t { UV = 1, RED, BLUE, GREEN }; //get completely known sizes for your enum across compilers 
//Refernce https://www.cprogramming.com/c++11/c++11-nullptr-strongly-typed-enum-class.html

LED operator++(LED& led, int) { // int denotes postfix++
  switch (led) {
  case UV: led = RED; break;
  case RED: led = BLUE; break;
  case BLUE: led = GREEN; break;
  case GREEN: led = UV; break;
  } 
  return led;
  //  if (led == GREEN) return led = UV;
  //  std::uint8_t x = led;
  //  return led = static_cast<LED> (++x); //<-- not sure what this line is doing completely
} //REFERNCE http://carsonbaker.org/looping-through-cpp-enums

LED led_state = UV;

void setup() {
  Serial.begin(9600);
  //intialise pins
  pinMode(LED_green_pin, OUTPUT);
  pinMode(LED_blue_pin, OUTPUT);
  pinMode(LED_red_pin, OUTPUT);
  pinMode(Button_pin, INPUT);
}

void loop() {

  switch (led_state) {
  case UV: analogWrite(LED_UV_pin, 255); break;

  case RED: analogWrite(LED_red_pin, 255); break;

  case BLUE: analogWrite(LED_blue_pin, 255); break;

  case GREEN: analogWrite(LED_green_pin, 255); break;
  }

  if (digitalRead(Button_pin) == HIGH) {
    delay(1000); //debounces and also prevents digitalRead happening again too quickly
    led_state++;
  }
  Serial.print(digitalRead(Button_pin));
}
