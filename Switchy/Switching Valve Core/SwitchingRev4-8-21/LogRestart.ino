void LogRestart(void) {
  //////////////////////////
  //Datafile initial print//
  //////////////////////////

  //Defines "now" as what ever the rtc says it is.
  DateTime now = rtc.now();

  //This line will either open the file, if it already exists or write it and open it if it doesn't.
  //Add and open another separate file
  dataFile = SD.open("VALVELOG.TXT", FILE_WRITE);
  GetFileName();
  dailyFile = SD.open(filename, FILE_WRITE);

  //If it opened correctly print the following. This gets saved onto the SD Card everytime the program is restarted.
  //That way it's easy to see in the data file if there was ever a power loss etc.
  if (dataFile) {
    Serial.print("Opened main data file");
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
    dataFile.print(F("Valve"));
    dataFile.print(' ');
    dataFile.print("PROGRAM_RESTARTED");

    if (CheckDST() == 1) {
      dataFile.print(' ');
      dataFile.print("+1hr for PDT");
    }
    else {
      dataFile.print(' ');
      dataFile.print("PST");
    }

    dataFile.println(' ');
    // This next line is very important. It closes aka saves the new things you just wrote. Make sure everytime you write something
    // that you use this.
    dataFile.close();
  }

  if (dailyFile) {
     Serial.print("Opened daily data file");
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
    dailyFile.print(F("Valve"));
    dailyFile.print(' ');
    dailyFile.print(F("PROGRAM_RESTARTED"));
    dailyFile.println(' ');

    if (CheckDST() == 1) {
      dailyFile.print(' ');
      dailyFile.print("+1hr for PDT");
    }
    else {
      dailyFile.print(' ');
      dailyFile.print("PST");
    }

    // This next line is very important. It closes/saves the new things you just wrote. Make sure everytime you write something
    // that you use this.
    dailyFile.close();
  }


}
