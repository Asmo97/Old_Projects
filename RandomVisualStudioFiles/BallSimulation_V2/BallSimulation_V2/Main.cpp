//PreProcessor Instruction, it copies and paste all the code from one directory to this translation unit.
//<> indicate that the header file is in another working directory, not in this current directory. "" does the same but checks current directory first. 
#include <iostream>
#include <cmath>

class Vector { //create a vector class. class and structs are the same except that structs are always public
public:
	//data members
	static const int dimensions = 2; //Const- this variable can not be accssed by a method. Static - not sure why, but makes the array recognise the variable
	double Data[dimensions]; //create an array allocated on the STACK
public:
	Vector(const Vector& v) { //Create a copy constructor 
		for (int i(0); i < dimensions; i++)
			(*this)[i] = v.Data[i];
	}

	Vector(double X, double Y) {
		Data[0] = X;
		Data[1] = Y;
	}

public:
	//member functions
	double& operator[](size_t idx) { return Data[idx]; } //operator overload
	double operator[](size_t idx) const { return Data[idx]; } //?????
	Vector operator*(double scaleup) const;
};

class Entity {
public:
	//Data Members
	Vector Position;
	Vector Velocity;
	Vector Acceleration;
	double Radius;
	double Mass;

public:
	//Constructor intialiser list
	Entity(const Vector& Pos, const Vector& Vel, const Vector& Acc, double Rad, double M) :
		Position(Pos),
		Velocity(Vel),
		Acceleration(Acc),
		Radius(Rad),
		Mass(M) {}
};

Vector Vector::operator*(double scaleup) const { //Using scope resolution operator to make code neater
	Vector NewVector(*this);

	for (int i(0); i < dimensions; i++)
		NewVector[i] *= scaleup;

	return NewVector;
}

int main() {

	Vector P(5, 5);
	Vector V(1, 1);
	Vector A(0, 0);
	Entity Ball(P, V, A, 5, 20);

	Ball.Position = Ball.Position * 4;
	std::cout << Ball.Position[1]<< std::endl;
	std::cin.get();
}