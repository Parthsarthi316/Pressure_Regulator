//ALERT: SHUT THE INPUT VALVE BEFORE STARTING THIS CODE AS IT'LL CALCULATE THE INITIAL ERRORS (I GUESS YOU DON'T WANT THEM TO BE INACCURATE)
// ENJOY 8)

//IN THE SERIAL MONITOR, INPUT IN THE FORM, PRESSURE1_PRESSURE2. WHERE PRESSURE 1 AND 2 ARE THE REQUIRED PRESSURES BY THE USER.

float InputPressure = 500; //[kPa]

#include <Wire.h>
#include <SPI.h>

#define PRINT_CALCULATED
#define PRINT_SPEED 100
byte address=0x11;
#define PressureVcc1 5.000
const int CS1 = 9, CS2 = 10;
int PressurePin1 = A0, PressurePin2 = A1;
String Input;
float err1,err2;
int PSET1,PSET2;

void setup() {
    Serial.begin(9600); 
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    err1=analogRead(PressurePin1);
    err1=err1*5/1024;
    err1=err1/0.009;
    err2=analogRead(PressurePin2);
    err2=err2*5/1024;
    err2=err2/0.009;
    SPI.begin();
    digitalPotWrite1(0x00);
    delay(1000);
    digitalPotWrite1(0x80);
    delay(1000);
    digitalPotWrite1(0xFF);
    delay(1000);
    digitalPotWrite2(0x00);
    delay(1000);
    digitalPotWrite2(0x80);
    delay(1000);
    digitalPotWrite2(0xFF);
    delay(1000);
    Serial.println("START");
}
double MPX550DP (float err,int x)
{
  double SensVolt=x*5.0/1023.0;
  double ans=(SensVolt)/0.009;
  ans=ans-err;//kPa
  return ans;
}

//Setting value to a specific address on Chip 1
int digitalPotWrite1(int value)
{
  digitalWrite(CS1, LOW);
  digitalWrite(CS2,HIGH);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS1, HIGH);
}

//Setting value to a specific address on Chip 2
int digitalPotWrite2(int value)
{
  digitalWrite(CS2, LOW);
  digitalWrite(CS1,HIGH);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS2, HIGH);
}

//Separating x and y from input "x_y"
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() 
{
  while (Serial.available() > 0) 
  {
    Input = Serial.readString();//Input desired pressure. Input 0 to shut off. 
    String P1=getValue(Input,'_',0);
    String P2=getValue(Input,'_',1);
    PSET1 = P1.toInt();
    PSET2 = P2.toInt();
    delay(300);
    if (PSET1<0 || PSET2<0)
      Serial.println("OFF");
    else if (PSET1 > InputPressure || PSET2 > InputPressure) // Error           
      Serial.println("Error: Target pressure too high");
    else
    {
      Serial.println("Hello     ");
      Serial.print("PSET1: ");
      Serial.print(PSET1);
      delay(500);
      Serial.print("---------PSET2: ");
      Serial.println(PSET2);
      delay(500);
      int mappin1=map(PSET1,0,500,0,255);
      int mappin2=map(PSET2,0,500,0,255);
      delay(500);
      digitalPotWrite1(mappin1);
      delay(500);
      digitalPotWrite2(mappin2);
      delay(500);
      int x1=analogRead(PressurePin1);
      int x2=analogRead(PressurePin2);            
      delay(1);
      double p1=MPX550DP(err1,x1);
      Serial.print("Current Reading P1:");
      Serial.println(p1);
      
      double p2=MPX550DP(err2,x2);
      Serial.print("Current Reading P2:");
      Serial.println(p2);
      
      //Division by a suitable number (12) such that it gradually approaches to the pressure value defined by the user
      int i=mappin1/12;
      int j=mappin2/12;
      while(p1<PSET1 || p2<PSET2)
      {
        if(p1<PSET1){
        int temp=analogRead(PressurePin1);
        p1=MPX550DP(err1,temp);
        digitalPotWrite1(mappin1+i);
        delay(500);
        i+=1;
        }
        if(p2<PSET2){
        int temp=analogRead(PressurePin2);
        p2=MPX550DP(err2,temp);
        digitalPotWrite2(mappin2+j);
        delay(500);
        j+=1;
        }
      }
      Serial.println(p1);
      Serial.println(p2);
    }
  }
}
