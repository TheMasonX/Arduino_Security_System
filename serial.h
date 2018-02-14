int serialToneSelect;
String serialIn = "";
int serialInInt;
int helpWait = 3000;
bool helpDisplayed = false;

void SerialRead ()
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
          Serial.print(keyTone.pitch);
          keyTone.pitch = serialInInt;
          PlayTone(keyTone);
          break;
          case 'c':
          Serial.print("Changed the Correct Command entered tone from ");
          Serial.print(correctCommandTone.pitch);
          correctCommandTone.pitch = serialInInt;
          PlayTone(correctCommandTone);
          break;
          case 'i':
          Serial.print("Changed the Incorrect Command entered tone from ");
          Serial.print(incorrectCommandTone.pitch);
          incorrectCommandTone.pitch = serialInInt;
          PlayTone(incorrectCommandTone);
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

void PrintCommand (String command, String description)
{
  Serial.print("*");
  Serial.print(command + " ");
  Serial.print(description + "\n");
}

void PrintHelp ()
{
  helpDisplayed = true;
  ClearDisplay();
  Serial.print("Commands Are:\n");
  PrintCommand(changePinCommand, "Change Pin");
  delay(helpWait);
  ClearDisplay();
  PrintCommand(armCommand, "Arm");
  PrintCommand(disarmCommand, "Disarm");
  delay(helpWait);
  ClearDisplay();
  helpDisplayed = false;
}

