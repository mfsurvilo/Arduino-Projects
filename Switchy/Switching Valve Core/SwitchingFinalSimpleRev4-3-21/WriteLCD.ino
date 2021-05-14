/*LCD Pin setup from right to left. 
GND
Resistor for backlight to 5V
40
41
42
43
blank
blank
blank
blank
44
GND
45
Potentiometer for brightness
5V
Ground
*/

void WriteLCD(int i) {       
  lcd.begin(16,2);      
  lcd.setCursor(0,0);   // Where do you want to start writing words?

  lcd.print("VALVE ");
  lcd.print(i);
      
  lcd.print(" ON");
}
