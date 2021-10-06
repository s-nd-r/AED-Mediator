#define potRND    A1
#define potDAP    A0
#define NUM_LEDS  6       // define the number of LEDs here
#define NUM_LEDS2 12      // define the number of LEDs here
#define LED_PIN   12      // Mode LED
#define LED_PIN2  13      // LED in shifter
#define button    8       // park button pin
#include "Servo.h"
#include <FastLED.h>

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

int butflag = 0;          // flag value for button

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
int currentMode = 0;    //0 = drive, 1 = assisted, 2 = piloted, 3 = neutral, 4 = reverse, 5 = park

void setup() {
  pinMode(potRND, INPUT);
  pinMode(potDAP, INPUT);
  pinMode(button, INPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS2);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(250);
}

void loop() {
  int posRND = analogRead(potRND);                   // vertical position
  int posDAP = analogRead(potDAP);                   // horizontal position

  if (posRND > Rbegin && posRND < Rend) {            // if shifter is in R position
    currentMode = 4;
    //digitalWrite (currentMode) --> whatever we need for the RJ45 thing to write currentMode
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
        //digitalWrite (currentMode) --> whatever we need for the RJ45 thing to write currentMode
        Serial.println ("park" + currentMode);
        leds[0] = CRGB(10, 10, 10);
        leds[1] = CRGB(10, 10, 10);
        leds[2] = CRGB(10, 10, 10);
        leds[3] = CRGB::Black;
        leds[4] = CRGB::Black;
        leds[5] = CRGB(255, 255, 255);                  // P is lit up
        fill_solid(leds2, NUM_LEDS2, CRGB :: Black);    // shifter handle is off
        FastLED.show();
      } else {                                         // ... and previous mode was park, it goes into neutral
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
      leds[3] = CRGB(10, 10, 10);
      leds[4] = CRGB(10, 10, 10);
      leds[5] = CRGB::Black;                  // P is lit up
      fill_solid(leds2, NUM_LEDS2, CRGB :: Black);    // shifter handle is off
      FastLED.show();

    } else if (posDAP > DAbegin && posDAP < DAend) {
      currentMode = 1;
      //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB(10, 10, 10);
      leds[3] = CRGB(255, 100, 0);                                                                // Assisted is lit up
      leds[4] = CRGB(10, 10, 10);
      leds[5] = CRGB::Black;
      fill_solid(leds2, NUM_LEDS2, CRGB (255, 100, 0));                                           // shifter handle is amber
      FastLED.show();

    } else if (posDAP > DPbegin) {
      currentMode = 2;
      //digitalWrite.RJ45 (currentMode) --> whatever we need for the RJ45 thing to write currentMode
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB(10, 10, 10);
      leds[3] = CRGB(10, 10, 10);
      leds[4] = CRGB::Purple;                                                                     // Piloted is lit up
      leds[5] = CRGB::Black;
      fill_solid(leds2, NUM_LEDS2, CRGB (149, 0, 255));                                           // shifter handle is purple
      FastLED.show();
    }
  }

} // end loop
