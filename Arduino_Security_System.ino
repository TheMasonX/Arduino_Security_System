bool armed = false;

const byte pinLength = 4; //PIN Length (4 char)
String pin = "1234";
String enteredPinString = "";

int invalidPinTries = 0;
const int maxInvalidPinTries = 5;

const char commandChar = '*';
const byte commandLength = 2; //Command Sequence Length (2 char)
const String changePinCommand = "42";
const String armCommand = "13";
const String disarmCommand = "23";
const String lightSensitivityCommand = "55";
const String lightLevelCommand = "56";

String enteredCommandString = "";

float lightLevel;
bool isLight;
int lightSensitivity = 250;
int lightCheckFrequency = 4000;
int iterations = 10;
int iterationDelay = 2;
bool displayLightLevel = false;

bool alarmOn;
int alarmFrequency = 2000;

const char helpChar = 'D';

#include "serial_LCD.h"
#include "audio.h"
#include "keypad.h"
#include "menus.h"
#include "timer.h"

ISR(TIMER1_COMPA_vect)
{
  CheckLightLevel();
}

ISR(TIMER1_COMPB_vect)
{
  Alarm();
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  MainDisplay();
  SetupTimers(lightCheckFrequency, alarmFrequency);
//  PrintHelp();
}

void loop()
{
//  SerialRead();
  char key = GetKey();

  if(key == commandChar)
  {
    CommandEnter();
    MainDisplay();
  }
  else if (key == helpChar)
  {
    PrintHelp();
    MainDisplay();
  }
}

float GetLightLevel ()
{
  int sum = 0;
  for(int i = 0; i < iterations; i++)
  {
    sum += analogRead(A0);
    delay(iterationDelay);
  }
  return sum / (float)iterations;
}

void CheckLightLevel ()
{
  lightLevel = GetLightLevel();
  isLight = lightLevel > lightSensitivity;

  if(displayLightLevel)
  {
    SetCursor(0,1);
    Print((isLight ? "Light: " : "Dark: "));
    Println(String(lightLevel) + "            ");
  }
}

void Alarm ()
{
  if(isLight && armed)
  {
    PlayTone(alarmTone);
  }
}

