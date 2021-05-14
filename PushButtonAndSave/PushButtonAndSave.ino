/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SparkFunDS3234RTC.h>
#include <SPI.h>
#include <SD.h>


#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin

File myFile;
int buttonPin = 6; 


void setup() {

 pinMode(buttonPin, INPUT);

// Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

    rtc.begin(DS13074_CS_PIN);


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

    rtc.update();

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  
  myFile = SD.open("test3.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test3.txt...");
    
    

    {
  myFile.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
    myFile.print('0'); // Print leading '0' for minute
  myFile.print(String(rtc.minute()) + ":"); // Print minute
  if (rtc.second() < 10)
    myFile.print('0'); // Print leading '0' for second
  myFile.print(String(rtc.second())); // Print second

  if (rtc.is12Hour()) // If we're in 12-hour mode
  {
    // Use rtc.pm() to read the AM/PM state of the hour
    if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
    else myFile.print(" AM");
  }
  
  myFile.print(" | ");

  // Few options for printing the day, pick one:
  myFile.print(rtc.dayStr()); // Print day string
  //Serial.print(rtc.dayC()); // Print day character
  //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
  myFile.print(" - ");
#ifdef PRINT_USA_DATE
  myFile.print(String(rtc.month()) + "/" +   // Print month
                 String(rtc.date()) + "/");  // Print date
#else
  myFile.print(String(rtc.date()) + "/" +    // (or) print date
                 String(rtc.month()) + "/"); // Print month
#endif
  myFile.println(String(rtc.year()) + " Program Has Been Restarted");        // Print year
}
   ;
    
    // close the file:
   myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test3.txt");
  if (myFile) {
    Serial.println("test3.txt:");
    

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
   
  }



   



}







void loop() {

rtc.update();

  if (!digitalRead(buttonPin)) {
   
  } else {
      myFile = SD.open("test3.txt", FILE_WRITE);

 
  if (myFile) {{
    {
  myFile.print(String(rtc.hour()) + ":"); 
  if (rtc.minute() < 10)
    myFile.print('0'); 
  myFile.print(String(rtc.minute()) + ":");
  if (rtc.second() < 10)
    myFile.print('0');
  myFile.print(String(rtc.second()));

  if (rtc.is12Hour())
  {
 
    if (rtc.pm()) Serial.print(" PM"); 
    else myFile.print(" AM");
  }
  
  myFile.print(" | ");

 
  myFile.print(rtc.dayStr()); 
  myFile.print(" - ");
#ifdef PRINT_USA_DATE
  myFile.print(String(rtc.month()) + "/" +   
                 String(rtc.date()) + "/");  
#else
  myFile.print(String(rtc.date()) + "/" +    
                 String(rtc.month()) + "/"); 
#endif
  myFile.println(String(rtc.year()) + " Button has been pushed");   
}
   ;
  }
    myFile.close();
    
      // re-open the file for reading:
  myFile = SD.open("test3.txt");
  if (myFile) {
    Serial.println("test3.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  }

  
  delay(10);        // delay in between reads for stability
  }
  }


