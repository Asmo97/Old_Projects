//PreProcessor Instruction, it copies and paste all the code from one directory to this translation unit.
//<> indicate that the header file is in another working directory, not in this current directory. "" does the same but checks current directory first. 
#include <iostream>
#include <cmath>

class Vector { //create a vector class. class and structs are the same except that structs are always public
public:
	//data members
	double Data[2]; //create an array allocated on the STACK
public:
	Vector() {}
	Vector(const Vector& v) { //Create a copy constructor 
		for (int i(0); i < 2; i++)
			(*this)[i] = v.Data[i];
	}

	Vector(double X, double Y) { //another constructor for easy input
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
	Vector operator+(const Vector& vec) const; //add 2 vectors
	Vector operator-(const Vector& vec) const; //subtract 2 vectors
	Vector CreateTangetVector() const; //Create a perpendicular vector

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
	Entity() {}
	//Constructor intialiser list
	Entity(const Vector& Pos, const Vector& Vel, const Vector& Acc, double Rad, double M) :
		Position(Pos),
		Velocity(Vel),
		Acceleration(Acc),
		Radius(Rad),
		Mass(M) {}

<<<<<<< HEAD
=======

>>>>>>> ff9946af022283682c8561dd55b5f7606c45424b
	//void update() {
	//	//this member function is responsible for frame rate independance
	//	this->Position = (this->Position) + ((this->Velocity) * (dt / 1000)); //Note: dt is in milli seconds
	//	this->Velocity = this->Velocity + (this->Acceleration * (dt / 1000)); //not in use this for this simulation
	//}
	//void KeepInBorders() {
	//	//Prevent the ball from going out of bounds fully elastic collision 
	//	if (this->Position[1] - this->Radius <= 0 || this->Position[1] + this->Radius >= SCREEN_HEIGHT) {
	//		this->Velocity[1] = this->Velocity[1] * -1;
	//	}

	//	if (this->Position[0] - this->Radius <= 0 || this->Position[0] + this->Radius >= SCREEN_WIDTH) {
	//		this->Velocity[0] = this->Velocity[0] * -1;
	//	}
	//}
};

//------------------------------------------------------------------------------------------------------------
//Using scope resolution operator to make code neater
double Vector::Length() const {
	return sqrt((this->Data[0] * this->Data[0]) + (this->Data[1] * this->Data[1]));
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

Vector Vector::operator+(const Vector& vec) const { //Pass the vector by reference to avoid taking a copy!
	Vector NewVector(*this);
<<<<<<< HEAD

	for (int i(0); i < 2; i++)
		NewVector[i] += vec[i];

	return NewVector;
}

Vector Vector::operator-(const Vector& vec) const {
	Vector NewVector(*this);

	for (int i(0); i < 2; i++)
		NewVector[i] -= vec[i];

	return NewVector;
}

Vector Vector::CreateTangetVector() const {
	Vector tangent(*this);
	tangent[0] = -1 * this->Data[1];
	tangent[1] = this->Data[0];
	return tangent;
}

int main() {
	Entity Ball[2];
=======

	for (int i(0); i < 2; i++)
		NewVector[i] += vec[i];
>>>>>>> ff9946af022283682c8561dd55b5f7606c45424b

	return NewVector;
}

Vector Vector::operator-(const Vector& vec) const {
	Vector NewVector(*this);

	for (int i(0); i < 2; i++)
		NewVector[i] -= vec[i];

	return NewVector;
}

Vector Vector::CreateTangetVector() const {
	Vector tangent(*this);
	tangent[0] = -1 * this->Data[1];
	tangent[1] = this->Data[0];
	return tangent;
}

int main() {
	Entity Ball[2];
	Vector P(5, 5);
	Vector V(1, 2);
	Vector A(0, 0);
	Ball[0] = Entity(P, V, A, 5, 20);

<<<<<<< HEAD
	Ball.Position = Ball[0].Position + Ball[0].Velocity;
=======
	Ball[0].Position = Ball[0].Position + Ball[0].Velocity;
>>>>>>> ff9946af022283682c8561dd55b5f7606c45424b
	std::cout << Ball[0].Position[1] << std::endl;
	std::cin.get();
}
