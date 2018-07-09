//PreProcessor Instruction, it copies and paste all the code from one directory to this translation unit.
//<> indicate that the header file is in another working directory, not in this current directory. "" does the same but checks current directory first. 
#include <iostream>
#include <cmath>

class Vector { //create a vector class. class and structs are the same except that structs are always public
public:
	//data members
	double Data[2]; //create an array allocated on the STACK
public:
	Vector(const Vector& v) { //Create a copy constructor 
		for (int i(0); i < 2; i++)
			(*this)[i] = v.Data[i];
	}

	Vector(double X, double Y) {
		Data[0] = X;
		Data[1] = Y;
	}

public:
	//member functions
	double& operator[](size_t idx) { return Data[idx]; } //operator overload ????
	double operator[](size_t idx) const { return Data[idx]; } //?????
	double Length() const;
	Vector NormalizeVector() const; //Create unit vector
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

//Using scope resolution operator to make code neater

double Vector::Length() const {
	return sqrt((*this)[0] * (this)[0])
}

Vector Vector::NormalizeVector() const {
	Vector NormVec(*this);

	for (int i(0); i < 2; i++)
		NormVec[i] /= Length();
	
	return NormVec;
}

Vector Vector::operator*(double scaleup) const {
	Vector NewVector(*this);

	for (int i(0); i < 2; i++)
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