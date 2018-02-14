#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void ReadSerial ()
{
  if (Serial.available())
  {
    delay(100); // wait a bit for the entire message to arrive
    int x = 0;
    int y = 0;
    
    // read all the available characters
    while (Serial.available() > 0)
    {
      int inChar = Serial.read();
      if (inChar == '\n')
      {
        x = 0;
        y++;
        lcd.setCursor(x,y);
        Serial.println("~");
      }
      else if (inChar >= 15 && inChar < 32)
      {
        x = inChar - 15;
        lcd.setCursor(x,y);
      }
      else if (inChar == 1)
      {
        y = 0;
        lcd.setCursor(x,y);
      }
      else if (inChar == 2)
      {
        y = 1;
        lcd.setCursor(x,y);
      }
      else if (inChar == 3)
      {
        lcd.clear(); // clear the screen
        delay(10);
        lcd.setCursor(x,y);
      }
      else
      {
        // display each character to the LCD
        Serial.print(char(inChar));
        lcd.write(inChar);
      }
    }
  }
}

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
//  lcd.print("Hello, world!");
  Serial.begin(9600);
}

void loop()
{  
  ReadSerial();

//  lcd.setCursor(0, 1);
//  delay(500);  
//  // print the number of seconds since reset:
//  lcd.print(millis() / 1000);
}
