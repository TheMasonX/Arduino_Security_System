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

String enteredCommandString = "";


const char helpChar = 'D';

#include "serial_LCD.h"
#include "audio.h"
#include "serial.h"
#include "keypad.h"
#include "menus.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  MainDisplay();
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


