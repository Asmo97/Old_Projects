#include <iostream>
#include <array>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>

class Vector : public std::array<float, 3> {
	//std::array<float, 2> vec; // <- You've inherited from std::array, so Vector IS an array.
public:
	//Defualt constructor - Apparently i need it in order to create an array of objects(or instances) for the entity class?
	Vector() {}
	//Another constructor
	Vector(float X, float Y, float Z) {
		(*this)[0] = X;
		(*this)[1] = Y;
		(*this)[2] = Z;
	}
public:
	//member functions, will use the scope operator :: to make the class easy to read
	float Length() const; //Finds the length of a vector
	Vector NormalizeVector() const; //Create unit vector
	Vector operator*(float scale) const; //scale the vector
	float operator*(const Vector& vec) const; //Dot Product of two vectors
	Vector operator+(const Vector& vec) const; //add 2 vectors
	Vector operator-(const Vector& vec) const; //subtract 2 vectors
};

class Entity {
public:
	//Data Members
	Vector Position;
	Vector Velocity;
	Vector Force;
	float Mass;

public:
	//Defualt constructor - Apparently i need it in order to create an array of objects(or instances)
	Entity() {}
	//Constructor intialiser list
	Entity(const Vector& Pos, const Vector& Vel, const Vector& Frc, float M) :
		Position(Pos),
		Velocity(Vel),
		Force(Frc),
		Mass(M)
	{}
};

float Vector::Length() const {
	//There's two ways of accessing the base class. One way is to
	//dereference the this pointer, i.e. *this. the other is to call
	//them explicitly, like this std::array::operator[](0). I think
	//*this is neater
	float length = std::sqrt((*this)[0] * (*this)[0] + (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2]);
	return length;
}

Vector Vector::NormalizeVector() const {
	Vector NormVec(*this);

	for (int i(0); i < 3; i++)
		NormVec[i] /= Length();

	return NormVec;
}

Vector Vector::operator*(float scale) const {
	Vector NewVector(*this);

	for (int i(0); i < 3; i++)
		NewVector[i] *= scale;

	return NewVector;
}

float Vector::operator*(const Vector& vec) const { //Pass the vector by reference to avoid taking a copy!

	return ((*this)[0] * vec[0] + (*this)[1] * vec[1] + (*this)[2] * vec[2]);

}

Vector Vector::operator+(const Vector& vec) const { //Pass the vector by reference to avoid taking a copy!
	Vector NewVector(*this);

	for (int i(0); i < 3; i++)
		NewVector[i] += vec[i];

	return NewVector;
}

Vector Vector::operator-(const Vector& vec) const {
	Vector NewVector(*this);

	for (int i(0); i < 3; i++)
		NewVector[i] -= vec[i];

	return NewVector;
}

std::ostream& operator<<(std::ostream& stream, const Vector& vec) {
	return stream << vec[0] << "," << vec[1] << "," << vec[2] << ",";
}

class WriteData : public std::fstream {
private:
	std::ofstream outData;
public:
	WriteData(float time) {
		std::ostringstream fn;

		fn << "ParticleData" << time << ".csv";

		if (std::remove("ParticleData.csv") != 0)
			std::cout << "No existing file called ParticleData.csv, creating a new file" << std::endl;
		else
			std::cout << "Removed existing ParticleData.csv" << std::endl;

		(*this).open("ParticleData.csv", std::ios::app);
		(*this) << "Particle" << "," << "Mass" << "," << "Px" << "," << "Py" << "," << "Pz" << "," << "Vx" << "," << "Vy" << "," << "Vz" << "," << "Fx" << "," << "Fy" << "," << "Fz" << std::endl;
	}

	void WriteParticlesData(int particle, float mass, const Vector& pos, const Vector& vel, const Vector& force) {
		(*this) << particle << "," << mass << "," << pos << vel << force << std::endl;
	}
	~WriteData() {
		(*this).close();
	}
};

int main() {
	float time;
	float MAX_TIME = 10.00f;
	float dt = 1.00f;

	//Create an array of Particle Obj on the Stack!
	const unsigned int NumOfParticles = 1;
	Entity Particle[NumOfParticles];

	Vector Position(0, 0, 0);  //On ground initially
	Vector Velocity(0, 10, 0); //Particle thrown straight up 
	Vector Force(0, -2, 0); //gravity force

	Particle[0] = Entity(Position, Velocity, Force, 20); //Give particel object some attributes via entity constructor



	for (time = 0; time < MAX_TIME; time += dt) {
		WriteData ParticlesData(time);

		for (int i(0); i < NumOfParticles; i++) {
			std::cout << "Particle " << i << " Position:" << Particle[i].Position << " Veclocity:" << Particle[i].Velocity << " Force:" << Particle[i].Force << std::endl;
			//Standard Euler
			Particle[i].Position = Particle[i].Position + Particle[i].Velocity * dt;
			Particle[i].Velocity = Particle[i].Velocity + Particle[i].Force * dt;

			ParticlesData.WriteParticlesData(i, Particle[i].Mass, Particle[i].Position, Particle[i].Velocity, Particle[i].Force);
		}

	}

	std::cin.get();
	return 0;
}

