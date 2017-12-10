#include <SimpleDHT.h>

const int pinDHT11 = 2;
SimpleDHT11 dht11;

const int digitPins[4] = {
  4,5,6,7};          

const int latch=9;  //74HC595  pin 9 STCP
const int clock=10; //74HC595  pin 10 SHCP
const int data=8;   //74HC595  pin 8 DS

const byte digit[10] =      //seven segment digits in bits
{
  B00111111, //0
  B00000110, //1
  B01011011, //2
  B01001111, //3
  B01100110, //4
  B01101101, //5
  B01111101, //6
  B00000111, //7
  B01111111, //8
  B01101111  //9
};

int digitBuffer[4] = {
  0};

int digitScan = 0, flag=0, soft_scaler = 0;

void setup(){
    for(int i=0;i<4;i++)
  {
    pinMode(digitPins[i],OUTPUT);
  }
    pinMode(latch, OUTPUT);
    pinMode(clock, OUTPUT);
    pinMode(data, OUTPUT);
    Serial.begin(9600);

  }

  void updateDisp(){
    for(int j=0;j<4;j++)
      digitalWrite(digitPins[j], LOW);

     digitalWrite(latch, LOW);
     shiftOut(data, clock, MSBFIRST, B11111111);
     digitalWrite(latch, HIGH);

     delay(3);
     digitalWrite(digitPins[digitScan],HIGH);

     digitalWrite(latch, LOW);
     if(digitScan==2)
       shiftOut(data, clock, MSBFIRST, ~(digit[digitBuffer[digitScan]] | B10000000));
     else
       shiftOut(data, clock, MSBFIRST, ~digit[digitBuffer[digitScan]]);

       digitalWrite(latch, HIGH);
       digitScan++;
       if(digitScan>3) digitScan=0;
    }

  void loop(){   
    Serial.println("=================================");
    Serial.println("Sample DHT11...");

    byte temperature = 0;
    byte humidity = 0;
    byte data[40] = {0};
    if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
      Serial.print("Read DHT11 failed");
      return; 
     }

  Serial.print("Sample RAW Bits: ");
  for (int g = 0; g < 40; g++) {
    Serial.print((int)data[g]);
    if (g > 0 && ((g + 1) % 4) == 0) {
      Serial.print(' ');
    }
  }
 
  Serial.println("");
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" °C, ");
  Serial.print((int)humidity); Serial.println(" %");
  
  // DHT11 sampling rate is 1HZ.
  delay(2000);
  int tempC = int(temperature*100);
  
  digitBuffer[3] = int(tempC)/1000;
  digitBuffer[2] = (int(tempC)%1000)/100;
  digitBuffer[1] = (int(tempC)%100)/10;
  digitBuffer[0] = (int(tempC)%100)%10;
  updateDisp();
  delay(2);
   
    }
