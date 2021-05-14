void LCDPan(const String& mystring)
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
