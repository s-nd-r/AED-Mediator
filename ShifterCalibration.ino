// potRND = pin A1
// potDAP = pin A0
int pin = A0;
int value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);

}

void loop() {
  value = analogRead(pin);
  Serial.println(value);
}
