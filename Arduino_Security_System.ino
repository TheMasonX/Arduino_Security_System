bool armed = false;

const byte pinLength = 4; //PIN Length (4 char)
String pin = "1234";
String enteredPinString = "";
const int pinTimeout = 15000;
unsigned long pinStart;
int invalidPinTries = 0;
const int maxInvalidPinTries = 5;

const char commandChar = '*';
const byte commandLength = 2; //Command Sequence Length (2 char)
const String changePinCommand = "42";
const String armCommand = "13";
const String disarmCommand = "23";

String enteredCommandString = "";
const int commandTimeout = 5000;
const int invalidCommandDelay = 2000;
unsigned long commandStart;

const char helpChar = 'D';

#include "serial_LCD.h"
#include "audio.h"
#include "serial.h"
#include "keypad.h"

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

void MainDisplay ()
{
  ClearDisplay();
  Serial.print("System ");
  Serial.print((armed ? "Armed" : "Offline"));
  Serial.print("\n");
  if(invalidPinTries > 0)
  {
    Serial.print("PIN Attempts: ");
    Serial.print(invalidPinTries);
    Serial.print("\n");
  }
}

void CommandEnter ()
{
  commandStart = millis();
  bool checkedCommand = false;
  enteredCommandString = "";
  int charIndex = 0;
  
  while (millis() - commandStart < commandTimeout)
  {
    char key = GetKey();
    if(charIndex < commandLength)
    {
      if(key)
      {
        enteredCommandString += key;
        charIndex++;
//        delay(5);
      }
    }
    else if(!checkedCommand)
    {
      checkedCommand = true;
      delay(keyTone.duration + commandToneDelay);
      
      if(enteredCommandString == changePinCommand)
      {
          ChangePin();
      }
      else if(enteredCommandString == armCommand)
      {
          Arm();
      }
      else if(enteredCommandString == disarmCommand)
      {
          Disarm();
      }
      else
      {
        PlayTone(incorrectCommandTone);
        ClearDisplay();
        Serial.print("Invalid Command!\n");
        delay(invalidCommandDelay);
      }
      break;
    }
    delay(1);
  }
}

bool EnterPin ()
{
  Serial.print("Enter PIN: ");
  pinStart = millis();
  bool checkedPin = false;
  bool validPin = false;
  enteredPinString = "";
  int charIndex = 0;
  
  while (millis() - pinStart < pinTimeout)
  {
    char key = GetKey();
    if(charIndex < pinLength)
    {
      if(key)
      {
        enteredPinString += key;
        Serial.print("*");
//        PrintBlock();
        charIndex++;
//        delay(5);
      }
    }
    else if(!checkedPin)
    {
      checkedPin = true;
      delay(keyTone.duration + commandToneDelay);
      
      if(enteredPinString == pin)
      {
          invalidPinTries = 0;
          validPin = true;
          PlayTone(correctPinTone);
      }
      else
      {
        invalidPinTries++;
        validPin = false;
        PlayTone(incorrectPinTone);
        
        ClearDisplay();
        Serial.print("Invalid PIN!\n");
        Serial.print("Attempts: ");
        Serial.print(invalidPinTries);
        Serial.print("\n");
        delay(invalidCommandDelay);
      }
      break;
    }
    delay(1);
  }
  return validPin;
}

void Arm ()
{
  if(!armed)
  {
    PlayTone(correctCommandTone);
    ClearDisplay();
    Serial.print("Arm System?\n");
    armed = EnterPin();
  }
  else
  {
    PlayTone(incorrectCommandTone);
    ClearDisplay();
    Serial.print("Can't Arm System\n");
    Serial.print("Already Armed\n");
    delay(invalidCommandDelay);
  }
}

void Disarm ()
{
  if(armed)
  {
    PlayTone(correctCommandTone);
    ClearDisplay();
    Serial.print("Disarm System?\n");
    armed = !EnterPin();
  }
  else
  {
    PlayTone(incorrectCommandTone);
    ClearDisplay();
    Serial.print("Can't Disarm\n");
    Serial.print("Already Disarmed\n");
    delay(invalidCommandDelay);
  }
}

void ChangePin ()
{
  PlayTone(correctCommandTone);
  ClearDisplay();
  Serial.print("Change PIN?\n");
  Serial.print("A To Continue\n");
  pinStart = millis();
  while (millis() - pinStart < pinTimeout)
  {
    char key = GetKey();
    if(key)
    {
      delay(keyTone.duration + commandToneDelay);
      
      if(key == 'A')
      {
        PlayTone(correctCommandTone);
        ClearDisplay();
        Serial.print("Change Confirmed\n");
        if(EnterPin())
        {
          delay(100);
          ClearDisplay();
          Serial.print("Old PIN Correct\n");
          EnterNewPin();
        }
        break;
      }
      else
      {
        PlayTone(incorrectCommandTone);
        ClearDisplay();
        Serial.print("Change Cancelled\n");
        delay(invalidCommandDelay);
        break;
      }
    }
  }
  
}

void EnterNewPin ()
{
  Serial.print("New PIN: ");
  
  pinStart = millis();
  enteredPinString = "";
  int charIndex = 0;
  
  while (millis() - pinStart < pinTimeout)
  {
    char key = GetKey();
    if(charIndex < pinLength)
    {
      if(key)
      {
        enteredPinString += key;
        Serial.print(key);
        charIndex++;
//        delay(5);
      }
    }
    else
    {
      delay(keyTone.duration + commandToneDelay);
      pin = enteredPinString;
      invalidPinTries = 0;
      PlayTone(correctPinTone);

      delay(300);

      ClearDisplay();
      Serial.print("PIN Changed!\n");
      delay(invalidCommandDelay);
      break;
    }
    delay(1);
  }
}
