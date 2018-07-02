#include <Wire.h>
#include <SFE_MicroOLED.h>  //OLED SFE_MicroOLED library
#include "Array.h"
#include <cmath>

//OLED set up
#define PIN_RESET 225 //OLED
#define DC_JUMPER 0  //OLED I2C Addres: 0 - 0x3C, 1 - 0x3D
MicroOLED oled(PIN_RESET, DC_JUMPER); //OLED I2C SETUP

#define ledPin 13 //builtin LED
bool ledState = false;

class Vector {
public:
	float x, y;
	float vec[] = { x,y };
	float Length() const;
	Vector operator*(float scaleup) const;
	Vector operator/(float scaledown) const;
	Vector UnitLength() const;
	Vector operator+(const Vector& vec) const;
	Vector operator-(const Vector& vec) const;
};

float Vector::Length() const {
	float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	return length;
}

Vector Vector::operator*(float scaleup) const {
	Vector ScaledVector;
	for (int i = 0; i < 2; i++) {
		ScaledVector[i] = (*this)[i] * scaleup;
	}
	return ScaledVector;
}

Vector Vector::operator/(float scaledown) const {
	Vector ScaledVector;
	if (scaledown == 0) {
	}
	else {
		for (int i = 0; i < 2; i++) {
			ScaledVector[i] = (*this)[i] / scaledown;
		}
		return ScaledVector;
	}
}

Vector Vector::UnitLength() const {
	Vector UnitLengthVector = (*this) / Length();
	return UnitLengthVector;
}

Vector Vector::operator+(const Vector& vec) const {
	Vector NewVec;

	for (int i = 0; i < 2; i++) {
		NewVec[i] = (*this)[i] + vec[i];
	}
	return NewVec;
}

Vector Vector::operator-(const Vector& vec) const {
	Vector NewVec;

	for (int i = 0; i < 2; i++) {
		NewVec[i] = (*this)[i] - vec[i];
	}
	return NewVec;
}

class Point : public std::array<float, 2> {
public:
};

Point operator+(Point OldPos, Vector Velocity) {
	Point NewPos;

	for (int i = 0; i < 2; i++) {
		NewPos[i] = OldPos[i] + Velocity[i];
	}
	return NewPos;
}


class Entity {
public:
	Vector Velocity, Gravity;
	Point Position;
	const int radius = 0;
};


void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	oled.begin();
	oled.clear(ALL);
	oled.display();
}


void loop() {
	// Flash the LED on pin 13 just to show that something is happening
	ledState = !ledState;
	if (ledState) {
		digitalWrite(ledPin, HIGH);
	}
	else {
		digitalWrite(ledPin, LOW);
	}
	Serial.println();
	delay(1000);
	draw();
}

void draw() {

}
