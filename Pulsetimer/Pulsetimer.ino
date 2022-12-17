void setup() {
  Serial.begin(115200);
  // Interrupt pins: used for triggering the conversion.
	pinMode(2, INPUT);
  pinMode(3, INPUT);
  // For blinking th eled
  pinMode(13, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(2), trigger1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), trigger2, CHANGE);

}

unsigned long value1 = 0;
unsigned long beg1 = 0;
unsigned long value2 = 0;
unsigned long beg2 = 0;

void trigger1() {
  if (digitalRead(2) == 0) {
    value1 = micros() - beg1;
    digitalWrite(13, LOW);
  } else {
    beg1 = micros();
    digitalWrite(13, HIGH);
  }
}
void trigger2() {
  if (digitalRead(3) == 0) {
    value2 = micros() - beg2;
    digitalWrite(13, LOW);
  } else {
    beg2 = micros();
    digitalWrite(13, HIGH);
  }
}

void loop() {
  if (value1 != 0) {
    Serial.print("1 ");
    Serial.println(value1);
    value1 = 0;
  }
  if (value2 != 0) {
    Serial.print("2 ");
    Serial.println(value2);
    value2 = 0;
  }
}
