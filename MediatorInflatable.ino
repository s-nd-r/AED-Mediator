int periodInflate = 5000;                // inflate time in milliseconds
int periodDeflate = 5000;                // deflate time in milliseconds
unsigned long time_now = 0;
int airFlow = 0;                  // 0=default, 1=inflate, 2=deflate

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);            // pin for inflate relay
  pinMode(12, OUTPUT);            // pin for deflate relay
  pinMode(13, OUTPUT);            // solenoid valve
}

void loop() {
  airFlow = Serial.read();
  if (airFlow = 1) {                        // if inflate signal is sent...
    if (millis() >= time_now + periodInflate) {    // timer starts counting and if it's below the period threshold...
      time_now += period;
      digitalWrite(11, HIGH);               // the relay (and thus the pump) is turned on
    } else {
      digitalWrite(11, LOW);                // after period ends the pump is turned off
    }
  }

  if (airFlow = 2) {                        // if deflate signal is sent...
    if (millis() >= time_now + periodDeflate) {    // timer starts counting and if it's below the period threshold...
      time_now += period;
      digitalWrite(13, HIGH);               // open solenoid valve to allow deflation
      digitalWrite(12, HIGH);               // the relay (and thus the pump) is turned on
    } else {
      digitalWrite(13, LOW);                // close solenoid valve
      digitalWrite(12, LOW);                // after period ends the pump is turned off
    }
