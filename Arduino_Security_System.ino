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
String enteredCommandString = "";
const int commandTimeout = 5000;
unsigned long commandStart;

#include "audio.h"
#include "serial.h"
#include "keypad.h"

void SetCursor (int x, int y)
{
  SetCursorX(x);
  SetCursorY(y);
}

void SetCursorX (int x)
{
  x = constrain(x, 0, 15);
  Serial.print(char(x + 15));
}

void SetCursorY (int y)
{
  y = constrain(y, 0, 1);
  Serial.print(char(y + 1));
}

void ClearDisplay ()
{
  Serial.print(char(3));
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  ClearDisplay();
  Serial.print("System Online...\n");
//  PrintHelp();
}

void loop()
{
  SerialRead();
  char key = GetKey();

  if(key == commandChar)
  {
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
          enteredCommandString += key;
          charIndex++;
          delay(5);
        }
      }
      else if(!checkedCommand)
      {
        checkedCommand = true;
        if(enteredCommandString == String(changePinCommand))
        {
            delay(keyToneDuration + commandToneDelay);
            PlayTone(correctCommandTone, commandToneDuration);
            ClearDisplay();
            Serial.print("Change PIN?\n");
            Serial.print("A To Confirm\n");
        }
        else
        {
          delay(keyToneDuration + commandToneDelay);
          PlayTone(incorrectCommandTone, commandToneDuration);
          ClearDisplay();
          Serial.print("Invalid Command!\n");
        }
        break;
      }
      
      delay(1);
    }
    
  }
}
