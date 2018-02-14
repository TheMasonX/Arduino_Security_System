#include <Key.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};


byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


const byte pinLength = 4; //PIN Length (4 char)
char pin[pinLength] = {
  '1', '2', '3', '4'
};
char enteredPin[pinLength];

const char commandChar = '*';
const byte commandLength = 2; //Command Sequence Length (2 char)
const char changePinCommand[commandLength] = {
  '2', '3'
};
//char enteredCommand[commandLength];
String enteredCommandString = "";
const int commandTimeout = 5000;
int commandStart;

#include "pitches.h"
int speakerOut = 11;
int keyTone = NOTE_A5;
int keyToneDuration = 80;

int correctCommandTone = NOTE_C5;
int incorrectCommandTone = NOTE_C3;
int commandToneDuration = 400;
int commandToneDelay = 5;
int serialToneSelect;
String serialIn = "";
int serialInInt;

void PlayTone (int pitch, int duration)
{
  tone(speakerOut, pitch, duration);
}

char GetKey ()
{
  char key = keypad.getKey();
  if(key)
  {
    PlayTone(keyTone, keyToneDuration);
  }
  return key;
}

void SetPitch ()
{
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar))
    {
      serialIn += (char)inChar;
    }
    else if (inChar == '\n')
    {
      if(serialIn.length() > 0)
      {
        serialInInt = serialIn.toInt();
        Serial.println("");
        switch (serialToneSelect)
        {
          default:
          case 'k':
          Serial.print("Changed the Key Press tone from ");
          Serial.print(keyTone);
          PlayTone(serialInInt, keyToneDuration);
          keyTone = serialInInt;
          break;
          case 'c':
          Serial.print("Changed the Correct Command entered tone from ");
          Serial.print(correctCommandTone);
          PlayTone(serialInInt, commandToneDuration);
          correctCommandTone = serialInInt;
          break;
          case 'i':
          Serial.print("Changed the Incorrect Command entered tone from ");
          Serial.print(incorrectCommandTone);
          PlayTone(serialInInt, commandToneDuration);
          incorrectCommandTone = serialInInt;
          break;
        }
        
        Serial.print(" to ");
        Serial.println(serialInInt);
      }
      
      serialIn = "";
    }
    else
    {
      serialToneSelect = inChar;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Security System Booted...");
  Serial.println("Valid Commands Are:");
  Serial.print("*");
  Serial.print(String(changePinCommand));
  Serial.println(" - Change Pin");
}

void loop()
{
  SetPitch();
  char key = GetKey();
  
  if (key)
  {
    Serial.println(key);
  }

  if(key == commandChar)
  {
    Serial.print("Command Entered: ");
    commandStart = millis();
    bool checkedCommand = false;
    enteredCommandString = "";
    int charIndex = 0;
    
    while (millis() < commandStart + commandTimeout)
    {
      key = GetKey();
      if(charIndex < commandLength)
      {
        if(key)
        {
//          enteredCommand[charIndex] = key;
          enteredCommandString += key;
          Serial.print(key);
          charIndex++;
          delay(5);
        }
      }
      else if(!checkedCommand)
      {
        checkedCommand = true;
        Serial.println("");
//        Serial.println(enteredCommandString);
//        Serial.print("Entered Command: ");
//        Serial.println(String(enteredCommand));
        if(enteredCommandString == String(changePinCommand))
        {
            delay(keyToneDuration + commandToneDelay);
            PlayTone(correctCommandTone, commandToneDuration);
            Serial.println("Change Pin Command");
        }
        else
        {
          delay(keyToneDuration + commandToneDelay);
          PlayTone(incorrectCommandTone, commandToneDuration);
          Serial.println("Invalid Command");
        }
        break;
      }
      
      delay(1);
    }
    
  }
}
