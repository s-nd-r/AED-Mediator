#define potRND    A1
#define potDAP    A0
#define NUM_LEDS  6       // define the number of LEDs here
#define NUM_LEDS2 12      // define the number of LEDs here
#define LED_PIN   11      // Mode LED
#define LED_PIN2  12      // LED in shifter
#define Ena       5       //enable pin, controls the speed of the motor
#define m1        4       //m1 and m2 control the direction of the motor
#define m2        3
#define button    8       // park button pin
#include "Servo.h"
#include <FastLED.h>

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

int butflag = 0;          // flag value for button

int DAcolor = 0;          // variable values for Assisted and Piloted, dependent on blocking
int DPcolor = 0;

Servo ServoBlock;

// begin and end values for modes on horizontal axis (drive, assisted, piloted)
// CALIBRATE THIS WITH ANALOG INPUT CODE
int DDbegin = 0;
int DDend = 6;
int DAbegin = 31;
int DAend = 47;
int DPbegin = 68;
int DPend = 75;

// begin and end values for modes on vertical axis (reverse, neutral, drive)
// CALIBRATE THIS WITH ANALOG INPUT CODE
int Rbegin = 0;
int Rend = 1;
int Nbegin = 200;
int Nend = 220;
int Dbegin = 0;
int Dend = 550;

// values to and from decision logic
int prefMode = 0;       //0 = drive, 1 = assisted, 2 = piloted
int currentMode = 0;    //0 = drive, 1 = assisted, 2 = piloted, 3 = neutral, 4 = reverse, 5 = park
int blockMode = 0;      //0 = no blocking, 1 = P blocked, 2 = A and P blocked
int goToMode = 0;       //0 = drive, 1 = assisted, 2 = piloted

// servo positions for each blocking option
int block0 = 130;
int block1 = 65;
int block2 = 20;

void setup() {
  pinMode(potRND, INPUT);
  pinMode(potDAP, INPUT);
  pinMode(Ena, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(button, INPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS2);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(250);

  ServoBlock.attach(9);
}

void loop() {
  int posRND = analogRead(potRND);                   // vertical position
  int posDAP = analogRead(potDAP);                   // horizontal position
  delay(10);                                         // space between readings.
  int newPosDAP = analogRead(potDAP);                // new horizontal position
  int difference = posDAP - newPosDAP;               // difference between readings: >0 = moves to right, <0 = moves to left
  //prefMode = digitalRead.RJ45(prefMode); --> whatever we need for the RJ45 thing to read prefMode


  if (posRND > Rbegin && posRND < Rend) {            // if shifter is in R position
    currentMode = 4;
    //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
    leds[0] = CRGB(255, 255, 255);                   // R is lit up
    leds[1] = CRGB(10, 10, 10);
    leds[2] = CRGB(10, 10, 10);
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    fill_solid(leds2, NUM_LEDS2, CRGB :: White);     // shifter handle is white
    FastLED.show();

  } else if (posRND > Nbegin && posRND < Nend) {     // if shifter is in N position
    butflag = 0;
    if (digitalRead(button) == HIGH) {               // if park button is pressed:
      if (butflag == 0) {                            // ... and previous mode was neutral, it goes into park
        butflag = 1;
        currentMode = 5;
        //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
        Serial.println ("park" + currentMode);
        leds[0] = CRGB(10, 10, 10);
        leds[1] = CRGB(10, 10, 10);
        leds[2] = CRGB(10, 10, 10);
        leds[3] = CRGB::Black;
        leds[4] = CRGB::Black;
        leds[5] = CRGB(255, 255, 255);                  // P is lit up
        fill_solid(leds2, NUM_LEDS2, CRGB :: Black);    // shifter handle is off
        FastLED.show();
      }
      else {                                         // ... and previous mode was park, it goes into neutral
        butflag = 0;
        currentMode = 3;
        //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
        leds[0] = CRGB(10, 10, 10);                   // R is lit up
        leds[1] = CRGB(255, 255, 255);
        leds[2] = CRGB(10, 10, 10);
        leds[3] = CRGB::Black;
        leds[4] = CRGB::Black;
        leds[5] = CRGB(10, 10, 10);
        fill_solid(leds2, NUM_LEDS2, CRGB :: White);     // shifter handle is white
        FastLED.show();
      }
      delay(100);                                    // wait for the hardware to settle
    }

  } else if (posRND > Dbegin && posRND < Dend) {

    if (posDAP < DDend) {
      currentMode = 0;
      //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB(255, 255, 255);
      leds[3] = DAcolor;
      leds[4] = DPcolor;
      leds[5] = CRGB::Black;                  // P is lit up
      fill_solid(leds2, NUM_LEDS2, CRGB :: Black);    // shifter handle is off
      FastLED.show();
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);

    } else if (((posDAP > DDend && posDAP < DAbegin) && prefMode == 0) && difference < 0) {              // if shifter is between D and A, and if the preferred mode is D, and user is shifting up
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 200);

    } else if (posDAP > DAbegin && posDAP < DAend) {
      currentMode = 1;
      //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB(10, 10, 10);
      leds[3] = CRGB(255, 100, 0);                                                                // Assisted is lit up
      leds[4] = DPcolor;
      leds[5] = CRGB::Black;
      fill_solid(leds2, NUM_LEDS2, CRGB (255, 100, 0));                                           // shifter handle is amber
      FastLED.show();
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);

    } else if (((posDAP > DAend && posDAP < DPbegin) && prefMode == 1) && difference < 0) {       // if shifter is between A and P, and if the preferred mode is A, and user is shifting up
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 200);

    } else if (posDAP > DPbegin) {
      currentMode = 2;
      //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB(10, 10, 10);
      leds[3] = DAcolor;
      leds[4] = CRGB::Purple;                                                                     // Piloted is lit up
      leds[5] = CRGB::Black;
      fill_solid(leds2, NUM_LEDS2, CRGB (149, 0, 255));                                           // shifter handle is purple
      FastLED.show();
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);
    }


    if (blockMode == 0) {                    // if loop for blocking modes
      ServoBlock.write(block0);              // no blocking
      int DAcolor = CRGB(10, 10, 10);
      int DPcolor = CRGB(10, 10, 10);

    } else if (blockMode == 1) {             // Piloted blocked
      ServoBlock.write(block1);
      int DAcolor = CRGB(10, 10, 10);
      int DPcolor = CRGB::Black;

    } else if (blockMode == 2)              // Assisted and Piloted blocked
      ServoBlock.write(block2);
    int DAcolor = CRGB::Black;
    int DPcolor = CRGB::Black;

  } else {                             // if no block value is provided, then nothing is blocked
    ServoBlock.write(block0);
    ServoBlock.write(block0);              // no blocking
    int DAcolor = CRGB(10, 10, 10);
    int DPcolor = CRGB(10, 10, 10);
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
    if (posDAP > DAend) {
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 180);
    } else if (posDAP < DAbegin) {
      digitalWrite(m1, LOW);
      digitalWrite(m2, HIGH);
      digitalWrite(Ena, 180);
    } else {
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(Ena, 0);
    }
  }

} // end loop
