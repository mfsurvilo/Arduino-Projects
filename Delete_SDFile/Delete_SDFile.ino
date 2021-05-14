#include <SD.h>
#include <SPI.h>

const int chipSelect = 4;

void setup(){
 
  
    Serial.begin(9600);

    Serial.print("Initializing SD card...");
    // Make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(4, OUTPUT);

    // See if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        // Don't do anything more:
        return;
    }
    Serial.println("card initialized.");

    if(SD.exists("valvelog.txt"))
    {
        SD.remove("valvelog.txt");
        
        Serial.println("File removed sucessfully");
    }
    else
    {
        Serial.println("no file to remove");
    }
}

void loop(){

}
