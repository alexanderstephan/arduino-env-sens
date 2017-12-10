
//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
#include <SimpleDHT.h>

int pinDHT11 = 2;
SimpleDHT11 dht11;



//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void setup()
{
  lcd.begin(16, 2);
}
void loop()
{
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11");
    return;
  }
  
  // Display Temperature in C
  lcd.setCursor(0, 0);
  lcd.print("Temperature");

  lcd.setCursor(12, 0);
  lcd.print(temperature);

  lcd.setCursor(14,0);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("Humidity");
  
  lcd.setCursor(12, 1);
  lcd.print(humidity);

  
  lcd.setCursor(14,1);
  lcd.print(" %");
  
  delay(500);
}
