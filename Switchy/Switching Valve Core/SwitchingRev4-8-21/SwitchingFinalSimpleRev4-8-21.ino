

/*/////////////////////////////////////////////////////////////////////////////////////
  PTR SWITCHING VALVE

  Written by Matt Survilo, Everrett Stiley,  and Carla Calderon.
  For questions email mfsurvilo@gmail.com



  PURPOSE - Code allows for switching between three valves used in collaboration with PTR-MS.
  ///////////////////////////////////////////////////////////////////////////////////

  UPDATES LOG - If you update this file in anyway are save it for use in the GBRL shared drive please comment below the version
  you have created and what modifications you made to the code.
  rev 8-13-19 : Carla - Added daily file saving, utilized EEPROM to save last stored program.
  rev 9-29-19 : Matt - Added text scrolling. Streamlined the valve programming process. Added clock display in the begining to check local time.
               Rearranged UI questioning.
  rev 11-20-19: Matt - Updated relay switching code to create time between solonoid switching. Was running into issues of system powering off. Updated
            DualPanChoose, DELAY and AOrB to all pass out buttonstate. Hopefully that will increase button reliability.
            Changed A or B to # and * b/c keypad was not working correctly.
  rev 7-29-20  Matt - Seperated UDFs , simplified UI by removing fist choice second choice etc. It will always go 1 through 4. eIf you don't want to use a
  valve then simply make the time at that position 0 seconds.
  rev 3-31-21 Matt - Changed interval1 - interval4 from int to long, also same for variable countdown. 
  rev 4-3-21 Matt - EEPROM not saving correct values, added eepromwritelong , eepromreadlong to store long values. Also changed eeprom addresses from 4,5,6,7 to the current values.
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
  EEPROMWritelong () - Writes a long value into 4 individual memory slots in the EEPROM.
  EEPROMReadlong () - Reads from the above four addresses, and reconstructs a long value from 4 ints. 


  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
*/

//library inclusion
//#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

//Global Variables

//Assign address location to each varible that will be stored long term(EEPROM)
int interval1addr = 1;
int interval2addr = 6;
int interval3addr = 11;
int interval4addr = 16;

int i = 0;
int programmed;
int master = 0;
int ii = 0;
int jj = 0;

//This is the chip select for the SD Card
const int chipSelect = 53;
int buttonstate = 0;

// Real time clock variables setup
long startMillis = 0;
int started = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// SD card pin setup on arduino board. This is for an adafruit SD reader
// vcc = 5v  DO = 50   DI = 51 CS = 53  SCK= 52
////////////////////////
//       LCD Setup    //
////////////////////////
LiquidCrystal lcd(45, 43, 41, 39, 37, 35);

//Starts or adds the RCT clock
RTC_DS3231 rtc;

//File Setup

//Creates a data file to be used later in the code.
//Store data in two places: one file will contain continuous data, the second file will have only the data from that day

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
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};
//rowPins[ROWS]
byte colPins[COLS] = {26, 27, 28, 29};
byte rowPins[ROWS] = {22, 23, 24, 25};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void setup() {
  //Relay variable initiallization
  int R1 = 2;
  int R2 = 3;
  int R3 = 4;
  int R4 = 5;

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
  Serial.println(F("Serial monitor functional"));

  //Does the RTC exist and function?
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC") );
    LCDPan("RTC Failure. Check battery.");
    while (1);
  }
  else {
    Serial.println(F("RTC is functional") );
  }

  //SD Card setup
  Serial.print(F("Initializing SD card..."));
  pinMode(chipSelect, OUTPUT);

  //Makes sure the SD card reader is functioning.
  if (!SD.begin(chipSelect)) {
    Serial.print(F("initialization failed!"));
    LCDPan("SD Failure. Check card.");
    LCDDualPan("SD Failure.", "Check card.");
    while (1);
  }
  Serial.print(F("initialization done"));
