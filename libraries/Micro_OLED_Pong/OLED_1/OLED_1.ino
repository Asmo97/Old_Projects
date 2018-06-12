#include <Wire.h>  // I2C
#include <SFE_MicroOLED.h>  // SFE_MicroOLED library

#define PIN_RESET 2  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

MicroOLED oled(PIN_RESET, DC_JUMPER); //I2C SETUP

int SCREEN_WIDTH = 64;
int SCREEN_HEIGHT = 48;

int ButtonA = 2; //left button pin
int ButtonB = 14; //right button pin
int Pot = 0; //potentiometer pin (analog)

boolean StartGame = false;
boolean EndGame = false;

int score0 = 0; //left player score
int score1 = 0; //right player score
int p1_y_read = 19; //right paddle starting position (paddle 1)
int ball_y = random(4,45); //ball starting position (y direction)
int ball_x = 31; //ball starting position (x direction, middle of OLED)
int ball_rad = 2; //ball radius
char *ball_direction_start[] = {"LEFT_DOWN", "LEFT_UP", "RIGHT_DOWN", "RIGHT_UP"}; //4 possible vectors that the ball can take at start of game
char *ball_direction = ball_direction_start[random(0,3)]; //pick a random vector
int ball_velocity_x = 2;
int ball_velocity_y = 2;
int winner = 0; //player winner counter


void setup() {
  Serial.begin(9600);
  pinMode(ButtonA, INPUT);
  pinMode(ButtonB, INPUT);
  pinMode(Pot, INPUT); //potentiometer input
  oled.begin();
  oled.clear(ALL);
  oled.display(); //Scene 1 -- SFE_microOLED symbol 
  delay(3000);
  startingPage(); //Scene 2
  waitingPage(); //Scene 3
}

void loop() { 
  if (digitalRead(ButtonA) == 1 || digitalRead(ButtonB) == 1){
    StartGame = !StartGame; //Wait on Scene 3 until input from user
  }
  if (StartGame == true && EndGame == false){
    oled.clear(PAGE); // Start Scene 4 -- Pong Game

    //read inputs and convert them to game type data
    //left paddle uses potentiometer
    int p0_y_read = constrain(debouncePot(analogRead(Pot)),100,900); //constrians value from range of 0-1023 to 100-900 (makes it easier to play with potentiometer)
    int p0_y_map = map(p0_y_read,900,100,1,35); //converts value from range of 100-900 to 1-35 (1-35 repersents the position of the very top pixel of the paddle)
    int p0_y = constrain(p0_y_map,1,35); //prevent random errors
    Serial.println(analogRead(Pot));

    //right paddle uses buttons
    if (digitalRead(ButtonA) == 1 && p1_y_read < 35) {
      p1_y_read = p1_y_read + 4; //how fast the paddle moves up
      p1_y_read = constrain(p1_y_read,1,35);
    }else if(digitalRead(ButtonB) == 1 && p1_y_read > 1){
      p1_y_read = p1_y_read - 4; //how fast the paddle moves down
      p1_y_read = constrain(p1_y_read,1,35);
    }
    int p1_y = constrain(p1_y_read,1,35); //prevent errors

    //Game draw loop
    drawGame(p0_y, p1_y, ball_y, ball_x, ball_rad, score0, score1);

    //Ball movement
    if (ball_direction == "LEFT_DOWN"){
        ball_x -= ball_velocity_x;
        ball_y += ball_velocity_y;
    }
    else if (ball_direction == "LEFT_UP"){
        ball_x -= ball_velocity_x;
        ball_y -= ball_velocity_y;
    }
    else if (ball_direction == "RIGHT_UP"){
        ball_x += ball_velocity_x;
        ball_y -= ball_velocity_y;
    }
    else if (ball_direction == "RIGHT_DOWN"){
        ball_x += ball_velocity_x;
        ball_y += ball_velocity_y;
    }

    //Check if the ball scored a point 
    if (ball_x + ball_rad >= SCREEN_WIDTH) {
      score0++; //left player gets a point
      ball_y = random(4,45); //reset the ball starting position
      ball_x = 31;
      ball_direction = ball_direction_start[random(0,3)];
      delay(200);
    } else if(ball_x - ball_rad <= 0){
      score1++;
      ball_y = random(4,45); //reset the ball starting position
      ball_x = 31; 
      ball_direction = ball_direction_start[random(0,3)];
      delay(200);
    }

    //collison detection TOP WALL
     if (ball_y - ball_rad <= 0 && ball_direction == "RIGHT_UP"){
       ball_direction = "RIGHT_DOWN";
     } else if(ball_y - ball_rad <= 0 && ball_direction == "LEFT_UP"){
       ball_direction = "LEFT_DOWN";
     }

     //Collison detection BOTTOM WALL
     if (ball_y + ball_rad >= 48 && ball_direction == "RIGHT_DOWN"){
       ball_direction = "RIGHT_UP";
     } else if(ball_y + ball_rad >= 48 && ball_direction == "LEFT_DOWN"){
       ball_direction = "LEFT_UP";
     }

     //collision detection left paddle(paddle0)
     if (ball_x - ball_rad <= 4 && ((ball_y - ball_rad >= p0_y && ball_y - ball_rad <= p0_y + 12) || (ball_y + ball_rad >= p0_y && ball_y + ball_rad <= p0_y + 12)) && ball_direction == "LEFT_DOWN"){
      ball_direction = "RIGHT_DOWN";
     } else if (ball_x - ball_rad <= 4 && ((ball_y - ball_rad >= p0_y && ball_y - ball_rad <= p0_y + 12) || (ball_y + ball_rad >= p0_y && ball_y + ball_rad <= p0_y + 12)) && ball_direction == "LEFT_UP"){
      ball_direction = "RIGHT_UP";
     }

     //collision detection right paddle(paddle1)
     if (ball_x + ball_rad >= 59 && ((ball_y - ball_rad >= p1_y && ball_y - ball_rad <= p1_y + 12) || (ball_y + ball_rad >= p1_y && ball_y + ball_rad <= p1_y + 12)) && ball_direction == "RIGHT_DOWN"){
      ball_direction = "LEFT_DOWN";
     } else if (ball_x - ball_rad >= 59 && ((ball_y - ball_rad >= p1_y && ball_y - ball_rad <= p1_y + 12) || (ball_y + ball_rad >= p1_y && ball_y + ball_rad <= p1_y + 12)) && ball_direction == "RIGHT_UP"){
      ball_direction = "LEFT_UP";
     }
     
     //winner
     if (score0 == 5) {
      drawGame(p0_y, p1_y, ball_y, ball_x, ball_rad, 5, score1);
      winner = 1;
      EndGame = !EndGame;
     } else if(score1 == 5){
      drawGame(p0_y, p1_y, ball_y, ball_x, ball_rad, score0, 5);
      winner = 2;
      EndGame = !EndGame;
     }   
     
  }else if (StartGame == true && EndGame == true){
    //Scene 5 -- Winner
    oled.setFontType(0);
    oled.setCursor(9, 12);
    oled.printf("Player %d\n",winner);
    oled.setFontType(1);
    oled.setCursor(6, 25);
    oled.print("WINNER\n");
    oled.display();
    //Wait for input then restart game
      if (digitalRead(ButtonA) == 1 || digitalRead(ButtonB) == 1){
        score0 = 0;
        score1 = 0;
        ball_y = random(4,45); //ball starting position
        ball_x = 31; 
        ball_direction = ball_direction_start[random(0,3)];
        EndGame = !EndGame;
      }
  }
}

