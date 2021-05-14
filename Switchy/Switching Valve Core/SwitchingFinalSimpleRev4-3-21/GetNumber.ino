unsigned long GetNumber() {
   int num = 0;
   char key = keypad.getKey();


   while(key != '#')
   {   
      switch (key)
      {
         case NO_KEY:
            break;
         
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            lcd.clear(); 
            Serial.println(key);        
            num = num * 10 + (key - '0');
            lcd.clear(); 
            lcd.print(num);   
            break;

         case '*':
            
            num = 0;
            lcd.clear(); 
            lcd.print("                ");
            ;         
      }
      key = keypad.getKey();
   }
   return num;
}
