unsigned int clockFreq = 16000;
int prescale = 1024;
unsigned int maxVal = 65535;
unsigned int range1 = 65535;
unsigned int range2 = 0;
const float maxDelays[] = { 4.096, 32.768, 262.144, 1048.576, 4194.304 };
const int prescales[] = { 1, 8, 64, 256, 1024 };

void SetPrescale (float msDelay)
{
  msDelay = min(msDelay, maxDelays[4]); //make sure it's smaller than the max delay
  for (int i = 0; i < 5; i++)
  {
    if(msDelay <= maxDelays[i])
    {
      prescale = prescales[i];
      break;
    }
  }
  
  switch (prescale)
  {
    case 1:
      TCCR1B |= (1<<CS10);                //prescaler 1
      break;
    case 8:
      TCCR1B |= (1<<CS11);                //prescaler 8
      break;
    case 64:
      TCCR1B |= (1<<CS10) | (1<<CS11);    //prescaler 64
      break;
    case 256:
      TCCR1B |= (1<<CS12);                //prescaler 256
      break;
    case 1024:
      TCCR1B |= (1<<CS10) | (1<<CS12);    //prescaler 1024
      break;
  }
}

int CalcRange (float msDelay)
{
  return round(msDelay * clockFreq / (float)prescale) - 1;
}

void CalcTimers (float msDelay1, float msDelay2)
{
  SetPrescale(msDelay1);
  range1 = CalcRange(msDelay1);
  range2 = CalcRange(msDelay2);
}

void SetupTimer (float msDelay)
{
  TCCR1A = 0;
  TCCR1B = 0;
  CalcTimers(msDelay, 0);
  TCNT1 = max(maxVal - range1, 0); //make sure it's greater than zero
  TIMSK1 |= (1<<TOIE1); //enable timer overflow
}

//ISR(TIMER1_OVF_vect)
//{
//  CheckLightLevel();
//}

void SetupTimers (float msDelay1, float msDelay2)
{
  TCCR1A = 0;
  TCCR1B = (1<<WGM12);
  CalcTimers(msDelay1, msDelay2);
  OCR1A = range1;
  OCR1B = range2;
  TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B); //enable timer overflow
}

//ISR(TIMER1_COMPA_vect)
//{
//  CheckLightLevel();
//}
//
//ISR(TIMER1_COMPB_vect)
//{
//  Alarm();
//}
