//Include the FileIO header, for communicating with the SD card.
#include <FileIO.h> 
#include <Adafruit_BMP085.h>
#include <Wire.h>

Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();
 
  while(!Serial);
   Serial.println("Boreal Space\n");  
   Serial.println("Wayfinder-1 datalogger\n");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (!bmp.begin()) { 
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");         
  }
  // create a string that starts with a timestamp to organize the data to be logged.
  String dataString;
  dataString += getTimeStamp();
  //dataString += " = ";
  Serial.println(dataString);
  // Read the data from the sensors and append them to the string, 
  // separating the values with a comma :
  //for (int analogPin = 0; analogPin < 3; analogPin++) {
  //   int sensor = analogRead(analogPin);
   //  dataString += String(sensor);
    // if (analogPin < 2) {
     //  dataString += ",";  
    // }
  // }
  

   // open the file. note that only one file can be open at a time,
   // so you have to close this one before opening another.
   // The FileSystem card is mounted at the following "/mnt/FileSystema1"
   File dataFile = FileSystem.open("/mnt/sda1/arduino/sensor.txt", FILE_APPEND);
 
  // if the file is available, write to it:
   if (dataFile) {
     //dataFile.println(dataString);
    dataFile.println(dataString);
    dataFile.println("BMP085 Sensor\n");
     
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    dataFile.print("Temperature = ");
    dataFile.print(bmp.readTemperature());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    dataFile.println(" *C");
    dataFile.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    dataFile.print(bmp.readPressure());
    dataFile.println(" Pa");
  
    // Calculate altitude assuming 'standard' barometric    
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters"); 
  
    dataFile.print("Altitude = ");
    dataFile.print(bmp.readAltitude());
    dataFile.println(" meters"); 
 
  // you can get a more precise measurement of altitude  
  // if you know the current sea level pressure which will  
  // vary with weather and such. If it is 1015 millibars  
  // that is equal to 101500 Pascals.    

    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");        
    Serial.println();
 
    dataFile.print("Real altitude = ");
    dataFile.print(bmp.readAltitude(101500));
    dataFile.println(" meters");        
    dataFile.println();
    dataFile.close();
     // print to the serial port too:
     //Serial.println(dataString);
   }  
   // if the file isn't open, pop up an error:
   else {
     Serial.println("error opening datalog.txt");
   } 

  delay(3600000);
 
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


