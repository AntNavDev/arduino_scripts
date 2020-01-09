int dataPin = 8;
int latchPin = 2;
int clockPin = 3;
int blinkTime = 100;
const int registerSize = 16;
unsigned int mostSignificantBit = pow(2, (registerSize - 1));

/**
 * Move to next output pin
 * 
 */
void pulseClockPin()
{
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
  return;
}

/**
 * Load data to output pins
 * 
 */
void loadRegister()
{
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  return;
}

/**
 * Push data to output pins using a whole decimal number
 * and displays data in pins as binary representation
 * 
 * @param value What output pins to push data to
 */
void ssrWrite(int value)
{
  for(int index = 0; index < registerSize; index++)
  {
    unsigned int temp = value & mostSignificantBit;
    if(temp == mostSignificantBit)
    {
      digitalWrite(dataPin, HIGH);
    }
    else
    {
      digitalWrite(dataPin, LOW);
    }
    pulseClockPin();
    value = value << 1;
  }
  loadRegister();
  return;
}

/**
 * Shift data from one register output pin to the next and back
 * 
 * @param shiftCount The amount of pins to move when shifting output
 * @param alternateBlinkTime Change default blinkTime
 */
void backAndForth(int shiftCount = 1, int alternateBlinkTime = 0)
{
  unsigned int temp = 1;
  int newBlinkTime = alternateBlinkTime ? alternateBlinkTime : blinkTime;
  int loopMax = shiftCount > 0 ? (registerSize / shiftCount) : registerSize;
  // If the quotient is not a whole number, we add an extra loop(basically rounding up)
  if((registerSize * shiftCount) != loopMax && loopMax < registerSize)
  {
    loopMax++;
  }
  ssrWrite(0);
  delay(newBlinkTime);
  Serial.println("Shift Forward");
  for(int index = 0; index < loopMax; index++)
  {
    ssrWrite(temp);
    temp = temp << shiftCount;
    delay(newBlinkTime);
  }

  ssrWrite(0);
  delay(newBlinkTime);
  Serial.println("Shift Back");
  temp = mostSignificantBit;
  for(int index = 0; index < loopMax; index++)
  {
    ssrWrite(temp);
    temp = temp >> shiftCount;
    delay(newBlinkTime);
  }
  return;
}

/**
 * Turn 'on' all output pins and then turn them 'off'
 * 
 * @param alternateBlinkTime Change default blinkTime
 */
void blinkAll(int alternateBlinkTime = 0)
{
  // pow() was causing rounding errors
  unsigned int maxValue = (1UL << registerSize) - 1;
  int newBlinkTime = alternateBlinkTime ? alternateBlinkTime : blinkTime;
  Serial.println(maxValue);
  ssrWrite(maxValue);
  delay(newBlinkTime);
  ssrWrite(0);
  delay(newBlinkTime);
  return;
}

void setup() {
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
}

void loop() {
  backAndForth(2, 250);
  blinkAll(750);
  blinkAll(750);
  backAndForth();
  blinkAll(750);
  blinkAll(750);
}
