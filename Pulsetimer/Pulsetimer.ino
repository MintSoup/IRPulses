void setup() {
	// Start serial for sending data to PC
	Serial.begin(115200);
	// Interrupt pins
	// The signal from the comparators is fed into these pins.
	// When a state change occurs (rising or falling edge),
	// an interrupt is triggered. By measuring the time between
	// the rising and falling interrupts we calculate
	// the strenght of the pulse.
	pinMode(2, INPUT);
	pinMode(3, INPUT);

	// Auxillary LED pin
	pinMode(13, OUTPUT);

	// Interrupts
	attachInterrupt(digitalPinToInterrupt(2), trigger1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(3), trigger2, CHANGE);
}

// Variables for measuring timing.
// beg{1, 2} store the time of the rising edge interrupts,
// value{1, 2} store the length of the last pulse.

// Everything is in microseconds.

unsigned long value1 = 0;
unsigned long beg1 = 0;
unsigned long value2 = 0;
unsigned long beg2 = 0;

// Pin interrupts

void trigger1() {
	// if its a falling edge,
	// calculate the time since the rising
	// edge and store it inside value1.
	if (digitalRead(2) == 0) {
		value1 = micros() - beg1;
		digitalWrite(13, LOW);
	} else {
		// otherwise, store the current time in
		// beg1 for later use
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
	// if we had a pulse, inform
	// the PC about it
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
