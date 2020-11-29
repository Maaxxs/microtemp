/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// Controlling LCD screen
#include <LiquidCrystal.h>

// Set temperature PIN, type DHT11
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT11


/* Map PINs from LCD to UNO R3 board */
// Register Select PIN. Controls where in the LCD's memory we're writing data to.
// is either data register (screen) or instructions for the LCD controller
const int rs = 12;

// Enable PIN. Enables writing to registers
const int en = 11;

// Data registers
const int d4 = 5, d5 = 4, d6 = 3, d7 = 2;

// Thermistor Analog Data PIN
const int thermistor = 0;
// My thermistor is a 10k thermistor, so I need to use a 10K resistor
const float R1 = 10000;
// Some coefficients from the Steinhart-Hart equation
const float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float logR2, R2, temperature;
float v_out;


// Create display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Create temperature sensor
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  dht.begin();
  Serial.begin(9600);
}



// count failed reads
int failed = 0;

void loop() {
  // https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
  v_out = analogRead(thermistor);
  Serial.println(v_out);
  
  float v_in = v_out * 5 / 1024;
  Serial.println(v_in);
  
  R2 = R1 * (1023.0 / (float)v_out - 1.0);
  
  logR2 = log(R2);
  temperature = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  temperature = temperature - 273.15;

  Serial.print("Thermistor temp: ");
  Serial.print(temperature);
  Serial.println("°C");


   
  float humi  = dht.readHumidity();
  float tempC = dht.readTemperature();

  if (isnan(humi) || isnan(tempC)) {
    failed++;
    if (failed > 5) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Failed to read");
      lcd.setCursor(0, 1);
      lcd.print("values ");
      lcd.print(failed);
      lcd.print(" times.");
    }
    delay(2000);
    return;
  }
  // reset failed counter as read was successful
  failed = 0;

//  Serial.println(tempC);
//  Serial.println(humi);

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
  lcd.print(humi);
  lcd.print("%");

  delay(2000);
  
}
