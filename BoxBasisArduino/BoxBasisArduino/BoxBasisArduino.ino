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

//------------- STATES -------------
bool coilState			= 0;
bool motorState			= 0;

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// ------------- COMMANDS -------------
//1. Dodaj komende do enuma
enum
{
	kAcknowledge,
	kError,
	kCoil,
	kMotor,
};

// Callbacks define on which received commands we take action
//2. Przypisz callback do metody
void attachCommandCallbacks()
{
	// Attach callback methods
	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(kCoil, OnCoil);
	cmdMessenger.attach(kMotor, OnMotor);
}

// Called when a received command has no attached function
//3. Napisz funkcje
void OnUnknownCommand()
{
	cmdMessenger.sendCmd(kError, "Command without attached callback");
}

void OnCoil()
{
	coilState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kCoil, coilState);
}

void OnMotor()
{
	cmdMessenger.sendCmd(kMotor, motorState);
}

// Setup function
void setup()
{
	Serial.begin(115200);
 
	attachCommandCallbacks();

	cmdMessenger.sendCmd(kAcknowledge, "Arduino has started!");

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
	coil();
	motor();
}

// Returns if it has been more than interval (in ms) ago. Used for periodic actions
/*void blinkLed() 
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
}*/

// 4. Funkcja zmieniajaca dane pinu

void coil()
{
	digitalWrite(COIL, coilState ? HIGH : LOW);
}

void motor()
{
	digitalWrite(MOTOR, motorState ? HIGH : LOW);
}
