//References: To understand the physics behind the simulation 
//http://www.vobarian.com/collisions/2dcollisions2.pdf
//https://en.wikipedia.org/wiki/Elastic_collision

//PreProcessor Instruction, it copies and paste all the code from one directory to this translation unit.
//<> indicate that the header file is in another working directory, not in this current directory. "" does the same but checks current directory first. 
#include <Wire.h>
#include <SFE_MicroOLED.h>  //OLED SFE_MicroOLED library
#include <cmath>

//OLED set up
#define PIN_RESET 225 //OLED
#define DC_JUMPER 0  //OLED I2C Addres: 0 - 0x3C, 1 - 0x3D
MicroOLED oled(PIN_RESET, DC_JUMPER); //OLED I2C SETUP

#define ledPin 13 //builtin LED
bool ledState = false;
unsigned long old_time;
unsigned long new_time;
float dt;
const unsigned int SCREEN_WIDTH = 64;
const unsigned int SCREEN_HEIGHT = 48;


class Vector { //create a vector class. class and structs are the same except that structs are always public
public:
  //data members
  double Data[2]; //create an array allocated on the STACK
public:
  Vector() {} //Default constructor (I need this in order to create an array for ball instance)

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
  double operator*(const Vector& vec) const; //Dot Product of two vectors
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
  Entity(){}
  
  //Constructor intialiser list
  Entity(const Vector& Pos, const Vector& Vel, const Vector& Acc, double Rad) :
    Position(Pos),
    Velocity(Vel),
    Acceleration(Acc),
    Radius(Rad),
    Mass(Rad*20) //Desnsity of ball is ignored 
  {}

