#include <CmdMessenger.h>  // CmdMessenger

// ------------- CONSTS -------------
const int VOLTAGE		= A0;
const int BUZZER		= A5;
const int SW_BOX		= 2;
const int SW_TESTER		= 4;
const int COIL			= 5;
const int MOTOR			= 6;
const int NOK			= 8;
const int OK			= 9;

// ------------- VARIABLES -------------
int MOTOR_SPEED			= 250;
int QTY_TESTS			= 200;
int DELAYS				= 1;

bool coilState = 0;

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// ------------- COMMANDS -------------
enum
{
	kAcknowledge,
	kError,
	kSetLed, // Command to request led to be set in specific state
	kSetLedFrequency,
	kCoil,
};


// Blinking led variables 
const int kBlinkLed = 13;  // Pin of internal Led
bool ledState = 1;   // Current state of Led
float ledFrequency = 1.0; // Current blink frequency of Led

unsigned long intervalOn;
unsigned long intervalOff;
unsigned long prevBlinkTime = 0;


// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
	// Attach callback methods
	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(kSetLed, OnSetLed);
	cmdMessenger.attach(kSetLedFrequency, OnSetLedFrequency);
	cmdMessenger.attach(kCoil, OnCoil);
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
	cmdMessenger.sendCmd(kError, "Command without attached callback");
}

// Callback function that sets led on or off
void OnSetLed()
{
	// Read led state argument, interpret string as boolean
	ledState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kAcknowledge, ledState);
}

// Callback function that sets leds blinking frequency
void OnSetLedFrequency()
{
	// Read led state argument, interpret string as boolean
	ledFrequency = cmdMessenger.readFloatArg();
	// Make sure the frequency is not zero (to prevent divide by zero)
	if (ledFrequency < 0.001) { ledFrequency = 0.001; }
	// translate frequency in on and off times in miliseconds
	intervalOn = (500.0 / ledFrequency);
	intervalOff = (1000.0 / ledFrequency);
	cmdMessenger.sendCmd(kAcknowledge, ledFrequency);
}

void OnCoil()
{
	coilState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kCoil, coilState);
}

// Setup function
void setup()
{
	// Listen on serial connection for messages from the PC
	Serial.begin(115200);

	// Adds newline to every command
	//cmdMessenger.printLfCr();   

	// Attach my application's user-defined callback methods
	attachCommandCallbacks();

	// Send the status to the PC that says the Arduino has booted
	// Note that this is a good debug function: it will let you also know 
	// if your program had a bug and the arduino restarted  
	cmdMessenger.sendCmd(kAcknowledge, "Arduino has started!");

	// set pin for blink LED
	pinMode(kBlinkLed, OUTPUT);

	// ----------------- SET PINS -----------------
	pinMode(SW_BOX, INPUT_PULLUP);
	pinMode(SW_TESTER, INPUT_PULLUP);
	pinMode(COIL, OUTPUT);
	pinMode(MOTOR, OUTPUT);
	pinMode(OK, OUTPUT);
	pinMode(NOK, OUTPUT);

}

// Loop function
void loop()
{
	// Process incoming serial data, and perform callbacks
	cmdMessenger.feedinSerialData();
	delay(10);
	blinkLed();
	coil();
}

// Returns if it has been more than interval (in ms) ago. Used for periodic actions
void blinkLed() 
{
	if (millis() - prevBlinkTime > intervalOff) {
		// Turn led off during halfway interval
		prevBlinkTime = millis();
		digitalWrite(kBlinkLed, LOW);
	}
	else if (millis() - prevBlinkTime > intervalOn) {
		// Turn led on at end of interval (if led state is on)
		digitalWrite(kBlinkLed, ledState ? HIGH : LOW);
	}
}

void coil()
{
	digitalWrite(kBlinkLed, coilState ? HIGH : LOW);
}
