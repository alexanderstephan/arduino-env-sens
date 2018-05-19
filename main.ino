
#include "DS3231.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "SimpleDHT.h" // Temporarely using header files

// WiFi Credentials
const char* ssid = "FRITZ!Box 6490 Cable";
const char* password = "";

// Initialize DHT11 sensor
SimpleDHT11 dht11;

// Initialize  RTC-Module
//DS3231 clock;
//RTCDateTime dt;

int pinDHT11 = 2;
int ledPin = 13; // GPIO13
byte temperature = 0;
byte humidity = 0;


WiFiServer server(80);

void setup() {
  Serial.begin(115200);
//  clock.begin();
//  clock.setDateTime(__DATE__, __TIME__);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

// Set ledPin according to the request
// digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");

  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("<br><br>");

// Check DHT11  values
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    return;
  }

// Print temperature and humidity
  client.println("Current room temperature:");
  client.println((int)temperature); client.print(" Degrees Celsius");
  client.println("<br><br>");
  
  delay(50);
  
  client.println("Humidity:");
  client.println((int)humidity); client.println(" %");
  client.println("<br><br>");
   
  delay(50);
  // Print time with RTC-Module
//  dt = clock.getDateTime();
//
//  client.print("Long number format:          ");
//  client.println(clock.dateFormat("d-m-Y H:i:s", dt));
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
