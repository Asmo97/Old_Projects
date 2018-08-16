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
	double MAX_TIME = 1000; 
	double dt = 1.00; //1 Solar Day
	std::uint64_t file_counter = 0;

	//Create an array of Particle Obj on the Stack
	const unsigned int NumOfParticles = 10; //<---- WATCH OUT FOR THIS WHEN CREATING ANOTHER PARTICLE
	Entity Particle[NumOfParticles];

	//Give particel object some attributes via member function
	//The masses are taken relative to the Sun, heliocentric distances are in A.U. (astronomical units), time in Earth days
	Particle[0].CreatePlanet("Sun", 0, 0, 0, 0, 0, 0, 1.00f, 1);
	Particle[1].CreatePlanet("Mercury", 3.582003550509369E-01, -1.106711537126086E-01, -4.198772232887506E-02, 2.837327404682707E-03, 2.814552279662408E-02, 2.039574776485654E-03, 1.00f, 1.651998994E-7); //the planet (satalite)
	Particle[2].CreatePlanet("Venus", 1.169414393096834E-01, -7.178268450841996E-01, -1.659517206836183E-02, 1.982673552089452E-02, 3.181258433262683E-03, -1.100494062679515E-03, 1.00f, 2.447573548E-6);
	Particle[3].CreatePlanet("Earth", 8.071651056948089E-01, -6.116288312685261E-01, 2.026118733774949E-05, 1.010842147950843E-02, 1.365428708892866E-02, -7.936152929295118E-07, 1.00f, 3.003268796E-6);
	Particle[4].CreatePlanet("Mars", 1.002259978275141E+00, -9.599729608079449E-01, -4.470861097048866E-02, 1.020919012415160E-02, 1.130388712074698E-02, -1.365626667320784E-05, 1.00f, 3.227105859E-7);
	Particle[5].CreatePlanet("Jupiter", -3.032866238164082E+00, -4.451352194764283E+00, 8.635007952427044E-02, 6.150689965483747E-03, -3.896629107801202E-03, -1.214382179270277E-04, 1.00f, 9.544883078E-4);
	Particle[6].CreatePlanet("Saturn", 1.241022210371602E+00, -9.986898686592236E+00, 1.241943420031552E-01, 5.236307741465693E-03, 6.669446607616443E-04, -2.196298850769255E-04, 1.00f, 2.857933115E-4);
	Particle[7].CreatePlanet("Uranus", 1.729292868435325E+01, 9.800196745658209E+00, -1.875280685583884E-01, -1.961762424738595E-03, 3.234714069407250E-03, 3.717731411951507E-05, 1.00f, 4.365602213E-4);
	Particle[8].CreatePlanet("Neptune", 2.887289827253568E+01, -7.909509937130501E+00, -5.025924589017090E-01, 8.151468578369611E-04, 3.043624603687646E-03, -8.115060243079369E-05, 1.00f, 5.149610259E-5);
	Particle[9].CreatePlanet("Pluto", 1.146561091514581E+01, -3.160682122191916E+01, 6.484213579439634E-02, 3.036273559933204E-03, 4.170573917353994E-04, -9.176459347526458E-04, 1.00f, 6.552677898E-9);

	WritePythonData PythonParticleData(NumOfParticles);

	for (time = 0.00f; time < MAX_TIME; time += dt) {

		//WriteParaviewData ParaviewParticlesData(file_counter);

		for (int i(0); i < NumOfParticles; i++) {
			//write data to files
			//ParaviewParticlesData.WriteParticlesData(i, Particle[i].Radius, Particle[i].Position, Particle[i].Velocity, Particle[i].Acceleration);
			std::cout << "Particle " << i << " Position:" << Particle[i].Position << " Veclocity:" << Particle[i].Velocity << " Acceleration:" << Particle[i].Acceleration << std::endl;

			if (i != 0) { //we dont want to update the suns attributes 

				//Evaluate newtons law of grivatation (IN VECTOR FORM) https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
				double G = 0.0002959122083;
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