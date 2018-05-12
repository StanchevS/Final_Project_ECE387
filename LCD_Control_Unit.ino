#include <EmonLib.h>
EnergyMonitor emon1;

#include "DHT.h"

#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
/*
  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K potentiometer:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
   10K poterntiometer on pin A0

  created 21 Mar 2011
  by Tom Igoe
  modified 11 Nov 2013
  by Scott Fitzgerald
  modified 7 Nov 2016
  by Arturo Guadalupi

*/

// include the library code:
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Variables that keep track of the received (stateLights) and send data (instructions).




//Buttons attached to the following pins.
int ctImput = 7;
DHT dht(DHTPIN, DHTTYPE, 6);

long takeMesurment = millis();
int interval = 3000;

float temp;
double Irms;



void setup()
{
  // initialize both serial ports:
  Serial.begin(9600);
  emon1.current(1, 111.1);
  dht.begin();
  //Initialize the buttonPins as outputs.Size of the array is 6.

  pinMode(ctImput, INPUT);


  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);





  // set the cursor to the top left
  lcd.setCursor(0, 0);

  // Print a message to the lcd.
  lcd.print("Light:");
  // lcd.setCursor(9, 0);
  // lcd.print(" FAN:");
  lcd.setCursor(0, 1);
  lcd.print("TEMP:");

}//End of Setup.

// Process the Temperature.
void processTemp(int Tvar)
{
  lcd.setCursor(7, 1);
  lcd.write(byte(Tvar));
  lcd.setCursor(9, 1);
  lcd.print("C");
}



void loop() {



  //take temp measurements
  if ((millis() - takeMesurment ) >= interval)
  {

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

    // Read temperature as Celsius
    temp = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if ( isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    processTemp(temp);
    //*******************////////////////////////
     lcd.setCursor(6, 1);
    lcd.print(temp);
    lcd.setCursor(9, 0);
    if ((Irms*230.0) > 400.1 && (Irms*230.0) < 2100.1) {
      lcd.print("ON ");
    } else {
      lcd.print("OFF");
    }
    //Take curren/Voltage measurement
    double reading = analogRead(A1);
    Irms = emon1.calcIrms(reading);
    Serial.print("Pin reading: ");
    Serial.println(reading);
    Serial.println(" ");
    Serial.print("Library values: ");
    Serial.println(Irms * 230.0);         // Apparent power

    //Serial.println(Irms);

    takeMesurment = millis();
  }

  

   

    /*
      ///This is from the original code.
        int sensorReading = analogRead(A0);
        // map the result to 200 - 1000:
        int delayTime = map(sensorReading, 0, 1023, 200, 1000);
        // set the cursor to the bottom row, 5th position:
        lcd.setCursor(4, 1);
        // draw the little man, arms down:
        lcd.write(3);
        delay(delayTime);
        lcd.setCursor(4, 1);
        // draw him arms up:
        lcd.write(4);
        delay(delayTime);
    */

  
}





