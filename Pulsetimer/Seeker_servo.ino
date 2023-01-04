
// License: Attribution 4.0 International (CC BY 4.0)

#include <Adafruit_GFX.h>     //OLED
#include <Adafruit_SSD1306.h> // for more info on how to use it refer to https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
#include <Servo.h>
#include <Wire.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 00; // variable to store the servo position

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const byte leftPin =
    0; // 0 is also marked as RX1 on Arduino Micro Pro, RX0 on Nano Every
const byte rightPin = 2;
const byte servo_pin = 15;

volatile uint32_t left_time_fall, left_time_rise;
volatile uint32_t right_time_fall, right_time_rise;
bool left_armed = false;
bool left_end_pulse = false;
bool right_armed = false;
bool right_end_pulse = false;

// const byte buttonPin    = 10; // I chose 10 because on pro Micro it's at the
// corner bool button=false; //button status

#define LED_right 9
#define LED_left 10
#define LED_turnleft 16
#define LED_turnright 8

void loop() {
	if (left_armed && left_end_pulse && right_armed && right_end_pulse) {
		//    Serial.write((uint8_t*) &stack[--stack_top], 4);
		volatile uint32_t pulse_length = left_time_rise - left_time_fall;
		//    Serial.write((uint8_t*) &pulse_length, 4);
		pulse_length = right_time_rise - right_time_fall -
			pulse_length; // calculate the difference:  LEFT minus RIGHT
		//    Serial.write((uint8_t*) &pulse_length, 4); //<-- keep this
		left_armed = false;
		left_end_pulse = false;
		right_armed = false;
		right_end_pulse = false;
		if ((left_time_rise - left_time_fall) >
			(right_time_rise - right_time_fall)) { // left is stronger, turn left
			digitalWrite(LED_turnright, LOW);
			digitalWrite(LED_turnleft, HIGH);
			if (myservo.read() >= 0 && myservo.read() < 180)
				pos += 1;
			myservo.write(pos);
			delay(20);
			Serial.println("<--");
			Serial.println(myservo.read());
		} else if ((left_time_rise - left_time_fall) <
				   (right_time_rise -
					right_time_fall)) { // right is stronger, turn right
			digitalWrite(LED_turnright, HIGH);
			digitalWrite(LED_turnleft, LOW);
			if (myservo.read() > 0 && myservo.read() <= 180)
				pos -= 1;
			myservo.write(pos);
			delay(20);
			Serial.println("-->");
			Serial.println(myservo.read());
		} else { // on target
			digitalWrite(LED_turnleft, HIGH);
			digitalWrite(LED_turnright, HIGH);
			Serial.println("-x-");
			Serial.println(myservo.read());
		}
	}

	//  display_counts();
}

void setup() {
	//  pinMode(buttonPin,INPUT_PULLUP);
	//  attachInterrupt(digitalPinToInterrupt(buttonPin), SetMode, FALLING); //
	//  trigger when button pressed, but not when released

	pinMode(leftPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(leftPin), left_detect, CHANGE);
	pinMode(rightPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(rightPin), right_detect, CHANGE);
	Serial.begin(115200);

	// set up the LED
	pinMode(LED_left, OUTPUT);
	pinMode(LED_right, OUTPUT);
	pinMode(LED_turnleft, OUTPUT);
	pinMode(LED_turnright, OUTPUT);
	digitalWrite(LED_turnleft, LOW);
	digitalWrite(LED_turnright, LOW);
	digitalWrite(LED_left, LOW);
	digitalWrite(LED_right, LOW);

	myservo.attach(servo_pin); // attaches the servo on pin 9 to the servo object
	sweep();
}
void left_detect() {
	if (digitalRead(leftPin) == HIGH)
		left_doRising(); // you can use direct port read to be faster -
	// http://www.arduino.cc/en/Reference/PortManipulation -
	else
		left_doFalling();
}
void left_doFalling() {
	left_time_fall = micros();
	digitalWrite(LED_left, HIGH);
	left_armed = true; // first trigger
	//  Serial.write((uint8_t*) &left_time_fall, 4);
}
void left_doRising() {
	left_time_rise = micros();
	digitalWrite(LED_left, LOW);
	left_end_pulse = true;
	//  Serial.write((uint8_t*) &left_time_rise, 4);
}
void right_detect() {
	if (digitalRead(rightPin) == HIGH)
		right_doRising(); // you can use direct port read to be faster -
	// http://www.arduino.cc/en/Reference/PortManipulation -
	else
		right_doFalling();
}
void right_doFalling() {
	right_time_fall = micros();
	digitalWrite(LED_right, HIGH);
	right_armed = true; // first trigger
	//  Serial.write((uint8_t*) &right_time_fall, 4);
}
void right_doRising() {
	right_time_rise = micros();
	digitalWrite(LED_right, LOW);
	right_end_pulse = true;
	//  Serial.write((uint8_t*) &right_time_rise, 4);
}
void sweep() { // perform the initial sweep
	//  myservo.write(0);
	//  delay(1000); //first got to zero
	pos = myservo.read();
	for (; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myservo.write(pos); // tell servo to go to position in variable 'pos'
		delay(15);          // waits 15ms for the servo to reach the position
	}
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos); // tell servo to go to position in variable 'pos'
		delay(15);          // waits 15ms for the servo to reach the position
	}
	for (pos = 0; pos <= 90; pos += 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos); // tell servo to go to position in variable 'pos'
		delay(15);          // waits 15ms for the servo to reach the position
	}

	myservo.write(90); // go to 90deg
	delay(1000);       // wait a second for this to complete
}
