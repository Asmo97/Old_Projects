#include <cstdint>

#define ledPin 13 //builtin LED
#define MOSFET_Driver_PWM_Pin 21
#define MOSFET_Pin 14

enum State : std::uint8_t { OFFLINE = 1, TRANSMIT, LISTEN, LISTEN_TRANSMIT};
State Transducer = OFFLINE;

char rx_byte;

IntervalTimer cycle_timer;

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
    }else if ((rx_byte == '3')) {
      Serial.println("Transducer Listen and Transmit");
      Transducer = LISTEN_TRASNMIT;
    }else {
      Serial.print(rx_byte);
      Serial.println(" is not a valid command.");
    }
  }

  switch(Transducer){
    case OFFLINE: digitalWrite(ledPin, LOW); digitalWrite(MOSFET_Driver_PWM_Pin, LOW); digitalWrite(MOSFET_Pin, LOW); break;

    case TRANSMIT: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Pin, HIGH); Generate_Burst(); break;

    case LISTEN: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Driver_PWM_Pin, LOW); digitalWrite(MOSFET_Pin, LOW); break;

    case LISTEN_TRASNMIT: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Driver_PWM_Pin, HIGH); digitalWrite(MOSFET_Pin, LOW); break;
    
  }
}

unsigned int period = 10;
volatile int cycles = 5;
volatile int cycle_counter = 0;
volatile bool state = HIGH;

void Generate_Burst(){ //period in ms
  elapsedMillis timer;

  while(timer <= period){
    digitalWrite(MOSFET_Driver_PWM_Pin, state);
  }

  cycle_timer.begin(Generate_Cycles, 12.5); //transition every 12.5 micro seconds, 1/25e-6 = 40kHz
}

void Generate_Cycles(){
  digitalWrite(MOSFET_Driver_PWM_Pin, !state);
  cycle_counter++;

  if (cycle_counter > (cycles  * 2)){
    cycle_timer.end();
  }
}

