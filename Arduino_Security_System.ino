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
int commandStart;

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
  Serial.println("Security System Booted...");
  PrintHelp();
}

void loop()
{
  SerialRead();
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
