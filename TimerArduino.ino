//




#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; //number of rows
const byte COLS = 4; //number of columns
char keys[COLS][ROWS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

/*
D2 - keypad8
D3 - keypad7
D4 - keypad6
.
D9 - keypad1
*/
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/*SCL - A5
  SDA - A4*/
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 4 line display

unsigned long lastMillis;

unsigned long work = 5;
unsigned long notWork = 5;  

////////////////////////////////////Setup and Loop////////////////////////////////////////////
void setup()
{ 
  pinMode(10, OUTPUT); // 5V to SSR
  digitalWrite(10, LOW);  
  lastMillis = millis();  
  
  //Serial.begin(9600);
  
  lcd.init();      // initialize the lcd   
  lcd.backlight(); // light the screen
  lcd.noAutoscroll();  
}

void loop()
{
  while((millis()-lastMillis) <= 5000)
  {  
    //Startup screen
    lcd.setCursor(0,0);
    lcd.print("Witam");
    lcd.setCursor(0,1);    
    lcd.print("Szacownego"); 
    lcd.setCursor(0,2);
    lcd.print("Topiarza");        
  }  
  
  Menu();
}
/////////////////////////////////////////////////////////////////////////////////////////////////

void Menu()
{
  lcd.clear();
  while(true)
  {
    //Menu screen
    lcd.setCursor(0,0);    
    lcd.print("Menu:");
    lcd.setCursor(0,1);    
    lcd.print("Auto (A)");
    lcd.setCursor(0,2);    
    lcd.print("Reka (B)");  
    
    char key = keypad.getKey();
  
    if (key != NO_KEY)
    {    
      if(key == 'A')
      {
        lcd.clear();
        Auto();        
      }
      if(key == 'B')
      {
        lcd.clear();
        Manual();
      }    
    }
  }  
}

void Auto()
{  
  CountDown();
}

void CountDown()
{ 
  unsigned long _work = work;
  unsigned long _notWork = notWork;
  
  while(true)
  { 
    /////////////Work Loop////////////// 
    while(true)
    {
      digitalWrite(10, HIGH);
      ScreenAndLoop(_work, _notWork); 
                  
      _work --;
      
      if(_work <= 0)
      {
        _work = work;
        break; 
      }      
    }
    /////////////Break Loop/////////////
    while(true)
    {
      digitalWrite(10, LOW);
      ScreenAndLoop(_work, _notWork); 
             
      _notWork --;
      
      if(_notWork <= 0)
      {
        _notWork = notWork;
        break;     
      }      
    }    
  }
}

void ScreenAndLoop(unsigned long val, unsigned long val2)
{  
  Screen(val, val2);
  for(int i = 0; i <= 4; i++)
  {
    MenuAuto();        
    delay(200);
  }
}

void Manual()
{ 
  int num = 0; 
  while(true)
  {    
    if(num == 0)
    {
      //Manual controll screen
      HCS();
    }
    if(num == 1)
    {
      HCSWork();
    }
    if(num == 2)
    {
      HCSBreak();
    }

    char key = keypad.getKey();
  
    if (key != NO_KEY)
    {   
      lcd.clear(); 
      if( key == 'A')
      {
        digitalWrite(10, HIGH); 
        num = 1;       
      }
      if(key == 'B')
      {
        digitalWrite(10, LOW);
        num = 2;
      }
      if(key == 'D')
      {
        digitalWrite(10, LOW);
        Menu();
      }       
    }
  }
}

void HCS()
{
  lcd.setCursor(0,0);    
  lcd.print("Tryb reczny"); 
  lcd.setCursor(0,1);    
  lcd.print("(A) - ZASYP START");
  lcd.setCursor(0,2);    
  lcd.print("(B) - ZASYP STOP");
  lcd.setCursor(0,3);    
  lcd.print("(D) - MENU");
}

void HCSWork()
{
  lcd.setCursor(0,0);    
  lcd.print("Tryb reczny"); 
  lcd.setCursor(0,1);    
  lcd.print("(A) - ZASYP START *");
  lcd.setCursor(0,2);    
  lcd.print("(B) - ZASYP STOP");
  lcd.setCursor(0,3);    
  lcd.print("(D) - MENU");
}

void HCSBreak()
{
  lcd.setCursor(0,0);    
  lcd.print("Tryb reczny"); 
  lcd.setCursor(0,1);    
  lcd.print("(A) - ZASYP START");
  lcd.setCursor(0,2);    
  lcd.print("(B) - ZASYP STOP  *");
  lcd.setCursor(0,3);    
  lcd.print("(D) - MENU");
}

void SetUp()
{  
  lcd.clear();
  digitalWrite(10, LOW); 
  while(true)
  {
    SetUpScreen();
    
    char key = keypad.getKey();
  
    if (key != NO_KEY)
    {    
      if( key == 'A')
      {
        lcd.clear();     
        work = GetVal("pracy");    
      }
      if(key == 'B')
      {
        lcd.clear();     
        notWork = GetVal("przerwy");
      }
      if(key == 'D')
      {
        lcd.clear();  
        //Auto();        
        //Menu();
        CountDown();
      }
    }    
  }
}

void SetUpScreen()
{
  lcd.setCursor(0,0);    
  lcd.print("Nastawy: ");
  lcd.setCursor(0,1);
  lcd.print("(A) - Praca");
  lcd.setCursor(0,2);
  lcd.print("(B) - Przerwa");
  lcd.setCursor(0,3);    
  lcd.print("(D) - Auto");
}

int GetVal(String str)
{
  String msg = "Czas " + str + ": 1-9999s";
  String val = "";
  int num = 0;
    
  while(true)
  {
    lcd.setCursor(0,0);    
    lcd.print(msg);
    lcd.setCursor(0,1);  
    lcd.print(val); 
    lcd.setCursor(0,2);
    lcd.print("* skasuj, # enter");  

    char key = keypad.getKey();   
  
    if (key != NO_KEY)
    {  
      if(key == 'D')
      {        
        SetUp();
      }
      if(key ==  '*')
      {
        val = "";
      }
      if(key == '#')
      {
        num = val.toInt();
        break;
      }
      val += key;
    }    
  }
  lcd.clear();
  return num;
}

void MenuAuto()
{
  char key = keypad.getKey();
  
  if (key != NO_KEY)
  {    
    if( key == 'C')
    {
      digitalWrite(10, LOW);
      SetUp();        
    }
    if(key == 'D')
    {
      digitalWrite(10, LOW);
      Menu();
    } 
    //////////////
    if( key == '1')
    {
      work ++;       
    }
    if(key == '7')
    {
      work --;      
    }
    if(key == '3')
    {
      notWork ++;      
    }   
    if(key == '9')
    {
      notWork --;      
    }  
    ////////////    
  }  
}

void Screen(unsigned long s, unsigned long p)
{    
  lcd.setCursor(0,0);    
  lcd.print("Zasyp: ");
  lcd.setCursor(10,0);    
  lcd.print(ConvertToString(s)); 
      
  lcd.setCursor(0,1);    
  lcd.print("Przerwa: ");
  lcd.setCursor(10,1);    
  lcd.print(ConvertToString(p));

  lcd.setCursor(0,2);    
  lcd.print("CzasZ: ");
  lcd.setCursor(7,2);    
  lcd.print(work);

  lcd.setCursor(10,2);    
  lcd.print("CzasP: ");
  lcd.setCursor(17,2);    
  lcd.print(notWork);

  lcd.setCursor(0,3);    
  lcd.print("(D) MENU"); 
  lcd.setCursor(9,3);  
  lcd.print("(C) Nastawy"); 
}

String ConvertToString(unsigned long v)
{
  String str = String(v, DEC);  
  
  if(v < 10)
  {
    str = "0" + str;
  }     
  return str;
}
