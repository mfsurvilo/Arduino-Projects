void WriteDailySD(int i) {
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

    dailyFile.print(i);


    dailyFile.println(' ');// Prints a new line
    dailyFile.close();
  }
  else {
    Serial.println(F("Daily data logging not occuring. Check SD card"));
    LCDpan("Data logging not occuring. Check SD card!  ");
  }
}
