

//www.elegoo.com
//2016.12.9

#include <SimpleDHT.h>
#include <Wire.h>


DS3231 clock;
RTCDateTime dt;


// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
int lightPin = A0;

SimpleDHT11 dht11;

void setup() {
  
  Serial.begin(9600); 
  Serial.println("=================================");
  Serial.println("Initialize DS3231");;
  
  clock.begin();

  clock.setDateTime(__DATE__, __TIME__);

}

void loop() {
  // start working...

  dt = clock.getDateTime();


  Serial.print("Long number format:          ");
  Serial.println(clock.dateFormat("d-m-Y H:i:s", dt));

  Serial.print("Long format with month name: ");
  Serial.println(clock.dateFormat("d F Y H:i:s",  dt));

  Serial.print("Short format witch 12h mode: ");
  Serial.println(clock.dateFormat("jS M y, h:ia", dt));

  Serial.print("Today is:                    ");
  Serial.print(clock.dateFormat("l, z", dt));
  Serial.println(" days of the year.");

  Serial.print("Actual month has:            ");
  Serial.print(clock.dateFormat("t", dt));
  Serial.println(" days.");

  Serial.print("Unixtime:                    ");
  Serial.println(clock.dateFormat("U", dt));

  Serial.println();

  Serial.println("Sample DHT11...");

  // read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};

  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    return;
  }

  Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
    Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      Serial.print(' ');
    }
  }

  Serial.println("");

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" %");
  int reading = analogRead(lightPin);
  Serial.print((int)reading); Serial.print(" Lumen");

  // DHT11 sampling rate is 1HZ.
  delay(1000);
}

