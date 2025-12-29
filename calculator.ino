#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

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
}

String currentNum = "";
String totalNum = "";
float num1 = 0;
float num2 = 0;
float result = 0;
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
  if(totalNum.length() == 16) lcd.setCursor(0,1);
  char key = keypad.getKey();

  if (key)
  {
    if (key == 'C')
    {
      totalNum = "";
      lcd.clear();
      currentNum = "";
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
        totalNum = "";
        lcd.clear();
        currentNum = "";
        num1 = 0;
        num2 = 0;
        result = 0;
      }

      calculationComplete = false;
      currentNum += String(key);
      totalNum += String(key);
      lcd.print(key);
    }
    else
    {
      if (key == '+' || key == '-' || key == '*' || key == '/' || key == '=')
      {
        if (currentNum != "")
        {
          if (!hasNum1)
          {
            num1 = currentNum.toFloat();
            currentOp = key;
            totalNum += String(key);
            lcd.print(currentOp);
            currentNum = "";
            hasNum1 = true;

            if (key == '=')
            {
              calculate();
              result = num1;
              totalNum = "";
              lcd.clear();
              
              if (currentOp == '/')
              {
                lcd.print("= ");
                lcd.print(result);
              }
              else
              {
                lcd.print("= ");
                lcd.print(result, 0);
              }
              
              hasNum1 = false;
              calculationComplete = true;
            }
          }
          else
          {
            num2 = currentNum.toFloat();
            calculate();

            if (key != '=')
            {
              currentOp = key;
              totalNum+=String(key);
              lcd.print(currentOp);
            }
            else
            {
              totalNum = "";
              lcd.clear();
              
              if (currentOp == '/')
              {
                lcd.print("= ");
                lcd.print(result);
              }
              else
              {
                lcd.print("= ");
                lcd.print(result, 0);
              }
              
              hasNum1 = false;
              calculationComplete = true;
            }
          }
        }
      }
    }
  }
}
