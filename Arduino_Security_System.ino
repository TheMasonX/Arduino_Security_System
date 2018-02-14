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
char enteredPin[pinLength] = {
  '0', '0', '0', '0'
};

const char commandChar = '*';
const int commandLength = 2; //Command Sequence Length (2 char)
const char changePinCommand[commandLength] = {
  '2', '3'
};
char enteredCommand[commandLength] = {
  '0', '0'
};
const int commandTimeout = 5000;
int commandStart;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  char key = keypad.getKey();
  
  if (key)
  {
    Serial.println(key);
  }

  if(key == commandChar)
  {
    commandStart = millis();
    bool invalidCommand = false;
    bool correctCommand = false;
    int charIndex = 0;
    while (millis() < commandStart + commandTimeout)
    {
      if(!invalidCommand && !correctCommand)
      {
        key = keypad.getKey();

        if(key)
        {
          Serial.print("Command Key ");
          Serial.println(key);
          if(key == changePinCommand[charIndex])
          {
            charIndex++;
            if(charIndex == commandLength)
            {
              correctCommand = true;
              Serial.print("Change Pin Command ");
              Serial.println(changePinCommand);
            }
          }
          else 
          {
            invalidCommand = true;
          }
        }
      }
    }
  }
}
