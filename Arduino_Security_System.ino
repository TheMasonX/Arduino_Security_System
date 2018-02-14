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


const int pinLength = 4; //PIN Length (4 char)
char pin[pinLength] = {
  '1', '2', '3', '4'
};
char enteredPin[pinLength];

const char commandChar = '*';
const int commandLength = 2; //Command Sequence Length (2 char)
const char changePinCommand[commandLength] = {
  '2', '3'
};
char enteredCommand[commandLength];
const int commandTimeout = 5000;
int commandStart;

#include "pitches.h"
int speakerOut = 11;
int keyTone = NOTE_C4;
int keyToneDuration = 100;
String inString = "";

char GetKey ()
{
  char key = keypad.getKey();
  if(key)
  {
    tone(speakerOut, keyTone, keyToneDuration);
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
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n')
    {
      keyTone = inString.toInt();
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
    int charIndex = 0;
    
    while (millis() < commandStart + commandTimeout)
    {
      key = GetKey();
      if(charIndex < commandLength)
      {
        if(key)
        {
          Serial.print(key);
          enteredPin[charIndex] = key;
          charIndex++;
        }
      }
      else if(!checkedCommand)
      {
        checkedCommand = true;
        Serial.println("");
        Serial.print("Entered Command: ");
        Serial.println(String(enteredPin));
        if(enteredPin == changePinCommand)
        {
            checkedCommand = true;
            Serial.println("Change Pin Command");
        }
      }
    }
    
  }
}
