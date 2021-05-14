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
