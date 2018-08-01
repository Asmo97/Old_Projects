#define _USE_MATH_DEFINES // for C++

#include <iostream>
#include <array>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <cstdint>

class Vector : public std::array<double, 3> {
	//std::array<float, 2> vec; // <- You've inherited from std::array, so Vector IS an array.
public:
	//Defualt constructor - Apparently i need it in order to create an array of objects(or instances) for the entity class?
	Vector() {}
	//Another constructor
	Vector(double X, double Y, double Z) {
		(*this)[0] = X;
		(*this)[1] = Y;
		(*this)[2] = Z;
	}
public:
	//member functions, will use the scope operator :: to make the class easy to read
	double Length() const; //Finds the length of a vector
	Vector NormalizeVector() const; //Create unit vector
	Vector operator*(double scale) const; //scale the vector
	double operator*(const Vector& vec) const; //Dot Product of two vectors
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
	double Radius;
	double Density;
	double Volume = (4 / 3) * (double)M_PI * powf(Radius, 3.00f); //Casting for pi
	//float Mass = Density * Volume;
	double Mass;
	double KeneticE = 0;
	double PotentialE = 0;
	double TotalE = 0;

public:
	//Defualt constructor - Apparently i need it in order to create an array of objects(or instances)
	Entity() {}
	//Constructor intialiser list
	Entity(const Vector& Pos, const Vector& Vel, const Vector& Acc, double rad, double m) :
		Position(Pos),
		Velocity(Vel),
		Acceleration(Acc),
		Radius(rad),
		Mass(m)
		//Density(den)
	{}

	void CreatePlanet(const char* planet, double Px, double Py, double Pz, double Vx, double Vy, double Vz, double r, double m) {
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

double Vector::Length() const {
	//There's two ways of accessing the base class. One way is to
	//dereference the this pointer, i.e. *this. the other is to call
	//them explicitly, like this std::array::operator[](0). I think
	//*this is neater
	double length = std::sqrt((*this)[0] * (*this)[0] + (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2]);
	return length;
}

Vector Vector::NormalizeVector() const {
	Vector NormVec(*this);

	for (int i(0); i < 3; i++)
		NormVec[i] /= Length();

	return NormVec;
}

Vector Vector::operator*(double scale) const {
	Vector NewVector(*this);

	for (int i(0); i < 3; i++)
		NewVector[i] *= scale;

	return NewVector;
}

double Vector::operator*(const Vector& vec) const { //Pass the vector by reference to avoid taking a copy!

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

	void WriteParticlesData(int particle, double rad, const Vector& pos, const Vector& vel, const Vector& acc) {
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

	void WriteParticleData(int Particle, const Vector& pos, double totalE, double t) {
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
	double time;
	double MAX_TIME = 1;
	double dt = 1.00f / 100.00f;
	std::uint64_t file_counter = 0;

	//Create an array of Particle Obj on the Stack
	const unsigned int NumOfParticles = 10; //<---- WATCH OUT FOR THIS WHEN CREATING ANOTHER PARTICLE
	Entity Particle[NumOfParticles];

	//Give particel object some attributes via member function
	//SI units (radius is 1.00 for all planets)
	Particle[0].CreatePlanet("Sun", 0, 0, 0, 0, 0, 0, 1.00f, 1.989e+30);
	Particle[1].CreatePlanet("Mercury", 57909175e+3, 0, 0, 0, 47.362e+3, 0, 1.00f, 3.3011e+23); //the planet (satalite)
	Particle[2].CreatePlanet("Venus", 108208930e+3, 0, 0, 0, 35.02e+3, 0, 1.00f, 4.8675e+24);
	Particle[3].CreatePlanet("Earth", 149597890e+3, 0, 0, 0, 29.78e+3, 0, 1.00f, 5.97237e+24);
	Particle[4].CreatePlanet("Mars", 227936640e+3, 0, 0, 0, 24.007e+3, 0, 1.00f, 6.4171e+23);
	Particle[5].CreatePlanet("Jupiter", 778412020e+3, 0, 0, 0, 13.07e+3f, 0, 1.00f, 1.8982e+27);
	Particle[6].CreatePlanet("Saturn", 1426725400e+3, 0, 0, 0, 9.68e+3, 0, 1.00f, 5.6834e+26);
	Particle[7].CreatePlanet("Uranus", 2870972200e+3, 0, 0, 0, 6.80e+3, 0, 1.00f, 8.6810e+25);
	Particle[8].CreatePlanet("Neptune", 4498252900e+3, 0, 0, 0, 5.43e+3, 0, 1.00f, 1.0243e+26);
	Particle[9].CreatePlanet("Pluto", 5906380000e+3, 0, 0, 0, 4.67e+3, 0, 1.00f, 1.303e+22);

	WritePythonData PythonParticleData(NumOfParticles);

	for (time = 0.00f; time < MAX_TIME; time += dt) {

		//WriteParaviewData ParaviewParticlesData(file_counter);

		for (int i(0); i < NumOfParticles; i++) {
			//write data to files
			//ParaviewParticlesData.WriteParticlesData(i, Particle[i].Radius, Particle[i].Position, Particle[i].Velocity, Particle[i].Acceleration);
			std::cout << "Particle " << i << " Position:" << Particle[i].Position << " Veclocity:" << Particle[i].Velocity << " Acceleration:" << Particle[i].Acceleration << std::endl;

			if (i != 0) { //we dont want to update the suns attributes 

				//Evaluate newtons law of grivatation (IN VECTOR FORM) https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
				double G = 6.67408e-11; //m^3 kg-1 s^-2
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