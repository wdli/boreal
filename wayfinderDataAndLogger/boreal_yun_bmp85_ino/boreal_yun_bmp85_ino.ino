#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Process.h>
#include <FileIO.h>
   
Adafruit_BMP085 bmp = Adafruit_BMP085(10085);
int led = 13;

void setup(void) 
{
  

  String t;
  
  Bridge.begin(); //Need this for time stamp to work
  FileSystem.begin();
  pinMode(led, OUTPUT);
  
  Serial.println(" Welcome to Boreal Space Arduino Flight Test ");
  
  //t = getTimeStamp();
  
  Serial.begin(9600);
  Serial.println("Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  Blink(500, 3);
  
  //Serial.print("Time: ");
  //Serial.print(t);
  
  
}




void loop(void) 
{
  
  String t;
  
  t = getTimeStamp();
   
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
 
  Serial.println(t);
  File dataFile = FileSystem.open("/mnt/sda1/arduino/sensor.txt", FILE_APPEND);
  if (dataFile) {
   
    Blink(100, 5);

    dataFile.println(t);
    
    /* Display the results (barometric pressure is measure in hPa) */
    if (event.pressure)  {
      /* Display atmospheric pressue in hPa */
      Serial.print("Pressure:    ");
      Serial.print(event.pressure);
      Serial.println(" hPa");
      
      dataFile.print("Pressure:    ");
      dataFile.print(event.pressure);
      dataFile.println(" hPa");
      
      /* Calculating altitude with reasonable accuracy requires pressure    *
       * sea level pressure for your position at the moment the data is     *
       * converted, as well as the ambient temperature in degress           *
       * celcius.  If you don't have these values, a 'generic' value of     *
       * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
       * in sensors.h), but this isn't ideal and will give variable         *
       * results from one day to the next.                                  *
       *                                                                    *
       * You can usually find the current SLP value by looking at weather   *
       * websites or from environmental information centers near any major  *
       * airport.                                                           *
       *                                                                    *
       * For example, for Paris, France you can check the current mean      *
       * pressure and sea level at: http://bit.ly/16Au8ol                   */
       
      /* First we get the current temperature from the BMP085 */
      float temperature;
      bmp.getTemperature(&temperature);
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");
      
      
      dataFile.print("Temperature: ");
      dataFile.print(temperature);
      dataFile.println(" C");
  
      /* Then convert the atmospheric pressure, SLP and temp to altitude    */
      /* Update this next line with the current SLP for better results      */
      //float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      
      
      float seaLevelPressure = 1020; // adjust this according to local sea level pressure
      float alt = bmp.pressureToAltitude(seaLevelPressure,
                                          event.pressure,
                                          temperature);
      Serial.print("Altitude:    "); 
      Serial.print(alt); 
      Serial.println(" m");
      Serial.println("");
      
      dataFile.print("Altitude:    "); 
      dataFile.print(alt); 
      dataFile.println(" m");
      
      dataFile.println("");
      
    }
    else {
      Serial.println("Sensor error");
    }
  }
  else {
    Serial.println(" Failed to open /mnt/sda1/arduino/sensor.txt");
    Blink(1000,5);
    
  }
  
  delay(10000);
}



// This function return a string with the time stamp
String getTimeStamp() {
   String result;
   Process time;
   // date is a command line utility to get the date and the time  
  // in different formats depending on the additional parameter 
   time.begin("date");
   time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
                                 //             T for the time hh:mm:ss    
   time.run();  // run the command
 
  // read the output of the command
  while(time.available()>0) {
     char c = time.read();
     if(c != '\n')
       result += c;
  }
 
  return result;
}


// Blink LED

void Blink(int d, int num)
{
  while (num-- > 0) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    //Serial.println(" Blink ");
    delay(d);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    //Serial.println(" Sleep ");
    delay(d);               // wait for a second
  }
} 
  

