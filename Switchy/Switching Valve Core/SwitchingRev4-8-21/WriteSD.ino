void WriteSD(int i) {
  DateTime now = rtc.now();
  dataFile = SD.open("VALVELOG.TXT", FILE_WRITE);

  int add = 0;
     if (CheckDST() == 1) {
      int add = 1;
    }

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
    dataFile.print(i);

    if (CheckDST() == 1) {
      dataFile.print(' ');
      dataFile.print("+1hr for PDT");
    }
    else {
      dataFile.print(' ');
      dataFile.print("PST");
      }
    


    dataFile.println(' ');// Prints a new line
    dataFile.close();
  }
  else {
    Serial.println(F(" Data logging not occuring. Check SD card"));
     LCDPan("No data is logging. Check SD card!");
  }
}
