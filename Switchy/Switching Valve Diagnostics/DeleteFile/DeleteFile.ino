/*Valve controller for four way swagelock valve system. 


Matt Survilo
msurvilo@gmail.com

Oct, 5th 2018

----------------------------------------------------------------
/* SD card attached to SPI bus as follows.
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK  - pin 13
 ** CS   - pin 10 
 
 * RTC uses I2C. 
 ** SDA - pin A4
 ** SCL - pin A5

 
*/


#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"


RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//CONSTANTS

/*
#define POSITION_TIME  5000 // 5 seconds Each position change takes about 3seconds. 
#define SAMPLE_T1  295000// 4 mins 55 seconds  = 295 seconds = 295,000 milis
#define SAMPLE_T2  295000
#define SAMPLE_T3  295000
#define SAMPLE_T4  295000
*/

#define POSITION_TIME  2000 // 2 seconds Each position change takes about 3seconds. 
#define SAMPLE_T1  4000
#define SAMPLE_T2  4000
#define SAMPLE_T3  4000
#define SAMPLE_T4  4000

 

//PIN INFO
int relay1 = 9;
int relay2 = 8;
int relay3 = 7;
int relay4 = 6;
int relay5 = 5;

const int chipSelect = 10; //SD Chip Select

File dataFile;

void setup() {


  
  
  Serial.begin(57600);   // Open serial communications and wait for port to open:

if (! rtc.begin()) {     //Check to make sure RTC is online.
    Serial.println("Couldn't find RTC");
    while (1);
  }

  
  while (!Serial) {
    ;                    // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
                         // See if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed. Check SD card, wiring and chip select settings.");
                         // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");


 dataFile = SD.open("valvelog.txt", FILE_WRITE);                       
  if (dataFile) {
    Serial.print("Writing to valvelog.txt..."); 
    dataFile.close();
  }                   
  else {
    Serial.println("error opening valvelog.txt");
  }


dataFile = SD.open("valvelog.txt");              // Re-open the file for reading.
  if (dataFile) {
    Serial.println("valvelog.txt:");
    while (dataFile.available()) {               // Read from the file until there's nothing else in it:
      Serial.write(dataFile.read());
    }  
    dataFile.close();                            // Close the file.
  } else {
    Serial.println("Error opening valvelog.txt");// If the file never opened, print an error. 
  }

 
 // delete the file:
  Serial.println("Removing valvelog.txt...");
  SD.remove("valvelog.txt");

  if (SD.exists("valvelog.txt")) {
    Serial.println("valvelog.txt exists.");
  } else {
    Serial.println("valvelog.txt doesn't exist.");
  }  }
