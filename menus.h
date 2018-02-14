const int commandTimeout = 5000;
unsigned long commandStart;

const int pinTimeout = 15000;
unsigned long pinStart;

const int valEnterTimeout = 15000;
unsigned long valEnterStart;

const int notificationDelay = 2000;

int helpWait = 3000;
bool helpDisplayed = false;

void MainDisplay ()
{
  ClearDisplay();
  Print("System ");
  Println((armed ? "Armed" : "Disarmed"));
  if(invalidPinTries > 0)
  {
    Println("PIN Attempts: " + String(invalidPinTries));
  }
}

void DebugDisplay ()
{
  helpDisplayed = true;
  ClearDisplay();
  if(lightLevel < lightSensitivity)
  {
    Print((lightLevel < lightSensitivity) ? "Dark: " : "Light: ");
    Println(String(lightLevel) + "                 ");
  }
  delay(helpWait);
  
  ClearDisplay();
  helpDisplayed = false;
}

void PrintCommand (String command, String description)
{
  Println("*" + command + " " + description);
}

void PrintHelp ()
{
  helpDisplayed = true;
  ClearDisplay();
  Println("Commands Are:");
  PrintCommand(changePinCommand, "Change Pin");
  delay(helpWait);
  
  ClearDisplay();
  PrintCommand(armCommand, "Arm");
  PrintCommand(disarmCommand, "Disarm");
  delay(helpWait);
  
  ClearDisplay();
  PrintCommand(lightSensitivityCommand, "Light Sens.");
  PrintCommand(lightLevelCommand, "Light Level");
  delay(helpWait);
  
  ClearDisplay();
  helpDisplayed = false;
}

bool EnterPin ()
{
  Print("Enter PIN: ");
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
        Print("*");
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
        Println("Invalid PIN!");
        Println("PIN Attempts: " + String(invalidPinTries));
        delay(notificationDelay);
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
    Println("Arm System?");
    armed = EnterPin();
  }
  else
  {
    PlayTone(incorrectCommandTone);
    ClearDisplay();
    Println("Can't Arm System");
    Println("Already Armed");
    delay(notificationDelay);
  }
}

void Disarm ()
{
  if(armed)
  {
    PlayTone(correctCommandTone);
    ClearDisplay();
    Println("Disarm System?");
    armed = !EnterPin();
  }
  else
  {
    PlayTone(incorrectCommandTone);
    ClearDisplay();
    Println("Can't Disarm");
    Println("Already Disarmed");
    delay(notificationDelay);
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
        Print(key);
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
      Println("PIN Changed!");
      delay(notificationDelay);
      break;
    }
    delay(1);
  }
}

void ChangePin ()
{
  PlayTone(correctCommandTone);
  ClearDisplay();
  Println("Change PIN?");
  Println("A To Continue...");
  commandStart = millis();
  while (millis() - commandStart < commandTimeout)
  {
    char key = GetKey();
    if(key)
    {
      delay(keyTone.duration + commandToneDelay);
      
      if(key == 'A')
      {
        PlayTone(correctCommandTone);
        ClearDisplay();
        Println("Change Confirmed");
        if(EnterPin())
        {
          delay(100);
          ClearDisplay();
          Println("Old PIN Correct");
          EnterNewPin();
        }
        break;
      }
      else
      {
        PlayTone(incorrectCommandTone);
        ClearDisplay();
        Println("Change Cancelled");
        delay(notificationDelay);
        break;
      }
    }
  }
}

int GetValue (int valueLength, int minValue, int maxValue)
{
  SetCursor(0,1);
  Println("New:               ");
  valEnterStart = millis();
  int digitIndex = 0;
  int value = 0;
  while (millis() - valEnterStart < valEnterTimeout)
  {
    char key = GetKey();
    if(digitIndex < valueLength)
    {
      if(key)
      {
        if(isDigit(key))
        {
          value *= 10;
          value += String(key).toInt();
          SetCursor(0,1);
          Println("New: " + String(value) + "               ");
          digitIndex++;
        }
        else if(key == '#')
        {
          digitIndex = 100;
        }
      }
    }
    else
    {
      break;
    }
  }
  
  if(digitIndex == 0)
  {
    return minValue - 1;
  }
  
  value = constrain(value, minValue, maxValue);
  SetCursor(0,1);
  Print("New: " + String(value) + "             ");
  delay(keyTone.duration + commandToneDelay);
  PlayTone(correctCommandTone);
  return value;
}

void ChangeLightSensitivity ()
{
  PlayTone(correctCommandTone);
  ClearDisplay();
  Println("Changing Light");
  Println("Sensitivity...");
  delay(notificationDelay / 2);

  ClearDisplay();
  Println("Current: " + String(lightSensitivity));
  int newVal = GetValue (4, 0, 1023);
  if(newVal > 0)
  {
    lightSensitivity = newVal;
    ClearDisplay();
    delay(1);
    SetCursor(0,0);
    Println("New Light Sens:");
    Println(String(lightSensitivity) + "          ");
    delay(notificationDelay);
  }
}

void DisplayLightLevel ()
{
  PlayTone(correctCommandTone);
  displayLightLevel = !displayLightLevel;
  ClearDisplay();
//Println("123456789012345");
  Println("Light Level:");
  Println((displayLightLevel ? "Displayed" : "Hidden"));
  delay(notificationDelay);
}

void CommandEnter ()
{
  ClearDisplay();
  Println("Enter Command");
//  Print("*");
  
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
//        Print(key);
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
      else if(enteredCommandString == lightSensitivityCommand)
      {
          ChangeLightSensitivity();
      }
      else if(enteredCommandString == lightLevelCommand)
      {
          DisplayLightLevel();
      }
      else
      {
        PlayTone(incorrectCommandTone);
        ClearDisplay();
        Println("Invalid Command!");
        delay(notificationDelay);
      }
      break;
    }
    delay(1);
  }
}
