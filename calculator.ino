#include <Arduino.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup()
{
  lcd.init();
  lcd.backlight();

  for (int i = 2; i <= 12; i++) 
  {
    pinMode(i, INPUT_PULLUP);
  }

  for (int i = A0; i < A4; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

String currentNum = "";
float num1 = 0;
float num2 = 0;
float result = 0;
bool hasNum1 = false;
char currentOp;
char ops[] = {'+', '-', '*', '/', '='};
int inputDelay = 200;
void calculate();
bool calculationComplete = false;

void loop()
{
  for (int i = 2; i < 12; i++)
  {
    if (digitalRead(i) == LOW)
    {
      if (calculationComplete)
      {
        lcd.clear(); 
      }
      
      calculationComplete = false;
      currentNum += String(i - 2);
      lcd.print(i - 2);
      delay(inputDelay);
    }
  }

  for (int i = 12; i < A4; i++)
  {
    if (digitalRead(i) == LOW)
    {
      if (i != 13)
      {
        if (currentNum != "")
        {
          if (!hasNum1)
          {
            num1 = currentNum.toFloat();
            currentOp = ops[i - A0];
            lcd.print(currentOp);
            currentNum = "";
            hasNum1 = true;

            if (i == 12)
            {
              calculate();
              result = num1;
              lcd.clear();
              lcd.print(result);
              hasNum1 = false;
              calculationComplete = true;
            }
          }
          else
          {
            num2 = currentNum.toFloat();
            calculate();

            if (i != 12)
            {
              currentOp = ops[i - A0];
              lcd.print(currentOp);
            }
            else
            {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("The Calculator");
              lcd.setCursor(0, 1);
              lcd.print("will reset.");
              delay(2000);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("If you'll enter");
              lcd.setCursor(0, 1);
              lcd.print("any other no.");

              delay(2000);
              lcd.clear();
              lcd.noCursor();
              if (currentOp == '/')
              
                lcd.print(result);
              else
                lcd.print(result, 0);

              hasNum1 = false;
              calculationComplete = true;
            }
          }
        }
      }
    }
  }
}

void calculate()
{
  if (currentOp == '+')
    num1 += num2;

  else if (currentOp == '-')
    num1 -= num2;

  else if (currentOp == '*')
    num1 *= num2;

  else if (currentOp == '/')
  {
    if (num2 == 0)
      num1 = 0;
    else
      num1 /= num2;
  }

  result = num1;
  currentNum = "";
}