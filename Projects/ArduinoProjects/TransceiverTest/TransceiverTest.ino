#include <cstdint>

#define ledPin 13 //builtin LED
#define MOSFET_Driver_PWM_Pin 21
#define MOSFET_Pin 14

enum State : std::uint8_t { OFFLINE = 1, TRANSMIT, LISTEN};
State Transducer = OFFLINE;

char rx_byte;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(MOSFET_Driver_PWM_Pin, OUTPUT);
  pinMode(MOSFET_Pin, OUTPUT);
}

void loop() {
  
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    Serial.clear();                // stop other data coming
    
    // check if a number was received
    if ((rx_byte == '0')) {
      Serial.println("Transducer offline");
      Transducer = OFFLINE;
    }else if ((rx_byte == '1')) {
      Serial.println("Transducer Transmit");
      Transducer = TRANSMIT;
    }else if ((rx_byte == '2')) {
      Serial.println("Transducer Listen");
      Transducer = LISTEN;
    }else {
      Serial.print(rx_byte);
      Serial.println(" is not a valid command.");
    }
  }

  switch(Transducer){
    case OFFLINE: digitalWrite(ledPin, LOW); digitalWrite(MOSFET_Driver_PWM_Pin, LOW); digitalWrite(MOSFET_Pin, LOW); break;

    case TRANSMIT: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Pin, HIGH); Generate_Burst(10, 5, 15); break;

    case LISTEN: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Driver_PWM_Pin, LOW); digitalWrite(MOSFET_Pin, LOW); break;
    
  }
}

void Generate_Burst(int period, int cycles, int cycle_period){ //period in ms, cycle period in us
  elapsedMillis timer;

  while(timer <= period){
    digitalWrite(MOSFET_Driver_PWM_Pin, HIGH);
  }

  //-------Now create the cycles--------

  int current_cycles = 0;
  elapsedMicros cycle_timer;
  
  while (current_cycles < cycles){
    while (cycle_timer <= cycle_period) {
      digitalWrite(MOSFET_Driver_PWM_Pin, LOW);
    }
    
    while (cycle_timer > cycle_period && cycle_timer <= (cycle_period*2) ) {
      digitalWrite(MOSFET_Driver_PWM_Pin, HIGH);
    }
  
    current_cycles++;
    cycle_timer = 0;
  }
  
  timer = 0;
}