  void update() {
    //this member function is responsible for frame rate independance
    this->Position = (this->Position) + ((this->Velocity) * (dt / 1000)); //Note: dt is in milli seconds
    this->Velocity = this->Velocity + (this->Acceleration * (dt / 1000)); //not in use this for this simulation
  }
  void KeepInBorders() {
    //Prevent the ball from going out of bounds by fully elastic collision with borders
    if (this->Position[1] - this->Radius <= 0 || this->Position[1] + this->Radius >= SCREEN_HEIGHT) {
      this->Velocity[1] = this->Velocity[1] * -1;
    }

    if (this->Position[0] - this->Radius <= 0 || this->Position[0] + this->Radius >= SCREEN_WIDTH) {
      this->Velocity[0] = this->Velocity[0] * -1;
    }
  }
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

double Vector::operator*(const Vector& vec) const { //Pass the vector by reference to avoid taking a copy!

  return (this->Data[0] * vec[0]) + (this->Data[1] * vec[1]);

}

Vector Vector::operator+(const Vector& vec) const { //Pass the vector by reference to avoid taking a copy!
  Vector NewVector(*this);

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

int const NumOfBalls = 4;
Entity Ball[NumOfBalls]; //Create a ball object
double Y_Axis_Flip[NumOfBalls]; //To draw the ball on the oled the y axis has to be inverted, see repo: Fun_stuff\libraries\Micro_OLED_Pong\Micro_OLED_Schematic.png 

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  oled.begin();
  oled.clear(ALL);
  oled.display();

  Vector MovingVelocity(30, 30);
  Vector StaticVelocity(0, 0);
  Vector BallAcceleration(0, 0);

  //Normal cartesian coordinates
  Vector BallPosition0(5, 20);
  Vector BallPosition1(12, 12);
  Vector BallPosition2(35, 35);
  Vector BallPosition3(40, 15);

  //Moving Ball
  Ball[0] = Entity(BallPosition0, MovingVelocity, BallAcceleration, 2);

  //Target Balls
  Ball[1] = Entity(BallPosition1, StaticVelocity, BallAcceleration, 8);
  Ball[2] = Entity(BallPosition2, StaticVelocity, BallAcceleration, 4);
  Ball[3] = Entity(BallPosition3, StaticVelocity, BallAcceleration, 6);
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

  oled.clear(PAGE); //clear the buffer

  //Find dt, the time it takes to finish one void loop(){}
  new_time = millis(); //millis starts a timer that is in milli seconds
  dt = (new_time - old_time);
  Serial.println(dt);
  old_time = new_time;


  for (int n = 0; n < NumOfBalls; n++) {
    for (int m = n+1; m < NumOfBalls; m++) {
      if ((Ball[n].Position - Ball[m].Position).Length() <= (Ball[n].Radius + Ball[m].Radius)) {
        //Check if balls have collided

        //Draw line between ball centers to show collision detection
        oled.line(Ball[n].Position[0], Y_Axis_Flip[n], Ball[m].Position[0], Y_Axis_Flip[m]);

        //Find Distance beetween the ball centers and the ammount of overlap between them 
        double Distance = (Ball[n].Position - Ball[m].Position).Length();
        double Overlap = 0.2f * (Distance - Ball[n].Radius - Ball[m].Radius);

        //Step 1 - Create the normal unit vector and then the tangent vector
        Vector Normal = (Ball[n].Position - Ball[m].Position).NormalizeVector();
        Vector Tangent = Normal.CreateTangetVector();

        //displace colliding balls away from the collision (static collision)
        //          Ball[n].Position.x += Overlap * Normal.x;
        //          Ball[n].Position.y += Overlap * Normal.y;
        //
        //          Ball[m].Position.x -= Overlap * Normal.x;
        //          Ball[m].Position.y -= Overlap * Normal.y;

        //Step 2 - Skip

        //Step 3  - project the velocity vectors onto the unit normal(N) and unit tangent(T) vectors by computing the dot product 
        double Velocity_T1 = Ball[n].Velocity * Tangent;
        double Velocity_N1 = Ball[n].Velocity * Normal;

        double Velocity_T2 = Ball[m].Velocity * Tangent;
        double Velocity_N2 = Ball[m].Velocity * Normal;

        /*Step 4 - Find the new Tangential Velocities velocity do not change after the collision because there is no
        force between the circles in the tangential direction during the collision*/
        //NOTE: AC means After COllision
        double AC_Velocity_T1 = Velocity_T1;
        double AC_Velocity_T2 = Velocity_T2;

        //Step 5 - Find the Normal Velcity after collision using the 1D collision Forumula
        double AC_Velocity_N1 = (Velocity_N1 * (Ball[n].Mass - Ball[m].Mass) + 2. * Ball[m].Mass * Velocity_N2) / (Ball[n].Mass + Ball[m].Mass);
        double AC_Velocity_N2 = (Velocity_N2 * (Ball[m].Mass - Ball[n].Mass) + 2. * Ball[n].Mass * Velocity_N1) / (Ball[n].Mass + Ball[m].Mass);

        //Step 6 - Convert the scalar normal and tangential velocities into vectors (not the dot product)
        Vector AC_Velocity_N1_Vec = Normal * AC_Velocity_N1;
        Vector AC_Velocity_T1_Vec = Tangent * AC_Velocity_T1;

        Vector AC_Velocity_N2_Vec = Normal * AC_Velocity_N2;
        Vector AC_Velocity_T2_Vec = Tangent * AC_Velocity_T2;

        //Step 7 - Find the final velocity vectors by adding the normal and tangential components for each object
        Vector AC_Velocity_1 = AC_Velocity_N1_Vec + AC_Velocity_T1_Vec;
        Vector AC_Velocity_2 = AC_Velocity_N2_Vec + AC_Velocity_T2_Vec;

        //Step 8 - Update the Balls Velocity
        Ball[n].Velocity = AC_Velocity_1;
        Ball[m].Velocity = AC_Velocity_2;
      }
    }
  }

  //update balls
  for (int i = 0; i < NumOfBalls; i++) {
    Ball[i].update();
    Y_Axis_Flip[i] = map(Ball[i].Position[1], 0, SCREEN_HEIGHT, SCREEN_HEIGHT, 0);
  }

  //draw balls
  for (int j = 0; j < NumOfBalls; j++) {
    oled.circle(Ball[j].Position[0], Y_Axis_Flip[j], Ball[j].Radius);
  }

  ShowFPS();
}


void ShowFPS() {
  oled.setFontType(0);
  oled.setCursor(0, 0);
  oled.print("FPS:\n");
  oled.setCursor(24, 0);
  oled.print(1 / (dt / 1000));
  oled.display();
}

//nested for loop debug, with 2 balls: Ball[0] and Ball[1]
//n = 0 m = 0 GOOD, ACCOUNTED FOR
//n = 0 m = 1 COLLISION
//n = 1 m = 0 BAD, COLLISION DETECTION AGAIN
//n = 1 m = 1 GOOD, ACCOUNTED FOR
