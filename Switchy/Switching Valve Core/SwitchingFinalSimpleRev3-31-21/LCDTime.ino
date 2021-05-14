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
