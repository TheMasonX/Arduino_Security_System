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

void SetCursor (int x, int y)
{
  SetCursorX(x);
  SetCursorY(y);
}

void ClearDisplay ()
{
  Serial.print(char(3));
}

void ScrollLeft ()
{
  Serial.print(char(4));
}

void ScrollRight ()
{
  Serial.print(char(5));
}

void PrintBlock ()
{
  Serial.print(char(6));
}

void Print (String text)
{
  Serial.print(text);
}

void Print (char text)
{
  Print(String(text));
}

void Println (String text)
{
  Print(text + "\n");
}

void Println (char text)
{
  Println(String(text));
}

