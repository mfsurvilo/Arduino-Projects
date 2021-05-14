/*/////////////////////////////////////////////////////////////////////////////////////
PTR SWITCHING VALVE

Written by Everrett Stiley, Matt Survilo and Carla Calderon.
For questions email msurvilo@gmail.com



PURPOSE - Code allows for switching between three valves used in collaboration with PTR-MS. 
///////////////////////////////////////////////////////////////////////////////////

UPDATES LOG - If you update this file in anyway are save it for use in the GBRL shared drive please comment below the version
 you have created and what modifications you made to the code. 
 rev 8-13-19 : Carla - Added daily file saving, utilized EEPROM to save last stored program. 
 rev 9-29-19 : Matt - Added text scrolling. Streamlined the valve programming process. Added clock display in the begining to check local time. 
               Rearranged UI questioning.
 rev 11-20-19: Updated relay switching code to create time between solonoid switching. Was running into issues of system powering off. Updated
            DualPanChoose, DELAY and AOrB to all pass out buttonstate. Hopefully that will increase button reliability.
            Changed A or B to # and * b/c keypad was not working correctly.
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
UDFs - Functions and their purpose.

WriteSD() - Saves logging locations to the SD card in a text file called VALVELOG.TXT
WriteDailySD() - Saves daily logging locations to the SD card in text files. Text files are titled for using the date.
WriteSerial() - Writes logging locations and activity to the serial monitor.
WriteLCD() - Writes logging locations and activity to the LCD screen.
ValveOn() - Input argument is an integer. Based on logic developed in code appropriate relays will turn on. 
LCDPan() - Input argument is a single string. Screen will pan to end of expression twice and then pause. 
LCDDualPan() -  Input argument is two strings. Screen will pan to end of longest expression twice and then pause. 
DualPanChoose() -  Input argument is two string. Screen will pan until A or B is chosen. 
GetNumber() - Waits on keypad input. Numbers are entered using # sym. 
DELAY() - Input arguement is an integer. Works like built in delay() function but it is run on millis(). 
          You can embed a function inside of it easily. 
AOrB() - Waits for keyapd input using A or B keys only. 
LCDtime() - Displays the current time int the LCD screen.
GetFileName() - Creates the daily filename to be referenced elsewhere. 

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
*/

//library inclusion
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

//Enter Sampling Locations in ascending order
int settingchoice;
int firstchoice = 1;
int secondchoice = 2;
int thirdchoice = 3;
int fourthchoice = 4;

//Sampling interval
unsigned long interval1;
unsigned long interval2;
unsigned long interval3;
unsigned long interval4;

//Assign address location to each varible that will be stored long term(EEPROM)
int firstchoiceaddr = 0;
int secondchoiceaddr = 1;
int thirdchoiceaddr = 2;
int fourthchoiceaddr = 3;
int interval1addr = 4;
int interval2addr = 5;
int interval3addr = 6;
int interval4addr = 7;

unsigned long period1;
unsigned long period2;
unsigned long period3;
unsigned long period4;



//The defining the relay pins. 
int R1 = 2;
int R2 = 3;
int R3 = 4;
int R4 = 5;

// These help the code process, and are used as benchmarks as programming with the keypad progresses. 
int i = 0;
int programmed;
int master = 0;
int ii = 0;
int jj = 0;

//This is the chip select for the SD Card
const int chipSelect = 53;
int buttonstate = 0;

// Real time clock variables setup
unsigned long startMillis = 0;
int started = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};




// SD card pin setup on arduino board. This is for an adafruit SD reader
// vcc = 5v  DO = 50   DI = 51 CS = 53  SCK= 52

//LCD pins
LiquidCrystal lcd(45, 43, 41, 39, 37, 35);

//Starts or adds the RCT clock 
RTC_DS3231 rtc;

//File Setup
//Creates a data file to be used later in the code. 
//Store data in two places: one file will contain continuous data, the second file will have only the data from that day

//Below is a work in progress-carla

char filename[ ] = "00000000.TXT"; // Should be 8 characters or less. Should be the date

File dataFile;
File dailyFile;

////////////////////////
//USB and Keypad Setup//
////////////////////////

