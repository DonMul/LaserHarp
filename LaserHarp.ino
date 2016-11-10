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

const int sensor        = 8;    // Sensitivity of the Light Sensor
const int delaylaser    = 5;    // Miliseconds the laser should be turned on. If you increase this, the laser will be brighter, but the harp will be less fluid
const int delaymotor    = 3;    // Miliseconds the motor should be delayed after laser interaction. This variable affects the speed, and fluidity of the harp.

const int octavePinUp   = 6;    // Send a signal over this port to up the octave number
const int octavePinDown = 5;    // Send a signal over this port to lower the octave number

const int laserPin      = 7;    // GPIO pin the laser is on

const int motorPin1     = 8;    // 1st GPIO pin for the stepper motor
const int motorPin2     = 9;    // 2nd GPIO pin for the stepper motor
const int motorPin3     = 10;   // 3rd GPIO pin for the stepper motor
const int motorPin4     = 11;   // 4rd GPIO pin for the stepper motor

const int buzzerPin     = 13;   // GPIO pin the busser is on

double ladder[12] = {
    130.82, // STEP 1
    138.59, // STEP 2
    146.83, // STEP 3
    155.56, // STEP 4
    164.81, // STEP 5
    174.61, // STEP 6
    185,    // STEP 7
    196,    // STEP 8
    207.65, // STEP 9
    220,    // STEP 10
    233.08, // STEP 11
    246.94  // STEP 12
};

int octave = 2;

int previousUpState = LOW;
int previousDownState = LOW;

// Collection of steps in order. Every step exists from the following data:
// stepActivated, step in the ladder, motorPin1Value, motorPin2Value, motorPin3Value, motorPin4Value
int steps[16][6] = {
	{0, 0, HIGH, LOW, LOW, LOW} // STEP 1

	{0, 1, LOW, HIGH, LOW, LOW} // STEP 2
	{0, 2, LOW, LOW, HIGH, LOW} // STEP 3
	{0, 3, LOW, LOW, LOW, HIGH} // STEP 4
	{0, 4, HIGH, LOW, LOW, LOW} // STEP 5
	{0, 5, LOW, HIGH, LOW, LOW} // STEP 6
	{0, 6, LOW, LOW, HIGH, LOW} // STEP 7
	{0, 7, LOW, LOW, LOW, HIGH} // STEP 8
	{0, 8, HIGH, LOW, LOW, LOW} // STEP 9
	{0, 9, LOW, HIGH, LOW, LOW} // STEP 10
	{0, 10, LOW, LOW, HIGH, LOW} // STEP 11

	{0, 11, LOW, HIGH, LOW,  LOW} // STEP 12

	{0, 10, HIGH, LOW, LOW, LOW} // STEP 11
	{0, 9, LOW, LOW, LOW, HIGH} // STEP 10
	{0, 8, LOW, LOW, HIGH, LOW} // STEP 9
	{0, 7, LOW, HIGH, LOW, LOW} // STEP 8
	{0, 6, HIGH, LOW, LOW, LOW} // STEP 7
	{0, 5, LOW, LOW, LOW, HIGH} // STEP 6
	{0, 4, LOW, LOW, HIGH, LOW} // STEP 5
	{0, 3, LOW, HIGH, LOW, LOW} // STEP 4
	{0, 2, HIGH, LOW, LOW, LOW} // STEP 3
	{0, 1, LOW, LOW, LOW, HIGH} // STEP 2
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

	pinMode(buttonPinUp, INPUT);
	pinMode(buttonPinDown, INPUT);

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
		tone(buzzerPin, round(ladder[note] * pow(2^octave)));
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
	for (int i = 0; i < sizeof(steps); i++) {
		var data = &steps[i];
		performStep(data[0], data[1], data[2], data[3], data[4], data[5]);
	}

	shouldOctaveUp();
	shouldOctaveDown();
}

/**
    Check if octave must go up (With a max of 9)
*/
void shouldOctaveUp()
{
    upState = digitalRead(octavePinUp);

    if (upState == HIGH) {
        if (previousUpState == LOW) {
            octave = min(octave+1,9);
            previousUpState = HIGH
        }
    } else {
        previousUpState = LOW
    }
}

/**
    Check if octave must go down (With a min of 1)
*/
void shouldOctaveDown()
{
    downState = digitalRead(octavePinDown);

    if (downState == HIGH) {
            if (previousDownState == LOW) {
                octave = max(octave-1,1);
                previousDownState = HIGH
            }
        } else {
            previousDownState = LOW
        }
}