void startingPage() {
  oled.clear(PAGE);
  oled.setFontType(1);
  oled.setCursor(15, 4);
  oled.print("PONG!\n");
  oled.rect(12, 2, 46, 16);
  oled.setFontType(0);
  oled.setCursor(12, 30);
  oled.print("Made by:\n");
  oled.setCursor(16, 40);
  oled.print("AU Eng\n");
  oled.display();
  delay(5000);
}

void waitingPage() {
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("Best to 5 wins!\n");
  oled.print("Press any button to start!\n");
  oled.print("Good Luck!\n");
  oled.display();
}


void drawGame(int p0_y, int p1_y, int ball_y, int ball_x, int ball_rad, int score0, int score1) {
  //border
  oled.rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  //middle dashed line
  oled.line(31, 0, 31, 3); oled.line(31, 6, 31, 9); oled.line(31, 12, 31, 15); oled.line(31, 18, 31, 21); oled.line(31, 24, 31, 27); oled.line(31, 30, 31, 33); oled.line(31, 36, 31, 39); oled.line(31, 42,31, 45);
  //score (left)
  oled.setFontType(0);
  oled.setCursor(31-9,3);
  oled.print(score0);
  //score (right)
  oled.setCursor(31+5,3);
  oled.print(score1);
  //left paddle
  oled.line(4, p0_y, 4, p0_y + 12);
  //right paddle
  oled.line(59, p1_y, 59, p1_y + 12);
  //ball
  oled.circleFill(ball_x, ball_y, ball_rad);
  //display
  oled.display();
}

int debouncePot(int lastval){
  delay(1);
  int newval = analogRead(Pot);
  if (lastval - newval < 60 || lastval - newval > -60){
    return lastval; 
  }else{
    return newval; 
  }
}


