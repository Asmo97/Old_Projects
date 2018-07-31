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
	const char* Name;
	Vector Position;
	Vector Velocity;
	Vector Acceleration;
	float Radius;
	float Density;
	float Volume = (4 / 3) * (float)M_PI * powf(Radius, 3.00f); //Casting for pi
	//float Mass = Density * Volume;
	float Mass;
	float KeneticE = 0;
	float PotentialE = 0;
	float TotalE = 0;

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

	void CreatePlanet(const char* planet, float Px, float Py, float Pz, float Vx, float Vy, float Vz, float r, float m) {
		Vector Pos(Px, Py, Pz);
		Vector Vel(Vx, Vy, Vz);
		Vector Acc(0, 0, 0);

		this->Name = planet;
		this->Position = Pos;
		this->Velocity = Vel;
		this->Acceleration = Acc;
		this->Radius = r;
		this->Mass = m;
	}
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

class WriteParaviewData : public std::fstream {
private:
	std::ofstream outParaviewData;
public:
	//Paraview Data
	WriteParaviewData(std::uint64_t t) {
		std::string filename = std::string("ParaviewParticleData") + std::to_string(t) + std::string(".csv");

		if (std::remove(filename.c_str()) != 0)
			std::cout << "No existing file called " + filename + ", creating a new file" << std::endl;
		else
			std::cout << "Removed existing " + filename << std::endl;

		(*this).open(filename, std::ios::app);
		(*this) << "#" << "," << "R" << "," << "Px" << "," << "Py" << "," << "Pz" << std::endl;
	}

	void WriteParticlesData(int particle, float rad, const Vector& pos, const Vector& vel, const Vector& acc) {
		(*this) << particle << "," << rad << "," << pos << std::endl;
	}

	~WriteParaviewData() {
		(*this).close();
	}
};

class WritePythonData : public std::fstream {
private:
	std::ofstream outPythonData;
public:
	WritePythonData(int TotalParticals) {
		for (int i(0); i < TotalParticals; i++) {
			std::string filename = std::string("PythonParticleData") + std::to_string(i) + std::string(".csv");

			if (std::remove(filename.c_str()) != 0)
				std::cout << "No existing file called " + filename + ", creating a new file" << std::endl;
			else
				std::cout << "Removed existing " + filename << std::endl;

			(*this).open(filename, std::ios::app);
			(*this) << "Px" << "," << "Py" << "," << "Pz" << "," << "E" << "," << "t" << std::endl;
			(*this).close();
		}
	}

	void WriteParticleData(int Particle, const Vector& pos, float totalE, float t) {
		std::string filename = std::string("PythonParticleData") + std::to_string(Particle) + std::string(".csv");
		(*this).open(filename, std::ios::app);
		(*this) << pos << totalE << "," << t << std::endl;
		(*this).close();
	}
	
	~WritePythonData() {
		(*this).close();
	}
};


int main() {
	float time;
	float MAX_TIME = 1;
	float dt = 1.00f / 100.00f;
	std::uint64_t file_counter = 0;
	size_t sun_mass = 1.98847e+30;

	//Create an array of Particle Obj on the Stack
	const unsigned int NumOfParticles = 10; //<---- WATCH OUT FOR THIS WHEN CREATING ANOTHER PARTICLE
	Entity Particle[NumOfParticles];

	//Give particel object some attributes via member function
	//Astonomical Units for distances and velocity, and Solor mass http://planetstar.wikia.com/wiki/Astronomical_unit_per_year
	Particle[0].CreatePlanet("Sun", 0, 0, 0, 0, 0, 0, 1.00f, 1.00f);
	Particle[1].CreatePlanet("Mercury", 0.387f, 0, 0, 0, 10.020f, 0, 3.3011e+23/sun_mass, 1.00f); //the planet (satalite)
	Particle[2].CreatePlanet("Venus", 0.723f, 0, 0, 0, 7.388f, 0, 4.8675e+24 / sun_mass, 1.00f);
	Particle[3].CreatePlanet("Earth", 1.00f, 0, 0, 0, 6.283f, 0, 5.97237e+24 / sun_mass, 1.00f);
	Particle[4].CreatePlanet("Mars", 1.524f, 0, 0, 0, 5.082f, 0, 6.4171e+23 / sun_mass, 1.00f);
	Particle[5].CreatePlanet("Jupiter", 5.200f, 0, 0, 0, 2.754f, 0, 1.8982e+27 / sun_mass, 1.00f);
	Particle[6].CreatePlanet("Saturn", 9.538f, 0, 0, 0, 2.033f, 0, 5.6834e+26 / sun_mass, 1.00f);
	Particle[7].CreatePlanet("Uranus", 19.229f, 0, 0, 0, 1.432f, 0, 8.6810e+25 / sun_mass, 1.00f);
	Particle[8].CreatePlanet("Neptune", 30.058f, 0, 0, 0, 1.146f, 0, 1.0243e+26 / sun_mass, 1.00f);
	Particle[9].CreatePlanet("Pluto", 39.264f, 0, 0, 0, 0.996f, 0, 1.303e+22 / sun_mass, 1.00f);

	WritePythonData PythonParticleData(NumOfParticles);

	for (time = 0.00f; time < MAX_TIME; time += dt) {

		//WriteParaviewData ParaviewParticlesData(file_counter);

		for (int i(0); i < NumOfParticles; i++) {
			//write data to files
			//ParaviewParticlesData.WriteParticlesData(i, Particle[i].Radius, Particle[i].Position, Particle[i].Velocity, Particle[i].Acceleration);
			std::cout << "Particle " << i << " Position:" << Particle[i].Position << " Veclocity:" << Particle[i].Velocity << " Acceleration:" << Particle[i].Acceleration << std::endl;

			if (i != 0) { //we dont want to update the suns attributes 

				//Evaluate newtons law of grivatation (IN VECTOR FORM) https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
				float G = 4 * pow((float)M_PI,2); //see wiki page for units
				Vector r = Particle[0].Position - Particle[i].Position; //radius between planet and sun
				Particle[i].Acceleration = r.NormalizeVector() * ((G * Particle[0].Mass * Particle[i].Mass) / (r.Length() * r.Length())); //Centrefugal force 

				//semi-implicit Euler
				Particle[i].Velocity = Particle[i].Velocity + Particle[i].Acceleration * dt;
				Particle[i].Position = Particle[i].Position + Particle[i].Velocity * dt;

				//find planets total energy
				Particle[i].KeneticE = 0.5 * Particle[i].Mass * pow(Particle[i].Velocity.Length(), 2);
				Particle[i].PotentialE = (-1 * G * Particle[0].Mass * Particle[i].Mass) / r.Length();
				Particle[i].TotalE = Particle[i].KeneticE + Particle[i].PotentialE;

				PythonParticleData.WriteParticleData(i, Particle[i].Position, Particle[i].TotalE, time);
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