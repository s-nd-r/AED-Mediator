#define potRND    A1
int potDAP = A0;
#define Led1      6
#define Led2      7
#define Ena       5
#define m1        4
#define m2        3
#define button    8
#include "Servo.h"

Servo ServoBlock;

// begin and end values for modes on horizontal axis (drive, assisted, piloted)
int DDbegin = 0;
int DDend = 6;
int DAbegin = 31;
int DAend = 47;
int DPbegin = 68;
int DPend = 75;

// begin and end values for modes on vertical axis (reverse, neutral, drive)
int Rbegin = 0;
int Rend = 1;
int Nbegin = 200;
int Nend = 220;
int Dbegin = 0;
int Dend = 550;

int prefMode = 0;       //0 = drive, 1 = assisted, 2 = piloted
int currentMode = 0;    //0 = drive, 1 = assisted, 2 = piloted, 3 = neutral, 4 = reverse, 5 = park
int blockMode = 0;      //0 = no blocking, 1 = P blocked, 2 = A and P blocked
int goToMode = 0;       //0 = drive, 1 = assisted, 2 = piloted
int butflag = 0;

// servo positions for each blocking option
int block0 = 130;
int block1 = 65;
int block2 = 20;

void setup() {
  Serial.begin(9600);
  pinMode(potRND, INPUT);
  pinMode(potDAP, INPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Ena, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(button, INPUT);

  ServoBlock.attach(9);
}

void loop() {
  int posRND = analogRead(potRND);                   // vertical position
  int posDAP = analogRead(potDAP);                   // horizontal position
  delay(10);                                         // space between readings.
  int newPosDAP = analogRead(potDAP);                // new horizontal position
  int difference = posDAP - newPosDAP;               // difference between readings: >0 = moves to right, <0 = moves to left
  //Serial.println (difference);

  if (posRND > Rbegin && posRND < Rend) {            // if shifter is in R position
    currentMode = 4;

  } else if (posRND > Nbegin && posRND < Nend) {     // if shifter is in N position
    butflag = 0;
    if (digitalRead(button) == HIGH) {               // if park button is pressed:
      if (butflag == 0) {                            // ... and previous mode was neutral, it goes into park
        butflag = 1;
        currentMode = 5;
        Serial.println ("park" + currentMode);
        //digitalWrite(led, HIGH); --> whatever we need for turning P light on
      }
      else {                                         // ... and previous mode was park, it goes into neutral
        butflag = 0;
        currentMode = 3;
        Serial.println ("neutral" + currentMode);
        //digitalWrite(led, LOW); --> whatever we need for turning P light off and N on
      }
      delay(100);                                    // wait for the hardware to settle
    }

  } else if (posRND > Dbegin && posRND < Dend) {

    if (posDAP < DDend) {
      currentMode = 0;
      //digitalWrite(led, HIGH); --> whatever we need for turning D light on
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);

    } else if (((posDAP > DDend && posDAP < DAbegin) && prefMode == 0) && difference < 0) {              // if shifter is between D and A, and if the preferred mode is D, and user is shifting up
      ffRight();

    } else if (posDAP > DAbegin && posDAP < DAend) {
      currentMode = 1;
      //digitalWrite(led, HIGH); --> whatever we need for turning DA light on
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);

    } else if (((posDAP > DAend && posDAP < DPbegin) && prefMode == 1) && difference < 0) {       // if shifter is between A and P, and if the preferred mode is A, and user is shifting up
      ffRight();

    } else if (posDAP > DPbegin) {
      currentMode = 2;
      //digitalWrite(led, HIGH); --> whatever we need for turning DP light on
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);

    } else if (((posDAP > DAend && posDAP < DPbegin) && prefMode == 2) && difference > 0) {       // if shifter is between A and P, and if the preferred mode is P, and user is shifting down
      ffLeft();

    } else if (((posDAP > DDend && posDAP < DAbegin) && prefMode == 1) && difference > 0) {       // if shifter is between D and A, and if the preferred mode is A, and user is shifting down
      ffLeft();

    }
  }

  //digitalRead.RJ45 (blockMode) --> whatever we need for the RJ45 thing to read blockMode

  if (blockMode == 0) {               // if loop for blocking modes
    ServoBlock.write(block0);
    //all LED's are grey
  } else if (blockMode == 1) {
    ServoBlock.write(block1);
    //P is off, the rest is grey
  } else if (blockMode == 2) {
    ServoBlock.write(block2);
    // A and P are off, rest is grey
  } else {                             // if no block value is provided, then nothing is blocked
    ServoBlock.write(block0);
    //all LED's are grey
  }

  //digitalRead.RJ45 (goToMode) --> whatever we need for the RJ45 thing to read goToMode

  if (goToMode == 0 && currentMode != 0) {
    if (posDAP > DDend) {
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 1023);
    } else {
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);
    }
  } else if (goToMode == 1 && currentMode != 1) {
  }

  if (posDAP > DAend) {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(Ena, 1023);
  } else if (posDAP < DAbegin) {
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    digitalWrite(Ena, 1023);
  } else {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    digitalWrite(Ena, 0);
  }


} // end loop


void ffRight() {                     // Force feedback towards the right, so against shifting up
  digitalWrite(m1, LOW);
  digitalWrite(m2, HIGH);
  digitalWrite(Ena, 255);
}

void ok() {                             // No force feedback
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(Ena, 0);
}
