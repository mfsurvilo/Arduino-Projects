void WriteSerial(int i) {
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
  Serial.print(F("VALVE  "));
    Serial.print(i);
Serial.print(F(" ON"));

   if (CheckDST() == 1) {
      Serial.print(' ');
      Serial.print("+1hr For PDT");
    }
  
}
