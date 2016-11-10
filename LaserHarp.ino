/**
    Arduino code used for a laser harp. What you need to run this:
    - 1 Laser
    - 1 Stepper Motor
    - 1 Mirror
    - 1 Light sensor
    - 1 Buzzer

    @author Joost Mul
    @version 0.1
*/

int sensor      = 8;    // Sensitivity of the Light Sensor
int delaylaser  = 5;    // Miliseconds the laser should be turned on. If you increase this, the laser will be brighter, but the harp will be less fluid
int delaymotor  = 3;    // Miliseconds the motor should be delayed after laser interaction. This variable affects the speed, and fluidity of the harp.

int laserPin    =  7;   // GPIO pin the laser is on

int motorPin1   = 8;    // 1st GPIO pin for the stepper motor
int motorPin2   = 9;    // 2nd GPIO pin for the stepper motor
int motorPin3   = 10;   // 3rd GPIO pin for the stepper motor
int motorPin4   = 11;   // 4rd GPIO pin for the stepper motor

int buzzerPin   = 13;   // GPIO pin the busser is on

// Collection of steps in order. Every step exists from the following data:
// stepActivated, hertz, motorPin1Value, motorPin2Value, motorPin3Value, motorPin4Value
int steps[16][6] = {
	{0, 261, HIGH, LOW, LOW, LOW} // STEP 1

	{0, 277, LOW, HIGH, LOW, LOW} // STEP 2
	{0, 293, LOW, LOW, HIGH, LOW} // STEP 3
	{0, 311, LOW, LOW, LOW, HIGH} // STEP 4
	{0, 329, HIGH, LOW, LOW, LOW} // STEP 5
	{0, 349, LOW, HIGH, LOW, LOW} // STEP 6
	{0, 369, LOW, LOW, HIGH, LOW} // STEP 7
	{0, 392, LOW, LOW, LOW, HIGH} // STEP 8

	{0, 415, LOW, LOW, HIGH, LOW} // STEP 9

	{0, 392, LOW, HIGH, LOW, LOW} // STEP 8
	{0, 369, HIGH, LOW, LOW, LOW} // STEP 7
	{0, 349, LOW, LOW, LOW, HIGH} // STEP 6
	{0, 329, LOW, LOW, HIGH, LOW} // STEP 5
	{0, 311, LOW, HIGH, LOW, LOW} // STEP 4
	{0, 293, HIGH, LOW, LOW, LOW} // STEP 3
	{0, 277, LOW, LOW, LOW, HIGH} // STEP 2
};

/**
    Setup the arduino program
*/
void setup()
{
    // Enable the pins used by the Stepper Motor
	pinMode(motorPin1, OUTPUT);
	pinMode(motorPin2, OUTPUT);
	pinMode(motorPin3, OUTPUT);
	pinMode(motorPin4, OUTPUT);

	// Enable the pin used by the laser
	pinMode(laserPin, OUTPUT);

	// Enable the pin used by the buzzer
	pinMode(buzzerPin, OUTPUT);

	// Enable the blinking led
	pinMode(13, OUTPUT);
}

/**
    Performs a step/note for the laser harp.

    @param stepActive       Whether or not the step is currently active
    @param note             The amount of hertz the buzzer should play when it is triggered
    @param motorPin1Val     Value of the first GPIO pin of the stepper motor when this note triggers
    @param motorPin2Val     Value of the second GPIO pin of the stepper motor when this note triggers
    @param motorPin3Val     Value of the third GPIO pin of the stepper motor when this note triggers
    @param motorPin4Val     Value of the fourth GPIO pin of the stepper motor when this note triggers
*/
void performStep(int &stepActive, int note, int motorPin1Val, int motorPin2Val, int motorPin3Val, int motorPin4Val)
{
    // Enable the laser
	digitalWrite(laserPin, HIGH);

	// Wait a certain amount of miliseconds
	delay(delaylaser);

    // If the sensor is picking something up and the step is not yet active, perform a buzzer and set the step to active
	if ( (analogRead(0) > sensor ) && (stepActive == 0) ) {
		digitalWrite(13, HIGH);
		tone(buzzerPin, note);
		stepActive++;
	// If the sensor is not picking up anything, disable this step and kill the buzzer
	} else if(analogRead(0) < sensor ) {
		digitalWrite(13, LOW);
		noTone(buzzerPin);
		stepActive = 0;
	}

    // Disable the laser
	digitalWrite(laserPin, LOW);

    // Update the Stepper Motor pins
	digitalWrite(motorPin1, motorPin1Val);
	digitalWrite(motorPin2, motorPin2Val);
	digitalWrite(motorPin3, motorPin3Val);
	digitalWrite(motorPin4, motorPin4Val);

	// Give the motor some time
	delay(delaymotor);
}

/**
    Loop through all Laser Harp steps
*/
void loop()
{
	for (int i = 0; i < 16; i++) {
		var data = &steps[i];
		performStep(data[0], data[1], data[2], data[3], data[4], data[5]);
	}
}