LogRestart();  //Logs in the SD card that a restart has occured.
 

  //First message displayed to user
  lcd.clear();
  lcd.print("The current ");
  lcd.setCursor(0, 1);
  lcd.print(F("time is ..."));
  delay(1500);
  lcd.clear();
  LCDtime();
  lcd.clear();
  lcd.print(F("Press any key to"));
  lcd.setCursor(0, 1);
  lcd.print(F("start"));
  master = 0;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void loop() {

  //Sampling interval initialization
 unsigned long interval1;
 unsigned long interval2;
 unsigned long interval3;
 unsigned long interval4;

  //Master is set to zero only in startup. It will only be 0 right when you start the program.
  if (master == 0) {
    
  

    //Waittime and threshold are used to say, if no key is pressed in this long of a time, do this....
    unsigned long waittime = millis();
    unsigned long threshold = 60000;  // Timeout threshold set to 1 minute. If no button is pressed it will go to last used values.
    
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

    //If no key was pushed program will go back to the previously selected settings
    if (ii == 0 && jj == 1) {

      //Valve choices and intervals from previous settings
      interval1 = EEPROMReadlong(interval1addr);
      interval2 = EEPROMReadlong(interval2addr);
      interval3 = EEPROMReadlong(interval3addr);
      interval4 = EEPROMReadlong(interval4addr);

      master = 1;       // Since the intervals etc have been set, master is now 1 and we should move on
      // and never reenter the previous loop. This is one of two ways master = 1. See below
    }

    // If a button was pushed within the first five minutes of the system being turned on a new program is set via the LCD screen and the keypad
    if (ii == 1 && jj == 0) {
      //Ask user if they want to use previously selected settings or choose new ones
      lcd.clear();
      lcd.print(F("Choose setting"));
      delay(2000);
      lcd.clear();
      DualPanChoose("Press * for last used settings", "Press # for new settings");

      //Option 1:Call for previous settings
      if (buttonstate == 1) {
        lcd.clear();
        //Valve choices and intervals from previous settings

      interval1 = EEPROMReadlong(interval1addr);
      interval2 = EEPROMReadlong(interval2addr);
      interval3 = EEPROMReadlong(interval3addr);
      interval4 = EEPROMReadlong(interval4addr);

        Serial.print(F("First duration is:"));
        Serial.println(interval1);
        lcd.print("Valve 1 duration");
        lcd.setCursor(0, 1);
        lcd.print(String(interval1) + " seconds...");
        delay (2000);
        lcd.clear();


        Serial.print(F("Second duration is:"));
        Serial.println(interval2);
        lcd.print("Valve 2 duration");
        lcd.setCursor(0, 1);
        lcd.print(String(interval2) + " seconds...");
        delay (2000);
        lcd.clear();

        Serial.print(F("Third duration is:"));
        Serial.println(interval3);
        lcd.print("Valve 3 duration");
        lcd.setCursor(0, 1);
        lcd.print(String(interval3) + " seconds...");
        delay (2000);
        lcd.clear();

        Serial.print(F("Fourth duration is:  "));
        Serial.println(interval4);
        lcd.print("Valve 4 duration");
        lcd.setCursor(0, 1);
        lcd.print(String(interval4) + " seconds...");
        delay (1000);
        lcd.clear();

        programmed = 2;  //Tells program is done storing all my values, want to start?
        master = 1;
      }
    }
   
    //Option 2: Call for new settings
    if (buttonstate == 2) {

      Serial.println(F("To submit selection press # symbol"));
 
      lcd.clear();

      LCDDualPan("To submit selection", "press # symbol");
      delay(1000);
      LCDDualPan("To use less", "locations...");
      delay(1000);
      LCDDualPan("Make sampling", "time zero");
      delay(2000);
      LCDDualPan("Clear selection", "with * key");
      delay(2000);

      Serial.println(F("Enter first sample duration then press #"));;
      lcd.clear();
      LCDDualPan("First duration" , "in seconds?");
      interval1 = GetNumber();
      EEPROMWritelong(interval1addr, interval1);
      Serial.print(F("First sample duration is "));
      Serial.println(interval1);

      Serial.println(F("Enter second sample duration then press #"));;
      lcd.clear();
      LCDDualPan("Second duration" , "in seconds?");
      interval2 = GetNumber();
      EEPROMWritelong(interval2addr, interval2);
      Serial.print(F("Second sample duration is "));
      Serial.println(interval2);

      Serial.println(F("Enter third sample duration then press #"));
      lcd.clear();
      LCDDualPan("Third duration" , "in seconds?");
      interval3 = GetNumber();
      EEPROMWritelong(interval3addr, interval3);
      Serial.print(F("Third sample duration is "));
      Serial.println(interval3);

      Serial.println(F("Enter fourth sample duration then press #"));
      lcd.clear();
      LCDDualPan("fourth location" , "duration?");
      interval4 = GetNumber();
      EEPROMWritelong(interval4addr, interval4);
      Serial.print("Fourth sample duration is ");
      Serial.println(interval4);

      programmed = 2; //Tells program is done storing all my new values, and that you should start sampling
      master = 1;            // Since the intervals etc have been set, master is now 1 and we should move on
      // and never reenter the previous loop.
    }
  }

  //Lets actually run the program!
  if (master == 1) {

    //////////////////////
    //First valve//
    //////////////////////

    //This resets currentMillis so that it can be compared to start millis.
    long currentMillis = millis();
    int logger = 0;  // Has this valve change been logged onto the SD card?  0 means no
    int countdown = 0; // Initializing variable
    startMillis = currentMillis;

    i = 1; // This says the first choice valve is running.
//      Serial.println("  ");
//      Serial.print("((interval1) + (startMillis-currentMillis)/1000 )) AKA countdown varialble   ");
//      Serial.println(((interval1) + (startMillis-currentMillis)/1000 ));
//      Serial.print("smillis-currentmillis  ");
//      Serial.println(startMillis-currentMillis);
//      Serial.print("currentMillis-startMillis  ");
//      Serial.println(currentMillis-startMillis);
//      Serial.print("(currentMillis - startMillis)  ");
//      Serial.println((currentMillis - startMillis));
//      Serial.print("(interval1 * 1000)");
//      Serial.println((interval1 * 1000));
//      
//      Serial.print("Interval =   ");
//      Serial.println(interval1);
//      Serial.print("Countdown =  ");
//      Serial.println(countdown);
//      Serial.print("Current millis =   ");
//      Serial.println(currentMillis);
//      Serial.print("startMillis =   ");
//      Serial.println(startMillis);
//      Serial.println("  ");
//      delay(500);
      
    while ((currentMillis - startMillis) < (interval1 * 1000)) {
      currentMillis = millis();
      ValveOn(i); // Depending on who firstchoice is, the appropriate relays will be turned on. Check the UDF below.

      //      This shows how many seconds are left on the clock

      if (logger == 0) {
        logger = 1;      // We are going to log
        WriteSerial(i);  // Displays changes in the serial port. Check UDF below
        WriteLCD(i);     // Writes on LCD What is going on. Check UDF below.
        WriteSD(i);      // Saves the the SD card. Check UDF below.
        WriteDailySD(i); // Saves to the SD card daily file. Check UDF below.
      }
      
      countdown = ((interval1 *1000) - currentMillis + startMillis)/1000;
      lcd.setCursor(0, 1);
      if (countdown < 10) {
        lcd.print("0");
      }
      if (countdown < 100) {
        lcd.print("0");
      }
      if (countdown < 1000) {
        lcd.print("0");
      }
      lcd.print(countdown);
    }
      

    startMillis = currentMillis;  // Resets the startmillis clock for the next cycle
    logger = 0;  // resets "have we logged?" Ready for the next valve to open.

    ////////////////////////
    //Second  valve choice//
    ////////////////////////
    i = 2;
    while ((currentMillis - startMillis) < (interval2 * 1000)) {
      currentMillis = millis();
      ValveOn(i);

      if (logger == 0) {
        WriteSerial(i);
        WriteLCD(i);
        WriteSD(i);
        WriteDailySD(i);
        logger = 1;
      }
      countdown = ((interval2 *1000) - currentMillis + startMillis)/1000;
      lcd.setCursor(0, 1);
      if (countdown < 10) {
        lcd.print("0");
      }
      if (countdown < 100) {
        lcd.print("0");
      }
      if (countdown < 1000) {
        lcd.print("0");
      }
      lcd.print(countdown);
    }
    startMillis = currentMillis;
    logger = 0;

    //////////////////////
    //Third valve choice//
    //////////////////////
    i = 3;
    while ((currentMillis - startMillis) < (interval3 * 1000)) {
      currentMillis = millis();
      ValveOn(i);

      if (logger == 0) {
        WriteSerial(i);
        WriteLCD(i);
        WriteSD(i);
        WriteDailySD(i);
        logger = 1;
      }
      countdown = ((interval3 * 1000) - currentMillis + startMillis) / 1000;
      lcd.setCursor(0, 1);
      if (countdown < 10) {
        lcd.print("0");
      }
      if (countdown < 100) {
        lcd.print("0");
      }
      if (countdown < 1000) {
        lcd.print("0");
      }
      lcd.print(countdown);
    }

    startMillis = currentMillis;
    logger = 0;

    ///////////////////////
    //Fourth valve choice//
    ///////////////////////
    i = 4;

    while ((currentMillis - startMillis) < (interval4 * 1000)) {
      currentMillis = millis();
      ValveOn(4);
      if (logger == 0) {
        ValveOn(i);
        WriteSerial(i);
        WriteLCD(i);
        WriteSD(i);
        WriteDailySD(i);
        logger = 1;
      }
      
      countdown = ((interval4 * 1000) - currentMillis + startMillis) / 1000;
     
      lcd.setCursor(0, 1);
      if (countdown < 10) {
        lcd.print("0");
      }
      if (countdown < 100) {
        lcd.print("0");
      }
      if (countdown < 1000) {
        lcd.print("0");
      }
      lcd.print(countdown);
    }

    startMillis = currentMillis;
    logger = 0;
    i = 1;
  }

}
