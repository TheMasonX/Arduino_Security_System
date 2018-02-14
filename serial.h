int serialToneSelect;
String serialIn = "";
int serialInInt;

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

void PrintHelp ()
{
  Serial.println("Valid Commands Are:");
  Serial.print("*");
  Serial.print(String(changePinCommand));
  Serial.println(" - Change Pin");
  Serial.println("");
}

