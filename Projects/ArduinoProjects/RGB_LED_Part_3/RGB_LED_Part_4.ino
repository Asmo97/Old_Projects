/* Controlling RGB LED with the Teensy 3.6, Part 4 -- Basic Analog inputs with potentiometer and processing*/

//RGB LED controll Variables
#define LED_red_pin 23 //Analog PWM RGB pin
#define LED_green_pin 22 //Analog PWM RGB Pin
#define LED_blue_pin 21 //Analog PWM RGB Pin
#define Pot_pin 14 //Analog PWM potentiometer Pin
#define Button_pin 10 //Digital PWM Pin

//Normal LED controll Variables
#define red_pin 38
#define green_pin 37 
#define blue_pin 36 

//8 bit color operation
uint8_t red_level = 0;
uint8_t green_level = 0;
uint8_t blue_level = 0;
volatile char selected_color = 'r'; //start on red

void setup() {
  Serial.begin(9600);
  pinMode(LED_green_pin, OUTPUT);
  pinMode(LED_blue_pin, OUTPUT);
  pinMode(LED_red_pin, OUTPUT);
  pinMode(Pot_pin, INPUT);
  pinMode(Button_pin, INPUT);

  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);

  //attach an interupt that detects a change from 0 to 1 (RISING) from the digital button pin and excutes the function called swap.
  attachInterrupt(Button_pin, swapp, RISING);
}

void swapp(){
  //this function will cause the selected_pin to hop to RED, GREEN or BLUE pins if the button is pressed. 
  //Serial.write('S'); //write to processing that the led has swapped
  switch(selected_color){
    case 'r': //Red pin
      selected_color = 'g';
      break;
    case 'g': //Green pin
      selected_color = 'b';
      break;
    case 'b': //blue pin
      selected_color = 'r';
      break;
  }
}

void loop() {
  int pot_input = analogRead(Pot_pin); 
  pot_input = map(pot_input, 0, 1023, 0, 255);

  if (selected_color == 'r'){
    
    red_level = constrain(pot_input, 0, 255);
    
  }else if (selected_color == 'g'){
    
    green_level = constrain(pot_input, 0, 255);
    
  }else if(selected_color == 'b'){
    
    blue_level = constrain(pot_input, 0, 255);
    
  }

  //write to RGB LED
  analogWrite(LED_red_pin, red_level);
  analogWrite(LED_green_pin, green_level);
  analogWrite(LED_blue_pin, blue_level);
  
  
  //Serial.write(selected_level); //write to processing the current level

  //Write to single normal LED's. Their brightness determines thier level
  analogWrite(red_pin, red_level);
  analogWrite(green_pin, green_level);
  analogWrite(blue_pin, blue_level);
  Serial.println(red_level);
}
