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
int intervalCoil				= 50;

bool motorState					= 0;
unsigned long prevMotorTime		= 0;
int intervalMotor				= 250;

bool ledOKState					= 0;
unsigned long prevLedOKTime		= 0;
int intervalLedOK				= 100;

bool ledNOKState				= 0;
unsigned long prevLedNOKTime	= 0;
int intervalLedNOK				= 100;

float voltage					= 0;

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

// -------------- COMMANDS --------------
//1. Dodaj komende do enuma
enum
{
	kAcknowledge,
	kError,
	kCoil,
	kCoilTime,
	kMotor,
	kMotorTime,
	kLedOK,
	kLedNOK,
	kVoltage,
};

// Callbacks define on which received commands we take action
//2. Przypisz callback do metody
void attachCommandCallbacks()
{
	// Attach callback methods
	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(kCoil, OnCoil);
	cmdMessenger.attach(kCoilTime, OnCoilTime);
	cmdMessenger.attach(kMotor, OnMotor);
	cmdMessenger.attach(kMotorTime, OnMotorTime);
	cmdMessenger.attach(kLedOK, OnLedOK);
	cmdMessenger.attach(kLedNOK, OnLedNOK);
	cmdMessenger.attach(kVoltage, OnVoltage);
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

void OnCoilTime()
{
	intervalCoil = cmdMessenger.readInt16Arg();
	cmdMessenger.sendCmd(kCoilTime, intervalCoil);
}

void OnMotor()
{
	motorState = cmdMessenger.readBoolArg();
	cmdMessenger.sendCmd(kMotor, motorState);
}

void OnMotorTime()
{
	intervalMotor = cmdMessenger.readInt16Arg();
	cmdMessenger.sendCmd(kMotorTime, intervalMotor);
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

void OnVoltage()
{
	voltage = analogRead(VOLTAGE);
	voltage *= (5.0 / 1023.0);
	cmdMessenger.sendCmd(kVoltage, voltage);
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
	Coil();
	Motor();
	LedOK();
	LedNOK();
}

// 4. Funkcja zmieniajaca dane pinu

void Coil()
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

void Motor()
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

void LedOK()
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

void LedNOK()
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
	digitalWrite(NOK, ledNOKState ? HIGH : LOW);
}