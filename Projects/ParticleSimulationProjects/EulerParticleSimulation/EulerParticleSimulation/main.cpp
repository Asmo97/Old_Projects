#define _USE_MATH_DEFINES // for C++

#include <iostream>
#include <array>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <cstdint>

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
	Vector Acceleration;
	float Radius;
	float Density;
	float Volume = (4/3) * (float)M_PI * powf(Radius,3.00f); //Casting for pi
	//float Mass = Density * Volume;
	float Mass;

public:
	//Defualt constructor - Apparently i need it in order to create an array of objects(or instances)
	Entity() {}
	//Constructor intialiser list
	Entity(const Vector& Pos, const Vector& Vel, const Vector& Acc, float rad, float m) :
		Position(Pos),
		Velocity(Vel),
		Acceleration(Acc),
		Radius(rad),
		Mass(m)
		//Density(den)
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
	WriteData(std::uint64_t t) {
		std::string filename = std::string("ParticleData") + std::to_string(t) + std::string(".csv");
		if (std::remove(filename.c_str()) != 0)
			std::cout << "No existing file called " + filename + ", creating a new file" << std::endl;
		else
			std::cout << "Removed existing " + filename << std::endl;

		(*this).open(filename, std::ios::app);
		(*this) << "#" << "," << "R" << "," << "Px" << "," << "Py" << "," << "Pz" << "," << "Vx" << "," << "Vy" << "," << "Vz" << "," << "Fx" << "," << "Fy" << "," << "Fz" << std::endl;
	}

	void WriteParticlesData(int particle, float rad, const Vector& pos, const Vector& vel, const Vector& force) {
		(*this) << particle << "," << rad << "," << pos << vel << force << std::endl;
	}
	~WriteData() {
		(*this).close();
	}
};

int main() {
	float time;
	float MAX_TIME = 10.00f;
	float dt = 1.00f/60.00f;
	std::uint64_t file_counter = 0;

	//Create an array of Particle Obj on the Stack
	const unsigned int NumOfParticles = 2; //<---- WATCH OUT FOR THIS WHEN CREATING ANOTHER PARTICLE
	Entity Particle[NumOfParticles];

	Vector SunPosition(0, 0, 0); 
	Vector SunVelocity(0, 0, 0); 
	Vector SunAcceleration(0, 0, 0); 

	Vector PlanetPosition(2, 0, 0);
	Vector PlanetVelocity(0, 1, 0);
	Vector PlanetAcceleration(0, 0, 0);

	//Give particel object some attributes via entity constructor
	Particle[0] = Entity(SunPosition, SunVelocity, SunAcceleration, 10.00f, 10.00f); //the sun
	Particle[1] = Entity(PlanetPosition, PlanetVelocity, PlanetAcceleration, 1.00f, 1.00f); //the planet (satalite)


	for (time = 0.00f; time < MAX_TIME; time += dt) {

		WriteData ParticlesData(file_counter);

		for (int i(0); i < NumOfParticles; i++) {
			//write data to file
			ParticlesData.WriteParticlesData(i, Particle[i].Radius, Particle[i].Position, Particle[i].Velocity, Particle[i].Acceleration);
			std::cout << "Particle " << i << " Position:" << Particle[i].Position << " Veclocity:" << Particle[i].Velocity << " Force:" << Particle[i].Acceleration << std::endl;
			
			if (i != 0) { //we dont want to update the suns attributes 

				//explicit Euler
				Particle[i].Position = Particle[i].Position + Particle[i].Velocity * dt;
				Particle[i].Velocity = Particle[i].Velocity + Particle[i].Acceleration * dt;

				//semi-implitcit Euler
				//Particle[i].Velocity = Particle[i].Velocity + Particle[i].Acceleration * dt;
				//Particle[i].Position = Particle[i].Position + Particle[i].Velocity * dt;

				//Evaluate newtons law of grivatation (IN VECTOR FORM) https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
				float G = 1;
				Vector r = Particle[0].Position - Particle[i].Position;
				Particle[i].Acceleration = r.NormalizeVector() * ((G * Particle[0].Mass * Particle[i].Mass) / (abs(r * r)));


			}
			else {
				continue;
			}

		}

		file_counter++;

	}

	std::cin.get();
	return 0;
}
