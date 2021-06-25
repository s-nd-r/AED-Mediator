#include <FastLED.h>
#include <Servo.h>

#define NUM_LEDS 6    // define the number of LEDs here
#define NUM_LEDS2 12  // define the number of LEDs here
#define LED_PIN 13    // Mode LED
#define LED_PIN2 12   // LED in shifter
#define enA       5    //enable pin, controls the speed of the motor
#define in1       4     //in1 and in2 control the direction of the motor
#define in2       3
#define button    8

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

Servo ServoBlock;

char val; // Data received from the serial port
int butflag = 0;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 bps
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS2);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(250);
  ServoBlock.attach(10);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  //fill_solid(leds, NUM_LEDS, CRGB ::Orange); //
  //fill_solid(leds, NUM_LEDS, CRGB (255,100,0));// ORANGE (for A when selected), this makes all the LEDs purple, this is useful for the LEDs inside the joystick
  //fill_solid(leds, NUM_LEDS, CRGB (149,0,255)); // PURPLE (for H when selected)
  //fill_solid(leds, NUM_LEDS, CRGB :: White); // for R,N,D when selected
  //fill_solid(leds, NUM_LEDS, CRGB :: Black); // when mode not available, so LED off.

  //FastLED.show(); //this shows the leds, always has to be in the code after one of the lines above.

  //to turn on a seperate LED use this code (this is useful for the mode letters)
  //leds[0] = CRGB::Purple; // 0 is the 1st LED
  //leds[0] = CRGB(10, 10, 10); // when mode is available but not selected
  //FastLED.show();

  while (Serial.available()) { // If data is available to read,
    val = Serial.read(); // read it and store it in val
  }

  if (val == '1') {
    ServoBlock.write(127);          // Nothing is blocked
    leds[0] = CRGB(10, 10, 10);
    leds[1] = CRGB(10, 10, 10);
    leds[2] = CRGB(10, 10, 10);
    leds[3] = CRGB(10, 10, 10);
    leds[4] = CRGB(10, 10, 10);
    leds[5] = CRGB::Black;
    FastLED.show();

  } else if (val == '2') {
    ServoBlock.write(65);           // P is blocked
    leds[0] = CRGB(10, 10, 10);
    leds[1] = CRGB(10, 10, 10);
    leds[2] = CRGB(10, 10, 10);
    leds[3] = CRGB(10, 10, 10);
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    FastLED.show();

  } else if (val == 'd') {
    leds[2] = CRGB(255, 255, 255);  // D selected
    fill_solid(leds2, NUM_LEDS2, CRGB :: White);
    FastLED.show();

  } else if (val == 'a') {
    leds[3] = CRGB(255, 100, 0);    // A selected
    fill_solid(leds2, NUM_LEDS2, CRGB (255, 100, 0));
    FastLED.show();

  } else if (val == 'p') {
    leds[4] = CRGB::Purple;         // P selected
    fill_solid(leds2, NUM_LEDS2, CRGB (149, 0, 255));
    FastLED.show();

  } else if (val == 'r') {
    leds[0] = CRGB(255, 255, 255);  // R selected
    fill_solid(leds2, NUM_LEDS2, CRGB :: White);
    FastLED.show();

  } else if (val == 'n') {
    leds[1] = CRGB(255, 255, 255);  // N selected
    leds[5] = CRGB(10, 10, 10);     // Park available
    fill_solid(leds2, NUM_LEDS2, CRGB :: White);
    FastLED.show();

    //Let's see if this works!
    butflag = 0;
    if (digitalRead(button) == HIGH) {               // if park button is pressed:
      if (butflag == 0) {                            // ... and previous mode was neutral, it goes into park
        butflag = 1;
        leds[5] = CRGB(255, 255, 255);
      }
      else {                                         // ... and previous mode was park, it goes into neutral
        butflag = 0;
        leds[5] = CRGB(10, 10, 10);
      }
      delay(100);                                    // wait for the hardware to settle
    } //end of led button

  } else if (val == 'f') {          // Force Feedback!
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 200);

  } else if (val == 'g') {          // Force Feedback off
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);

  } else if (val == '8') {          // Force Feedback!
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 150);

  } else if (val == '9') {          // Force Feedback off
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 200);

  } else if (val == '0') {          // Force Feedback off
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 240);

  }
  
  //If park button doesn't work
  else if (val == 'k') {          // Park selected
    leds[5] = CRGB(255, 255, 255);
    leds[1] = CRGB(10, 10, 10);
    fill_solid(leds2, NUM_LEDS2, CRGB :: Black);
    FastLED.show();
    
  }
 

}
