void LCDtime() { //  Displays the current time in the LCD screen
  int add = 0;
  for (int upd = 1; upd < 6; upd++) {
     if (CheckDST() == 1) {
       add = 1;
       Serial.print("added 1");
    }
    
    lcd.clear();
    lcd.print("The current time is...");
    lcd.clear();
    DateTime now = rtc.now();
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.setCursor(0, 1);
    if (now.hour() < 10)
      lcd.print('0');
    lcd.print((now.hour()+add), DEC);
    lcd.print(':');
    if (now.minute() < 10)
      lcd.print('0');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second() < 10)
      lcd.print('0');
    lcd.print(now.second(), DEC);
//    lcd.print(' ');
    /*if (now.hour() > 12){
      lcd.print("PM");
      }
      else{
      lcd.print("AM");
    */
    if (CheckDST() == 1) {
      lcd.print(" PDT");
    }
    else { lcd.print(" PST");
    }

    delay(1000);
  }
}
