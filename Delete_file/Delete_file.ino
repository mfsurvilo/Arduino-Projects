
#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);


 SD.remove("test3.txt");


myFile = SD.open("test3.txt");

while (myFile.available()) {
      Serial.write(myFile.read());
 
}
}

//SD.remove("test.txt");
//SD.remove("test2.txt");
//SD.remove("test3.txt");




void loop() {
  // put your main code here, to run repeatedly:

}
