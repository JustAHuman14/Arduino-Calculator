#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <BigNumber.h>

const byte rows = 4;
const byte cols = 4;
byte rowPins[rows] = {2, 3, 4, 5};
byte colPins[cols] = {6, 7, 8, 9};

char keys[rows][cols] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'C', '0', '=', '/'},
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  lcd.init();
  lcd.backlight();
  BigNumber::begin(15);
  Serial.begin(9600);
}

String currentNum;
BigNumber num1;
String totalDigitsOnScreen;
BigNumber num2;
BigNumber result;
bool hasNum1 = false;
char currentOp;
bool calculationComplete = false;

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

void loop()
{
  char key = keypad.getKey();

  if (key)
  {
    if (key == 'C')
    {
      lcd.clear();
      currentNum = "";
      totalDigitsOnScreen = "";
      num1 = 0;
      num2 = 0;
      result = 0;
      hasNum1 = false;
      calculationComplete = false;
    }

    if (key >= '0' && key <= '9')
    {
      if (calculationComplete)
      {
        lcd.clear();
        currentNum = "";
        totalDigitsOnScreen = "";
        num1 = 0;
        num2 = 0;
        result = 0;
      }

      calculationComplete = false;

      if (totalDigitsOnScreen.length() != 16)
      {
        totalDigitsOnScreen += String(key);
        currentNum += String(key);
        lcd.print(key);
      }
    }
    else
    {
      if (key == '+' || key == '-' || key == '*' || key == '/' || key == '=')
      {
        if (currentNum != "")
        {
          if (!hasNum1)
          {
            num1 = BigNumber(currentNum.c_str());
            currentOp = key;
            totalDigitsOnScreen += String(key);
            lcd.print(currentOp);
            currentNum = "";
            hasNum1 = true;

            if (key == '=')
            {
              calculate();
              result = num1;
              char *rawNum1 = result.toString();
              String Num1 = String(rawNum1);
              free(rawNum1);
              lcd.clear();

              if (currentOp == '/')
              {
                if (Num1.indexOf('.') != -1)
                {
                  while (Num1.endsWith("0"))
                    Num1.remove(Num1.length() - 1);

                  if (Num1.endsWith("."))
                    Num1.remove(Num1.length() - 1);

                  if (Num1.length() >= 16)
                    lcd.print(Num1.substring(0, 16));
                  else
                    lcd.print(Num1);
                }
              }
              else
                lcd.print(Num1.substring(0, Num1.indexOf(".")));

              hasNum1 = false;
              calculationComplete = true;
            }
          }
          else
          {
            num2 = BigNumber(currentNum.c_str());
            calculate();

            if (key != '=')
            {
              currentOp = key;
              totalDigitsOnScreen += String(key);
              lcd.print(currentOp);
            }
            else
            {
              totalDigitsOnScreen = "";
              lcd.clear();
              lcd.noCursor();
              char *rawNum1 = num1.toString();
              String Num1 = String(rawNum1);
              free(rawNum1);

              if (currentOp == '/')
              {
                if (Num1.indexOf('.') != -1)
                {
                  while (Num1.endsWith("0"))
                    Num1.remove(Num1.length() - 1);

                  if (Num1.endsWith("."))
                    Num1.remove(Num1.length() - 1);
                  if (Num1.length() >= 16)
                    lcd.print(Num1.substring(0, 16));
                  else
                    lcd.print(Num1);
                }
              }
              else
                lcd.print(Num1.substring(0, Num1.indexOf(".")));

              hasNum1 = false;
              calculationComplete = true;
            }
          }
        }
      }
    }
  }
}
