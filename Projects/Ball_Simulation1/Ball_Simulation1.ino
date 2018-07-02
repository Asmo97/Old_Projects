//References: To understand the physics behind the simulation 
//http://www.vobarian.com/collisions/2dcollisions2.pdf
//https://en.wikipedia.org/wiki/Elastic_collision

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


class Vector {
public:
  Vector() {}
  Vector(double X, double Y) {
    x = X;
    y = Y;
  }
  Vector operator*(double scaleup) const;
  Vector NormalizeVector() const; //Create unit vector
  double Length() const; //Find length of a vector
  Vector operator+(const Vector& vec) const; //add 2 vectors
  Vector operator-(const Vector& vec) const; //subtract 2 vectors
  double operator*(const Vector& vec) const; //Dot Product of two vectors
  double DistanceBetweenCenters(Vector TargetBallPosition) const; //Distance between 2 circles from their centers
  Vector NormalizeBetweenCenters(Vector TargetBallPosition) const; //unit vector between the 2 centers
  Vector CreateTangetVector() const; //Create a perpendicular vector

public:
  double x, y;
};



class Entity {
public:
  Vector Velocity, Acceleration;
  Vector Position; //Removed the point class as it is the exact same as the vector class (x,y)
  int radius;
  double mass;

  void update() {
    this->Position = (this->Position) + ((this->Velocity) * (dt / 1000)); //Note: dt is in milli seconds
    this->Velocity = this->Velocity + (this->Acceleration * (dt / 1000)); //not in use this for this simulation

    //Prevent the ball from going out of bounds fully elastic collision 
    if (this->Position.y - this->radius <= 0 || this->Position.y + this->radius >= SCREEN_HEIGHT) {
      this->Velocity.y = this->Velocity.y * -1;
    }

    if (this->Position.x - this->radius <= 0 || this->Position.x + this->radius >= SCREEN_WIDTH) {
      this->Velocity.x = this->Velocity.x * -1;
    }

    //Stop the Ball after its velocity becomes really small 
    if (this->Velocity.Length() < 0.8f) {
      this->Velocity.x = 0;
      this->Velocity.y = 0;
    }
  }
};

Vector Vector::operator*(double scaleup) const {
  Vector NewVector;

  NewVector.x = x * scaleup;
  NewVector.y = y * scaleup;

  return NewVector;
}

Vector Vector::NormalizeVector() const {
  Vector UnitVector;
  UnitVector.x = x / Length();
  UnitVector.y = y / Length();
  return UnitVector;
}

double Vector::Length() const {
  float length = sqrt(x * x + y * y);
  return length;
}

Vector Vector::operator+(const Vector& vec) const {
  Vector NewVec;

  NewVec.x = x + vec.x;
  NewVec.y = y + vec.y;

  return NewVec;
}

Vector Vector::operator-(const Vector& vec) const {
  Vector NewVec;

  NewVec.x = x - vec.x;
  NewVec.y = y - vec.y;

  return NewVec;
}

double Vector::operator*(const Vector& vec) const {
  double DotProduct;

  DotProduct = (x * vec.x) + (y * vec.y);

  return DotProduct;
}

double Vector::DistanceBetweenCenters(Vector TargetBallPosition) const {
  double distance;
  distance = sqrt((this->x - TargetBallPosition.x) * (this->x - TargetBallPosition.x) + (this->y - TargetBallPosition.y) * (this->y - TargetBallPosition.y));
  return distance;
}

Vector Vector::NormalizeBetweenCenters(Vector TargetBallPosition) const {
  Vector Normal;
  Normal.x = (TargetBallPosition.x - this->x) / DistanceBetweenCenters(TargetBallPosition);
  Normal.y = (TargetBallPosition.y - this->y) / DistanceBetweenCenters(TargetBallPosition);
  return Normal;
}

Vector Vector::CreateTangetVector() const {
  Vector Tangent;
  Tangent.x = -1 * this->y;
  Tangent.y = this->x;
  return Tangent;
}

/*------------------------------------------------------------------------------------------------------------------------------*/

