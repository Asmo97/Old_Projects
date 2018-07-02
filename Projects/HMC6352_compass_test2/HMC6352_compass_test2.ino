#include <Wire.h>
#include <SFE_MicroOLED.h>  //OLED SFE_MicroOLED library

#define PIN_RESET 225 //OLED
#define DC_JUMPER 0  //OLED I2C Addres: 0 - 0x3C, 1 - 0x3D
MicroOLED oled(PIN_RESET, DC_JUMPER); //OLED I2C SETUP


int compassAddress = 0x42 >> 1; // From datasheet compass address is 0x42
// shift the address 1 bit right, the Wire library only needs the 7
// most significant bits for the address
int reading = 0; 
int ledPin = 13;
bool ledState = false;

void setup() 
{ 
  Wire.begin();       // join i2c bus (address optional for master) 
  Serial.begin(9600); // start serial communication at 9600bps 
  pinMode(ledPin, OUTPUT);
  oled.begin();
  oled.clear(ALL);
  oled.display();
} 

void loop() 
{ 
  // Flash the LED on pin 13 just to show that something is happening Also serves as an indication that we're not "stuck"
  ledState = !ledState;
  if (ledState) {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
  // step 1: instruct sensor to read echoes 
  Wire.beginTransmission(compassAddress);  // transmit to device
  // the address specified in the datasheet is 66 (0x42) 
  // but i2c adressing uses the high 7 bits so it's 33 
  Wire.write('A');        // command sensor to measure angle  
  Wire.endTransmission(); // stop transmitting 

  // step 2: wait for readings to happen 
  delay(10); // datasheet suggests at least 6000 microseconds 

  // step 3: request reading from sensor 
  Wire.requestFrom(compassAddress, 2); // request 2 bytes from slave device #33 

  // step 4: receive reading from sensor 
  if (2 <= Wire.available()) // if two bytes were received 
  { 
    reading = Wire.read();  // receive high byte (overwrites previous reading) 
    reading = reading << 8; // shift high byte to be high 8 bits 
    reading += Wire.read(); // receive low byte as lower 8 bits 
    reading /= 10;
    Serial.println(reading); // print the reading
  } 

  delay(500); // wait for half a second
}
