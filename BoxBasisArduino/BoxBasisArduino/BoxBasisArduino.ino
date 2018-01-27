#include <CmdMessenger.h>  // CmdMessenger

// -------------- CONSTS --------------
const int VOLTAGE				= A0;
const int BUZZER				= A5;
const int SW_BOX				= 2;
const int SW_TESTER				= 4;
const int COIL					= 5;
const int MOTOR					= 6;
const int NOK					= 8;
const int OK					= 9;

//-------------- STATES --------------
bool coilState					= 0;
unsigned long prevCoilTime		= 0;
unsigned long intervalCoil		= 50;

bool motorState					= 0;
unsigned long prevMotorTime		= 0;
unsigned long intervalMotor		= 250;

bool ledOKState					= 0;
unsigned long prevLedOKTime		= 0;
unsigned long intervalLedOK		= 100;

bool ledNOKState				= 0;
unsigned long prevLedNOKTime	= 0;
unsigned long intervalLedNOK	= 100;

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// -------------- COMMANDS --------------
//1. Dodaj komende do enuma
enum
{
	kAcknowledge,
	kError,
	kCoil,
	kMotor,
	kLedOK,
	kLedNOK,
};

// Callbacks define on which received commands we take action
//2. Przypisz callback do metody
void attachCommandCallbacks()
{
	// Attach callback methods
	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(kCoil, OnCoil);
	cmdMessenger.attach(kMotor, OnMotor);
	cmdMessenger.attach(kLedOK, OnLedOK);
	cmdMessenger.attach(kLedNOK, OnLedNOK);
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
	motorState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kMotor, motorState);
}

void OnLedOK()
{
	ledOKState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kLedOK, ledOKState);
}

void OnLedNOK()
{
	ledNOKState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kLedNOK, ledNOKState);
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

	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

}

// Loop function
void loop()
{
	// Process incoming serial data, and perform callbacks
	cmdMessenger.feedinSerialData();
	delay(10);
	coil();
	motor();
	ledOK();
	ledNOK();
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
	if (millis() - prevCoilTime > intervalCoil)
	{
		prevCoilTime = millis();
		if (coilState)
		{
			coilState = false;
			cmdMessenger.sendCmd(kCoil, coilState);
		}
	}
	digitalWrite(COIL, coilState ? HIGH : LOW);
}

void motor()
{
	if (millis() - prevMotorTime > intervalMotor)
	{
		prevMotorTime = millis();
		if (motorState)
		{
			motorState = false;
			cmdMessenger.sendCmd(kMotor, motorState);
		}
	}
	digitalWrite(MOTOR, motorState ? HIGH : LOW);
}

void ledOK()
{
	if (millis() - prevLedOKTime > intervalLedOK)
	{
		prevLedOKTime = millis();
		if (ledOKState)
		{
			ledOKState = false;
			cmdMessenger.sendCmd(kLedOK, ledOKState);
		}
	}
	digitalWrite(OK, ledOKState ? HIGH : LOW);
}

void ledNOK()
{
	if (millis() - prevLedNOKTime > intervalLedNOK)
	{
		prevLedNOKTime = millis();
		if (ledNOKState)
		{
			ledNOKState = false;
			cmdMessenger.sendCmd(kLedNOK, ledNOKState);
		}
	}
	digitalWrite(OK, ledNOKState ? HIGH : LOW);
}