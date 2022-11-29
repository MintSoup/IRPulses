void setup() {
  // Interrupt pin: used for triggering the conversion.
  // Feed the maximum of all phototransistor voltages here;
	pinMode(2, INPUT);
  // Auxillary pin for measuring how long the total
  // conversion sequence lasts. 
  pinMode(3, OUTPUT);
  // Inputs
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), trigger, RISING);
}

unsigned int samples = 0;
unsigned const int SAMPLES_TO_TAKE = 3000; // experimentally change this later

unsigned long times[4];

void trigger() { 
  if (samples == 0) // ignore non-single triggers
    samples = SAMPLES_TO_TAKE;
}

void eoc() {
  // control servos, send stuff over to pc, etc...
}


void loop() {
  times[0] = 0;
  times[1] = 0;
  times[2] = 0;
  times[3] = 0;

  if (samples > 0) {
    // PORTD holds 
    // the state of digital pins 0-7.

    PORTD |= (1 << 3); // Write HIGH to pin 3 
    while(samples-- > 0) {
      // for each pin, extract the corresponding 
      // bit and add it to the correct counter.
      int pd = PORTD;
      times[0] += (pd & (1 << 4)) >> 4;
      times[1] += (pd & (1 << 5)) >> 5;
      times[2] += (pd & (1 << 6)) >> 6;
      times[3] += (pd & (1 << 7)) >> 7;
    }
    PORTD &= (1 << 3); // Write LOW to pin 3 
    eoc();
  }
}
