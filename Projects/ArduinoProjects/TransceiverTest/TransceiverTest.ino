#include <cstdint>

#define ledPin 13 //builtin LED
#define MOSFET_Driver_Pin 1
#define MOSFET_Pin 14

enum State : std::uint8_t { OFFLINE = 1, TRANSMIT, LISTEN};
State Transducer = OFFLINE;

char rx_byte;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(MOSFET_Driver_Pin, OUTPUT);
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
    case OFFLINE: digitalWrite(ledPin, LOW); break;

    case TRANSMIT: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Driver_Pin, HIGH); analogWrite(MOSFET_Pin, 255); break;

    case LISTEN: digitalWrite(ledPin, HIGH); digitalWrite(MOSFET_Driver_Pin, LOW); analogWrite(MOSFET_Pin, 0); break;
    
  }
}

