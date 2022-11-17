/* Interfacing the bluepill_f103c8 with BMP280 sensor and Nokia 5110 (PCD8544) display.*/

#include <Wire.h>              // for I2C communication
#include <SPI.h>               // include SPI library
#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 library
#include <Adafruit_BMP280.h>   // include adafruit BMP280 sensor library.

// The PCD8544 SPI connections             (CLK, DIN, D/C, CS, RST).
Adafruit_PCD8544 display = Adafruit_PCD8544(PA5, PA7, PA6, PA4, PB0);

// define the BMP280 I2C address: 0x76 or 0x77 (dafault)
#define BMP280_I2C_ADDRESS  0x76

Adafruit_BMP280 bmp280;

void setup()
{
  //I2C configuration
  Wire.setSCL(PB8);
  Wire.setSDA(PB9); 
  Wire.begin();

  // initialize the display.
  display.clearDisplay();   // clear the screen.
  display.begin(); // Initialize the display.

  // set the display conntrast
  display.setContrast(50);

  display.setTextSize(1); // text size (6x8);
  display.setTextColor(BLACK, WHITE); // Text color
  display.setCursor(16, 0); // text position
  display.print("BMP280"); // send massage to display
  display.display(); // update the display
  
  // Check for connection
  if ( !bmp280.begin(BMP280_I2C_ADDRESS) )
  {  
    display.setCursor(2, 15);
    display.println("Connection");
    display.print("failed!");
    display.display();
    while (1);
  }
  /*BMP280 sampling settings*/
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,   // Operating mode.
                     Adafruit_BMP280::SAMPLING_X2,   // Temperature oversampling
                     Adafruit_BMP280::SAMPLING_X16,  // Pressure oversampling
                     Adafruit_BMP280::FILTER_X16,    // Filtering
                     Adafruit_BMP280::STANDBY_MS_500 // Standby timme
                     ); 
                     
  display.setCursor(3, 12);
  display.print("T°:");
  display.setCursor(3, 22);
  display.print("Pr:");
  display.setCursor(3,32);
  display.print("Alt:");

}

char text[12]; // used to store readings

void loop()
{
  // get Temperature and pressure readings from sensor.
  float temp     = bmp280.readTemperature();     // get temperature in °C
  float pressure = bmp280.readPressure() / 100;  // get Pressure in hPa
  float altitude = bmp280.readAltitude();       //  get Altitude in m
  //altitude = 44330 * (1.0 - pow(pressure / seaLevelhPa, 0.1903)).

  // Display data on the screen.
  // 1: Temperature
  display.setCursor(15, 12);
  if(temp >= 0)
    display.print(' ');
  else
    display.print('-');
  sprintf(text, "%02u.%02u C", (int)abs(temp), (int)(abs(temp) * 100) % 100);
  display.print(text);
  display.drawRect(53, 12, 3, 3, BLACK); // print degree symbol ( ° )

  // 2: Pressure
  display.setCursor(10, 22);
  sprintf(text, "%04u.%02u hPa", (int)pressure, (int)((uint32_t)(pressure * 100) % 100));
  display.print(text);

  // 3: Altitude
  display.setCursor(12,32);
  sprintf(text,"%04u.%02u m",(int)altitude, (int)((uint32_t)(altitude*100)% 100));
  display.print(text);

  // Update the display
  display.display();

  delay(2000); 
}