// keypad setup
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
//rowPins[ROWS]
byte colPins[COLS] = {26, 27, 28, 29}; 
byte rowPins[ROWS] = {22, 23, 24, 25}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void setup() {
    
  //Declaring that the pins associated with the relays are to be OUTPUTS
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  //Starts LCD 16 columns, 2 rows
  lcd.begin(16, 2);
  
  //Declaring that the SD chip select pin is an OUTPUT
  pinMode(chipSelect, OUTPUT);

  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
  }
  
  //Sets baud rate. If the serial monitor is not displaying correctly make sure this number matches what is chosen. 
  Serial.begin(9600);
  Serial.print(" ");
  Serial.println("Serial monitor functional");

  //Does the RTC exist and function?
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    LCDpan("RTC Failure. Check battery.");
    while (1);
  }
  else{
    Serial.println("RTC is functional");
  }

  //Defines "now" as what ever the rtc says it is. 
  DateTime now = rtc.now();

  //SD Card setup
  Serial.print("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);

  //Makes sure the SD card reader is functioning. 
  if (!SD.begin(chipSelect)) {
      Serial.print("initialization failed!");
       LCDpan("SD Failure. Check card.");
       LCDDualPan("SD Failure.","Check card.");
      while (1);
  }
  Serial.print("initialization done");

  //////////////////////////
  //Datafile initial print//
  //////////////////////////

  //This line will either open the file, if it already exists or write it and open it if it doesn't.
  //Add and open another separate file
  dataFile = SD.open("VALVELOG.TXT", FILE_WRITE);
  GetFileName();
  dailyFile = SD.open(filename, FILE_WRITE);
  
  //If it opened correctly print the following. This gets saved onto the SD Card everytime the program is restarted. 
  //That way it's easy to see in the data file if there was ever a power loss etc. 
  if (dataFile) {
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(' ');
    dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    dataFile.print(' ');
    // If the number is less than ten add a zero so that it can maintain the proper format. 
    if (now.hour() < 10)
      dataFile.print('0');
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    if (now.minute() < 10)
      dataFile.print('0');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    if (now.second() < 10)
      dataFile.print('0');
    dataFile.print(now.second(), DEC);
    dataFile.print(' ');
    dataFile.print("Valve");
    dataFile.print(' ');
    dataFile.print("PROGRAM_RESTARTED");
    dataFile.println(' ');
    // This next line is very important. It closes aka saves the new things you just wrote. Make sure everytime you write something
    // that you use this. 
    dataFile.close();
  }

  if (dailyFile) {
    dailyFile.print(now.year(), DEC);
    dailyFile.print('/');
    dailyFile.print(now.month(), DEC);
    dailyFile.print('/');
    dailyFile.print(now.day(), DEC);
    dailyFile.print(' ');
    dailyFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    dailyFile.print(' ');
    // If the number is less than ten add a zero so that it can maintain the proper format. 
    if (now.hour() < 10)
      dailyFile.print('0');
    dailyFile.print(now.hour(), DEC);
    dailyFile.print(':');
    if (now.minute() < 10)
      dailyFile.print('0');
    dailyFile.print(now.minute(), DEC);
    dailyFile.print(':');
    if (now.second() < 10)
      dailyFile.print('0');
    dailyFile.print(now.second(), DEC);
    dailyFile.print(' ');
    dailyFile.print("Valve");
    dailyFile.print(' ');
    dailyFile.print("PROGRAM_RESTARTED");
    dailyFile.print(' ');
    // This next line is very important. It closes/saves the new things you just wrote. Make sure everytime you write something
    // that you use this. 
    dailyFile.close();
  }

  //Very first message displayed to user 
  lcd.clear();
  lcd.print("The current ");
  lcd.setCursor(0,1);
  lcd.print("time is ...");
  delay(700);
  lcd.clear();
  LCDtime();
  
  lcd.clear();

  lcd.print("Press any key to");
  lcd.setCursor(0,1);
  lcd.print("start");
 master = 0;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void loop() {
  //Master is set to zero only in setup. It will only be 0 right when you start the program. 
  if (master == 0) {
    
    //Waittime and threshold are used to say, if no key is pressed in this long of a time, do this.... 
    unsigned long waittime = millis();
    unsigned long threshold = 60000;  //1 minute

    char key = keypad.getKey();  // key = NO_KEY if a key is never pressed



    // If a key is pressed this sets ii = 1
    if (key != NO_KEY) {
      ii = 1; 
      jj = 0;
    }
  
    // If instead the waittime exceeds the threshold, this sets jj = 1. Depending on which happens, 
    // either the previously chosen program will run, or you will reset and start a new one. 
    if (waittime > threshold) {
      ii = 0;
      jj = 1;
    }
    
    //If no key was pushed This happens.   
    //Program will go back to the previously selected settings
    if (ii == 0 && jj == 1) {
      
      //Valve choices and intervals from previous settings
      firstchoice = EEPROM.read(0);
      secondchoice = EEPROM.read(1);
      thirdchoice = EEPROM.read(2);
      fourthchoice = EEPROM.read(3);
      interval1 = EEPROM.read(4);
      interval2 = EEPROM.read(5);
      interval3 = EEPROM.read(6);
      interval4 = EEPROM.read(7);
      // This makes the times selected by the keypad into seconds. If this were not here it would be milli seconds. 
      period1 = interval1*1000;
      period2 = interval2*1000;
      period3 = interval3*1000;
      period4 = interval4*1000;
      master = 1;       // Since the intervals etc have been set, master is now 1 and we should move on 
                        // and never reenter the previous loop. This is one of two ways master = 1. See below
    }

    // If a button was pushed within the first five minutes of the system being turned on,
    // this is how a new program is set via the LCD screen and the keypad
    if (ii ==1 && jj == 0) { 
      //Ask user if they want to use previously selected settings or choose new ones 
      lcd.clear();
      lcd.print("Choose setting");
      delay(2000);
      lcd.clear();
      DualPanChoose("Press * for last used settings","Press # for new settings");

    
      //Option 1:Call for previous settings
      if (buttonstate == 1){
        lcd.clear();
        //Valve choices and intervals from previous settings
        firstchoice = EEPROM.read(0);
        secondchoice = EEPROM.read(1);
        thirdchoice = EEPROM.read(2);
        fourthchoice = EEPROM.read(3);
        interval1 = EEPROM.read(4);
        interval2 = EEPROM.read(5);
        interval3 = EEPROM.read(6);
        interval4 = EEPROM.read(7);
      /*
        Serial.println("First valve is: ");
        Serial.println(firstchoice);
        Serial.print("First duration is:");
        Serial.println(interval1);        
        lcd.print("Valve " + String(firstchoice) + " duration");
        lcd.setCursor(0,1);
        lcd.print(String(interval1) + " seconds...");
        delay (2000);
        lcd.clear();

        Serial.print("Second valve is: ");
        Serial.println(secondchoice);
        Serial.print("Second duration is:");
        Serial.println(interval2);
        lcd.print("Valve " + String(secondchoice) + " duration");
        lcd.setCursor(0,1);
        lcd.print(String(interval2) + " seconds...");
        delay (2000);
        lcd.clear();
        

        Serial.print("Third valve is: ");
        Serial.println(thirdchoice);
        Serial.print("Third duration is:");
        Serial.println(interval3);
        lcd.print("Valve " + String(thirdchoice) + " duration");
        lcd.setCursor(0,1);
        lcd.print(String(interval3) + " seconds...");
        delay (2000);
        lcd.clear();

       
        Serial.println(" ");
        Serial.print("Fourth valve is: ");
        Serial.println(fourthchoice);
        Serial.print("Fourth duration is:  ");
        Serial.println(interval4);
        lcd.print("Valve " + String(fourthchoice) + " duration");
        lcd.setCursor(0,1);
        lcd.print(String(interval4) + " seconds...");
        delay (1000);
        lcd.clear();
*/
        programmed = 2;  //Tells program is done storing all my values, want to start?
        master = 1;
        }
      }

      //Option 2: Call for new settings

      if (buttonstate == 2) {
        //Valve 1 choice and sample time
        Serial.println("To submit selection press # symbol");
        Serial.println("Enter first valve location number");
        lcd.clear();
        
       LCDDualPan("To submit selection","press # symbol");
        delay(1000);
        LCDDualPan("To use less","locations...");
        delay(2000);
        LCDDualPan("Make sampling", "time zero");
        delay(2000);
        LCDDualPan("Clear selection", "with * key");
        delay(2000);
        
        // First Choice
        lcd.clear();
        lcd.print("Pick first");
        lcd.setCursor(0,1);
        lcd.print("location #");
        firstchoice = GetNumber();
        EEPROM.update(firstchoiceaddr,firstchoice);
        Serial.println("First valve is ");
        Serial.println(firstchoice);

        Serial.println("Enter first sample duration then press #");;
        lcd.clear();
        LCDDualPan("First duration" ,"in seconds?");
        interval1 = GetNumber();
        EEPROM.update(interval1addr, interval1);
        Serial.print("First sample duration is ");
        Serial.println(interval1);
       
        //Second Choice      
        lcd.clear();
        lcd.print("Pick second");
        lcd.setCursor(0,1);
        lcd.print("location #");
        secondchoice = GetNumber();
        EEPROM.update(secondchoiceaddr,secondchoice);
        Serial.print("Second valve is ");
        Serial.println(secondchoice);

        Serial.println("Enter second sample duration then press #");;
        lcd.clear();
        LCDDualPan("Second duration" ,"in seconds?");
        interval2 = GetNumber();
        EEPROM.update(interval2addr, interval2);
        Serial.print("Second sample duration is ");
        Serial.println(interval2);

        //Third Choice      
        lcd.clear();
        lcd.print("Pick third");
        lcd.setCursor(0,1);
        lcd.print("location #");
        thirdchoice = GetNumber();
        EEPROM.update(thirdchoiceaddr,thirdchoice);
        Serial.print("Third valve is ");
        Serial.println(thirdchoice);

        delay(1000);
        Serial.println("Enter third sample duration then press #");;
        lcd.clear();
        LCDDualPan("Third duration" ,"in seconds?");
        interval3 = GetNumber();
        EEPROM.update(interval3addr, interval3);
        Serial.print("Third sample duration is ");
        Serial.println(interval3);

        //Fourth Choice
        
        lcd.clear();
        lcd.print("Pick fourth");
        lcd.setCursor(0,1);
        lcd.print("location #");
        fourthchoice = GetNumber();
        EEPROM.update(fourthchoiceaddr,fourthchoice);
        Serial.print("Fourth valve is ");
        Serial.println(fourthchoice);

        LCDDualPan("Fourth duration", "in seconds?");
        delay(1000);
        Serial.println("Enter fourth sample duration then press #");
        lcd.clear();
        LCDDualPan("fourth location" ,"duration?");
        interval4 = GetNumber();
        EEPROM.update(interval4addr, interval4);
        Serial.print("Fourth sample duration is ");
        Serial.println(interval4);
      
        programmed = 2; //Tells program is done storing all my new values, and that you should start sampling
        master = 1;            // Since the intervals etc have been set, master is now 1 and we should move on 
                              // and never reenter the previous loop. 
      }
    }


  //Lets actually run the program!
  if (master == 1) {
 
  // This makes the times selected by the keypad into seconds. If this were not here it would be milli seconds. 
    period1 = interval1*1000;
    period2 = interval2*1000;
    period3 = interval3*1000;
    period4 = interval4*1000;
  
    //////////////////////
    //First valve choice//
    //////////////////////
    
    //This resets currentMillis so that it can be compared to start millis. 
    unsigned long currentMillis = millis();       
    int logger = 0;  // Has this valve change been logged onto the SD card?  0 means no
 
    startMillis = currentMillis;

    i=1;  // This says the first choice valve is running. 
    while ((currentMillis - startMillis) < period1) {
      currentMillis = millis();
      ValveOn(firstchoice); // Depending on who firstchoice is, the appropriate relays will be turned on. Check the UDF below. 

//      This shows how many seconds are left on the clock

      if (logger == 0) {
        logger = 1; // We are going to log 
        WriteSerial();  // Displays changes in the serial port. Check UDF below
        WriteLCD();     // Writes on LCD What is going on. Check UDF below. 
        WriteSD();      // Saves the the SD card. Check UDF below. 
        WriteDailySD(); // Saves to the SD card daily file. Check UDF below. 
      }
      int countdown = (period1 - currentMillis + startMillis)/1000;
      lcd.setCursor(0,1);
      if (countdown <10) {   //If it's more than 10 add a zero. 
        lcd.print("0");}
      if (countdown <100) { //If it's more than 100 add another zero . . . etc
        lcd.print("0");}
      if (countdown <1000) {
        lcd.print("0");}
      lcd.print(countdown);
      Serial.print(countdown); 
    }

    
    startMillis = currentMillis;  // Resets the startmillis clock for the next cycle
    logger = 0;  // resets "have we logged?" Ready for the next valve to open. 

  ////////////////////////
  //Second  valve choice//
  ////////////////////////
  i=2;
        while ((currentMillis - startMillis) < period2) {
        currentMillis = millis();
        ValveOn(secondchoice);
  
        if (logger == 0) {
          WriteSerial();
          WriteLCD();
          WriteSD();
          WriteDailySD();
          logger = 1;
        }
        int countdown = (period2 - currentMillis + startMillis)/1000;
        lcd.setCursor(0,1);
        if (countdown <10) {
          lcd.print("0");}
          if (countdown <100) {
          lcd.print("0");}
           if (countdown <1000) {
          lcd.print("0");}
        lcd.print(countdown); 
        }
        startMillis = currentMillis;
        logger = 0;
     
  //////////////////////
  //Third valve choice//
  //////////////////////
  i=3;
      while ((currentMillis - startMillis) < period3) {
        currentMillis = millis();
        ValveOn(thirdchoice);        

        if (logger == 0) {
          WriteSerial();   
          WriteLCD();
          WriteSD();
          WriteDailySD();
          logger = 1;
        }
        int countdown = (period3 - currentMillis + startMillis)/1000;
        lcd.setCursor(0,1);
        if (countdown <10) {
          lcd.print("0");}
          if (countdown <100) {
          lcd.print("0");}
           if (countdown <1000) {
          lcd.print("0");}
        lcd.print(countdown); 
        }
        

        startMillis = currentMillis;
        logger = 0;
    
  ///////////////////////
  //Fourth valve choice//
  ///////////////////////
  i=4;
  
      while ((currentMillis - startMillis) < period4) {
        currentMillis = millis();
        ValveOn(fourthchoice);        
        if (logger == 0) {
          ValveOn(fourthchoice);
          WriteSerial();
          WriteLCD();
          WriteSD();
          WriteDailySD();
          logger = 1;
        }
        int countdown = (period4 - currentMillis + startMillis)/1000;
        lcd.setCursor(0,1);
        if (countdown <10) {
          lcd.print("0");}
          if (countdown <100) {
          lcd.print("0");}
           if (countdown <1000) {
          lcd.print("0");}
        lcd.print(countdown); 
        }
       
        startMillis = currentMillis;
        logger = 0;
        i=1;      
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void WriteSD() {
  DateTime now = rtc.now();
  dataFile = SD.open("VALVELOG.TXT", FILE_WRITE);
  
  if (dataFile) {   // If the data file opens correctly do the following, else check below. 
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(' ');
    dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    dataFile.print(' ');
    if (now.hour() < 10)
      dataFile.print('0');
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    if (now.minute() < 10)
      dataFile.print('0');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    if (now.second() < 10)
      dataFile.print('0');
    dataFile.print(now.second(), DEC);
    dataFile.print(' ');
      
    dataFile.print("VALVE ");
    
    if (i==1) {             // i = 1 means this is the first valve to be opened. 
      if (firstchoice == 1) {
        dataFile.print("1");}
      if (firstchoice == 2) {
        dataFile.print("2");}
      if (firstchoice == 3) {
        dataFile.print("3");}
      if (firstchoice == 4) {
        dataFile.print("4");}
    }
    
    if (i==2) {          // i = 2 means this is the second valve to be opened. 
      if (secondchoice == 1) {
        dataFile.print("1");}
      if (secondchoice == 2) {
        dataFile.print("2");}
      if (secondchoice == 3) {
         dataFile.print("3");}
      if (secondchoice == 4) {
        dataFile.print("4");}
    }
    if (i==3) {          // i = 3 means this is the third valve to be opened. 
      if (thirdchoice == 1) {
        dataFile.print("1");}
      if (thirdchoice == 2) {
        dataFile.print("2");}
      if (thirdchoice == 3) {
        dataFile.print("3");}
      if (thirdchoice == 4) {
        dataFile.print("4");}
    }
    if (i==4) {          // i = 4 means this is the fourth valve to be opened. 
      if (fourthchoice == 1) {
        dataFile.print("1");}
      if (fourthchoice == 2) {
        dataFile.print("2");}
      if (fourthchoice == 3) {
        dataFile.print("3");}
      if (fourthchoice == 4) {
        dataFile.print("4");}
    }

    dataFile.println(' ');// Prints a new line
    dataFile.close();
  }
  else {
    Serial.println(" Data logging not occuring. Check SD card");
        LCDpan("No data is logging. Check SD card!");
  }
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void WriteDailySD() {
  DateTime now = rtc.now();
  GetFileName();
  dailyFile = SD.open(filename, FILE_WRITE);
  
  if (dailyFile) {   //If the daily file opens correctly do the following, else check below. 
    dailyFile.print(now.year(), DEC);
    dailyFile.print('/');
    dailyFile.print(now.month(), DEC);
    dailyFile.print('/');
    dailyFile.print(now.day(), DEC);
    dailyFile.print(' ');
    dailyFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    dailyFile.print(' ');
    if (now.hour() < 10)
      dailyFile.print('0');
    dailyFile.print(now.hour(), DEC);
    dailyFile.print(':');
    if (now.minute() < 10)
      dailyFile.print('0');
    dailyFile.print(now.minute(), DEC);
    dailyFile.print(':');
    if (now.second() < 10)
      dailyFile.print('0');
    dailyFile.print(now.second(), DEC);
    dailyFile.print(' ');
      
    dailyFile.print("VALVE ");
    
    if (i==1) {             // i = 1 means this is the first valve to be opened. 
      if (firstchoice == 1) {
        dailyFile.print("1");}
      if (firstchoice == 2) {
        dailyFile.print("2");}
      if (firstchoice == 3) {
        dailyFile.print("3");}
      if (firstchoice == 4) {
        dailyFile.print("4");}
    }
    
    if (i==2) {          // i = 2 means this is the second valve to be opened. 
      if (secondchoice == 1) {
        dailyFile.print("1");}
      if (secondchoice == 2) {
        dailyFile.print("2");}
      if (secondchoice == 3) {
        dailyFile.print("3");}
      if (secondchoice == 4) {
        dailyFile.print("4");}
    }
    if (i==3) {          // i = 3 means this is the third valve to be opened. 
      if (thirdchoice == 1) {
        dailyFile.print("1");}
      if (thirdchoice == 2) {
        dailyFile.print("2");}
      if (thirdchoice == 3) {
        dailyFile.print("3");}
      if (thirdchoice == 4) {
        dailyFile.print("4");}
    }
    if (i==4) {          // i = 4 means this is the fourth valve to be opened. 
      if (fourthchoice == 1) {
        dailyFile.print("1");}
      if (fourthchoice == 2) {
        dailyFile.print("2");}
      if (fourthchoice == 3) {
        dailyFile.print("3");}
      if (fourthchoice == 4) {
        dailyFile.print("4");}
    }

    dailyFile.println(' ');// Prints a new line
    dailyFile.close();

  } 
  else {
    Serial.println("Daily data logging not occuring. Check SD card");
    LCDpan("Data logging not occuring. Check SD card!  ");
  }
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//Basically the same thing as above but the word 'dataFile' is replaced with 'Serial' so it prints in the serial monitor
void WriteSerial() {
  DateTime now = rtc.now();
  Serial.println();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(' ');
  if (now.hour() < 10)
    Serial.print('0');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  if (now.minute() < 10)
    Serial.print('0');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  if (now.second() < 10)
    Serial.print('0');
  Serial.print(now.second(), DEC);
  Serial.print(' ');
  Serial.print("VALVE  ");
  if (i==1) {
    if (firstchoice == 1) {
      Serial.print("1");}
    if (firstchoice == 2) {
      Serial.print("2");}
    if (firstchoice == 3) {
      Serial.print("3");}
    if (firstchoice == 4) {
      Serial.print("4");}
  }
  if (i==2) {
    if (secondchoice == 1) {
      Serial.print("1");}
    if (secondchoice == 2) {
      Serial.print("2");}
    if (secondchoice == 3) {
      Serial.print("3");}
    if (secondchoice == 4) {
      Serial.print("4");}
    }
  if (i==3) {
    if (thirdchoice == 1) {
      Serial.print("1");}
    if (thirdchoice == 2) {
      Serial.print("2");}
    if (thirdchoice == 3) {
      Serial.print("3");}
    if (thirdchoice == 4) {
      Serial.print("4");}
  }
  if (i==4) {
    if (fourthchoice == 1) {
      Serial.print("1");}
    if (fourthchoice == 2) {
      Serial.print("2");}
    if (fourthchoice == 3) {
      Serial.print("3");}
    if (fourthchoice == 4) {
      Serial.print("4");}
  }  
  Serial.print(" ON");
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//LCD Pin setup from right to left. 
//GND
//Resistor for backlight to 5V
//40
//41
//42
//43
//blank
//blank
//blank
//blank
//44
//GND
//45
//Potentiometer for brightness
//5V
//Ground

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void WriteLCD() {       //Same as the above two things but it writes the the LCD screen
  lcd.begin(16,2);      // I'm not sure why this is necessary
  lcd.setCursor(0,0);   // Where do you want to start writing words?

  lcd.print("VALVE ");
   if (i==1) {
    if (firstchoice == 1) {
      lcd.print("1");}
    if (firstchoice == 2) {
      lcd.print("2");}
    if (firstchoice == 3) {
      lcd.print("3");}
    if (firstchoice == 4) {
      lcd.print("4");}
    }
      if (i==2) {
    if (secondchoice == 1) {
      lcd.print("1");}
    if (secondchoice == 2) {
      lcd.print("2");}
    if (secondchoice == 3) {
      lcd.print("3");}
    if (secondchoice == 4) {
      lcd.print("4");}
    }
      if (i==3) {
    if (thirdchoice == 1) {
      lcd.print("1");}
    if (thirdchoice == 2) {
      lcd.print("2");}
    if (thirdchoice == 3) {
      lcd.print("3");}
    if (thirdchoice == 4) {
      lcd.print("4");}
    }
      if (i==4) {
    if (fourthchoice == 1) {
      lcd.print("1");}
    if (fourthchoice == 2) {
      lcd.print("2");}
    if (fourthchoice == 3) {
      lcd.print("3");}
    if (fourthchoice == 4) {
      lcd.print("4");}
    }
   
  lcd.print(" ON");
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void ValveOn(int j){    // You'll notice that in the code above, ValveOn always has an input argument
  
    if (j==1){ 
      digitalWrite(R1, LOW);
      //delay(1000);
      digitalWrite(R2, LOW);
      //delay(1000);
      digitalWrite(R3, LOW);
    }
  
    if (j==2) {  
      digitalWrite(R1, LOW);
      //delay(1000);
      digitalWrite(R2, LOW);
      //delay(1000);
      digitalWrite(R3, HIGH);
    }
    
    if(j==3) {   //For valve number 3 to be turned on only Relay 1 needs to be turned down. 
      digitalWrite(R1, LOW);
      //delay(1000);
      digitalWrite(R3, LOW);
      //delay(1000);
      digitalWrite(R2, HIGH);
    }
    
    if(j==4) {  // In order for valve four to be opened two realys need to be turned down, 1 and 3
      digitalWrite(R3, LOW);
    //  delay(1000);
      digitalWrite(R2, HIGH);
     // delay(1000);
      digitalWrite(R1, HIGH);
    }
  
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void LCDpan(const String& mystring)
{
  lcd.clear();
  lcd.print(mystring); 
  delay(500);
  int shift = mystring.length() - 16;
    if (shift > 0) {
        for (int rep = 1; rep < 3; rep++){
          for (int positionCounter = 0; positionCounter<shift; positionCounter++){
           lcd.scrollDisplayLeft();
           delay(450);
           }
          delay(1000);
          lcd.clear();
          lcd.print(mystring);
          delay(1000);
          }
          delay(1000);
        }
}
      
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void DualPanChoose(const String& mystring,const String& mystring2)
{
  lcd.clear();
  lcd.print(mystring); 
  lcd.setCursor(0,1);
  lcd.print(mystring2);
  int shift; 
  int shift1 = mystring.length() - 16;// length of strings to write
  int shift2 = mystring2.length() - 16;
  int delay1 = 0;                   // this delay changes every so often
  unsigned long cMillis = millis(); // current millis constantly updates
  unsigned long  sMillis = cMillis; // start millis is declared every once in a while
  buttonstate = 0;
  
  shift = max(shift1, shift2);

 if (shift > 0) {
    while (buttonstate == 0){
      
      for (int positionCounter = 0; positionCounter<shift; positionCounter++){  
       lcd.scrollDisplayLeft();
       buttonstate = DELAY(300);
       if (buttonstate != 0){
        break;
       }
      }
      buttonstate = DELAY(1000);
      
     if (buttonstate != 0){
       break;
      }
      
     lcd.clear();
     lcd.print(mystring); 
     lcd.setCursor(0,1);
     lcd.print(mystring2);
     DELAY(1000);
     if (buttonstate != 0){
       break;
     }
   } 
 }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void LCDDualPan(const String& mystring,const String& mystring2)
{
  lcd.clear();
  lcd.print(mystring); 
  lcd.setCursor(0,1);
  lcd.print(mystring2);
  int shift1 = mystring.length()-16;
  int shift2 = mystring2.length()-16;
  int delay1 = 0;                   // this delay changes every so often
  unsigned long cMillis = millis(); // current millis constantly updates
  unsigned long  sMillis = cMillis; // start millis is declared every once in a while
  buttonstate = 0;

  int shift = max(shift1, shift2);

  if (shift > 0) {
    for (int rep = 0; rep<2; rep++){
      for (int positionCounter = 0; positionCounter<shift; positionCounter++){
       lcd.scrollDisplayLeft();
       delay(300);
       }
     
       lcd.clear();
       lcd.print(mystring); 
       lcd.setCursor(0,1);
       lcd.print(mystring2);
       delay(1000);
    }
  } 
}
  
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

unsigned long GetNumber() {
   int num = 0;
   char key = keypad.getKey();


   while(key != '#')
   {   
      switch (key)
      {
         case NO_KEY:
            break;
         
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            lcd.clear(); 
            Serial.println(key);        
            num = num * 10 + (key - '0');
            lcd.clear(); 
            lcd.print(num);   
            break;

         case '*':
            
            num = 0;
            lcd.clear(); 
            lcd.print("                ");
            ;         
      }
      key = keypad.getKey();
   }
   return num;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int DELAY(int howlong){    

        unsigned long cMillis = millis(); // start millis is declared every once in a while

        unsigned long sMillis = cMillis;
      while ((cMillis - sMillis) < howlong) {
         buttonstate = AOrB();
         cMillis = millis();
         } 
       return buttonstate;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int AOrB() {
   char key = keypad.getKey();
      switch (key)
      {
        case '*':
            lcd.clear(); 
            lcd.print("*");
            delay(1000);
            buttonstate = 1;
            return buttonstate;
            break;  

        case '#': 
            lcd.clear(); 
            lcd.print("#");
            delay(1000);
            buttonstate = 2;       
            return buttonstate;
            break;      
      }
      key = keypad.getKey();
      return buttonstate;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void LCDtime(){  //  Displays the current time in the LCD screen
  for (int upd = 1; upd < 6; upd++){
  lcd.clear();
  lcd.print("The current time is...");
  lcd.clear();
  DateTime now = rtc.now();
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0,1);
    if (now.hour() < 10)
    lcd.print('0');
  lcd.print(now.hour(), DEC);
  lcd.print(':');
    if (now.minute() < 10)
    lcd.print('0'); 
  lcd.print(now.minute(), DEC);
  lcd.print(':');
    if (now.second() < 10)
    lcd.print('0');
  lcd.print(now.second(), DEC);
  lcd.print(' ');
  if (now.hour() > 12){
  lcd.print("PM");
  }
  else{
    lcd.print("AM");
  }
  delay(1000);
  }
  }

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void GetFileName(){
  Wire.begin(); //Important for RTClib.h
  rtc.begin();
  DateTime now = rtc.now();
  filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
  filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
  filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
  filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
  filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
  filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
  filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
  filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()

  return ;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