int const NumOfBalls = 4;
Entity Ball[NumOfBalls];
double Y_Axis_Flip[NumOfBalls]; //To draw the ball on the oled the y axis has to be inverted, see repo: Fun_stuff\libraries\Micro_OLED_Pong\Micro_OLED_Schematic.png 

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  oled.begin();
  oled.clear(ALL);
  oled.display();

  Vector MovingVelocity(20,-20);
  Vector StaticVelocity(0, 0);

  Vector BallPosition0(4, 4);
  Vector BallPosition1(12, 12);
  Vector BallPosition2(30, 30);
  Vector BallPosition3(5, 24);

  //Moving Ball
  Ball[0].Velocity = MovingVelocity;
  Ball[0].Position = BallPosition0;
  Ball[0].radius = 2;
  Ball[0].mass = Ball[0].radius * 20;//not really good technique as it ignores density of the ball 

  //Target Ball
  Ball[1].Velocity = StaticVelocity;
  Ball[1].Position = BallPosition1;
  Ball[1].radius = 8;
  Ball[1].mass = Ball[1].radius * 20;

  //Target Ball
  Ball[2].Velocity = StaticVelocity;
  Ball[2].Position = BallPosition2;
  Ball[2].radius = 4;
  Ball[2].mass = Ball[1].radius * 20;

  //Target Ball
  Ball[3].Velocity = StaticVelocity;
  Ball[3].Position = BallPosition3;
  Ball[3].radius = 6;
  Ball[3].mass = Ball[1].radius * 20;
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

  int nCollided = NumOfBalls + 1;//This could be any number just not in range of NumOfBalls
  int mCollided = NumOfBalls + 1;//This could be any number just not in range of NumOfBalls


  for (int n = 0; n < NumOfBalls; n++) {
    for (int m = 0; m < NumOfBalls; m++) {
      if (n != m && n != mCollided && m != nCollided) { //Make sure the ball does not collide with it self. Also prevent collision detection happening again for the same balls
        if (Ball[n].Position.DistanceBetweenCenters(Ball[m].Position) <= (Ball[n].radius + Ball[m].radius)) {
          //Check if balls have collided
          
          nCollided = n;
          mCollided = m;

          //Draw line between ball centers to show collision detection
          oled.line(Ball[n].Position.x, Y_Axis_Flip[n], Ball[m].Position.x, Y_Axis_Flip[m]);

          //Find Distance beetween the ball centers and the ammount of overlap between them 
          double Distance = Ball[n].Position.DistanceBetweenCenters(Ball[m].Position);
          double Overlap = 0.2f * (Distance - Ball[n].radius - Ball[m].radius);

          //Step 1 - Create the normal unit vector and then the tangent vector
          Vector Normal = Ball[n].Position.NormalizeBetweenCenters(Ball[m].Position);
          Vector Tangent = Normal.CreateTangetVector();

          //displace colliding balls away from the collision (static collision)
          Ball[n].Position.x += Overlap * Normal.x;
          Ball[n].Position.y += Overlap * Normal.y;

          Ball[m].Position.x -= Overlap * Normal.x;
          Ball[m].Position.y -= Overlap * Normal.y;

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
          double AC_Velocity_N1 = (Velocity_N1 * (Ball[n].mass - Ball[m].mass) + 2. * Ball[m].mass * Velocity_N2) / (Ball[n].mass + Ball[m].mass);
          double AC_Velocity_N2 = (Velocity_N2 * (Ball[m].mass - Ball[n].mass) + 2. * Ball[n].mass * Velocity_N1) / (Ball[n].mass + Ball[m].mass);

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
  }

  //update balls
  for (int i = 0; i < NumOfBalls; i++) {
    Ball[i].update();
    Y_Axis_Flip[i] = map(Ball[i].Position.y, 0, SCREEN_HEIGHT, SCREEN_HEIGHT, 0);
  }

  //draw balls
  for (int j = 0; j < NumOfBalls; j++) {
    oled.circle(Ball[j].Position.x, Y_Axis_Flip[j], Ball[j].radius);
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
