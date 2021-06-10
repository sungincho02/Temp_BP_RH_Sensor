// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>
#include "cactus_io_BME280_I2C.h"

// Create BME280 object
BME280_I2C bme; // I2C using address 0x77
// or BME280_I2C bme(0x76); // I2C using address 0x76

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 2, 3, 4, 5);

const int TEMP = 0;
const int BP = 1;
const int RH = 2;
int type = TEMP;
float tempC, tempF, bp, rh;
bool celcius = false;
float interval;

void setup() {
  // set up the LCD
  lcd.begin(16, 2);
  
  // set up the buttons
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  
  // set up the bme280 sensor
  if (!bme.begin()) {
    lcd.print("ERROR");
    while (1);
  }
  
  // calibration
  bme.setTempCal(-1);
  
  interval = millis();
}

void loop() {
  // check button click
  if (digitalRead(8) == LOW) {          // switch between celcius and farenheit
    celcius = !celcius;
    delay(300);
  }
  else if (digitalRead(9) == LOW) {
    if (type == TEMP || type == BP) {   // change between temperature, pressure, and humidity
      type += 1;
    }
    else {
      type = TEMP;
    }
    updateDisplay();
    interval = millis();
    delay(300);
  }
  
  // update every 0.5 seconds
  if (millis() > interval + 500) {
    // read data
    bme.readSensor();
    tempC = bme.getTemperature_C();
    tempF = bme.getTemperature_F();
    bp = bme.getPressure_MB();
    rh = bme.getHumidity();
  
    // check data validity
    if (isnan(tempC) || isnan(tempF) || isnan(bp) || isnan(rh)) {
      lcd.print("ERROR");
      while(1);
    }
    
    // update
    updateDisplay();
    interval = millis();
  }
}

// display temperature, pressure, or humidity on LCD
void updateDisplay() {
  lcd.clear();
  
    if (type == TEMP) {       // display temperature
      lcd.setCursor(1,0);
      if (celcius) {
        lcd.print("Temperature( C)");
        lcd.setCursor(13,0);
        lcd.print((char)223);
        lcd.setCursor(6,1);
        lcd.print(tempC);
      }
      else {
        lcd.print("Temperature( F)");
        lcd.setCursor(13,0);
        lcd.print((char)223);
        lcd.setCursor(6,1);
        lcd.print(tempF);
      }
    }
    else if (type == BP) {    // display pressure
      lcd.setCursor(2,0);
      lcd.print("Pressure(mb)");
      lcd.setCursor(5,1);
      lcd.print(bp);
    }
    else if (type == RH) {    // display humidity
      lcd.setCursor(3,0);
      lcd.print("Humidity(%)");
      lcd.setCursor(6,1);
      lcd.print(rh);
      
    }
}