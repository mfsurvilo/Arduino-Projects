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
