#include <Wire.h>
#include <SFE_MicroOLED.h>  //OLED SFE_MicroOLED library

#define PIN_RESET 225 //OLED
#define DC_JUMPER 0  //OLED I2C Addres: 0 - 0x3C, 1 - 0x3D
MicroOLED oled(PIN_RESET, DC_JUMPER); //OLED I2C SETUP


int HMC6352Address = 0x42; //COMPAS
// This is calculated in the setup() function 
int slaveAddress;
int ledPin = 13;
boolean ledState = false;
byte headingData[2];
int i, headingValue;

void setup()
{
// Shift the device's documented slave address (0x42) 1 bit right
// This compensates for how the TWI library only wants the
// 7 most significant bits (with the high bit padded with 0)
slaveAddress = HMC6352Address >> 1;   // This results in 0x21 as the address to pass to TWI
Serial.begin(9600);
pinMode(ledPin, OUTPUT);      // Set the LED pin as output
Wire.begin();

  oled.begin();
  oled.clear(ALL);
  oled.display();
}
void loop()
{
  // Flash the LED on pin 13 just to show that something is happening
  // Also serves as an indication that we're not "stuck" waiting for TWI data
  ledState = !ledState;
  if (ledState) {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
  // Send a "A" command to the HMC6352
  // This requests the current heading data
  Wire.beginTransmission(slaveAddress);
  Wire.send("A");              // The "Get Data" command
  Wire.endTransmission();
  delay(10);                   // The HMC6352 needs at least a 70us (microsecond) delay
  // after this command.  Using 10ms just makes it safe
  // Read the 2 heading bytes, MSB first
  // The resulting 16bit word is the compass heading in 10th's of a degree
  // For example: a heading of 1345 would be 134.5 degrees
  Wire.requestFrom(slaveAddress, 2);        // Request the 2 byte heading (MSB comes first)
  i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.receive();
    i++;
  }
  headingValue = headingData[0]*256 + headingData[1];  // Put the MSB and LSB together
  Serial.print("Current heading: ");
  Serial.print(int (headingValue / 10));     // The whole number part of the heading
  Serial.print(".");
  Serial.print(int (headingValue % 10));     // The fractional part of the heading
  Serial.println(" degrees");
  delay(500);
} 

