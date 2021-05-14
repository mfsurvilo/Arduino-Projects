// This prints input signal statuses into the serial monitor. 
  void PrintStatus() {
    Serial.print ("Home Pin (2) is ");
    Serial.println(digitalRead(buttonPin));
    Serial.print("Pin V1 (A0) is ");
    Serial.println(digitalRead(V1Pin));
    Serial.print("Pin V2 (A1) is ");
    Serial.println(digitalRead(V2Pin));
    Serial.print("Pin V3 (A2) is ");
    Serial.println(digitalRead(V3Pin));
}
