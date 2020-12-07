/* Bits and pieces from https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
 * 
 * 
 */

// LCD screen
#include <LiquidCrystal.h>

// I use a DHT11. Can read temperature and humidity
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
const float KELVIN_MIN = 273.15;

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

// needed for calculation
float logR2, R2, temperature;
// Read from analog PIN of thermistor
float v_out;

void loop() {
  // Reading and calculating the temperature from thermistor
  // analogRead() returns a value from 0 to including 1023 which represents
  // the voltage. Therefore, 1023 is the maximum = input voltage
  float v_out= analogRead(thermistor);
  if (isnan(v_out)) {
    temperature = -999;
  } else {
    R2 = R1 * (1023 / v_out - 1.0);

    // See the Steinhart-Hart equation for more info
    logR2 = log(R2);
    temperature = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    temperature = temperature - KELVIN_MIN;
  }

  //  Serial.print("Thermistor temp: ");
  //  Serial.print(temperature);
  //  Serial.println("°C");


  // Reading the termpature from DHT11
  float humidity  = dht.readHumidity();
  float temp_c = dht.readTemperature();

  if (isnan(humidity) || isnan(temp_c)) {
    failed++;
    humidity = -999;
    temp_c = -999;
    if (failed > 5) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Failed to read");
      lcd.setCursor(0, 1);
      lcd.print("values ");
      lcd.print(failed);
      lcd.print(" times.");
    } 
  }
  // reset failed counter as read was successful
  failed = 0;

//  Serial.println(temp_c);
//  Serial.println(humidity);

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temp_c);
//  lcd.print((char)223);
  lcd.print("|");

  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  upload_data(temperature, temp_c, humidity);

  // wait 1 minute
  delay(60000);
  
}

void upload_data(float t_thermistor, float t_dht, float h_dht) {
  Serial.print(t_thermistor);
  Serial.print(",");
  Serial.print(t_dht);
  Serial.print(",");
  Serial.println(h_dht);
}
