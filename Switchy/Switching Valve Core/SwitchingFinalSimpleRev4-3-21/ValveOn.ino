void ValveOn(int j){  
int R1 = 2;
int R2 = 3;
int R3 = 4;
int R4 = 5;


    
    if (j==1){ 
      digitalWrite(R1, LOW);
      //delay(1000);
      digitalWrite(R2, LOW);
      //delay(1000);
      digitalWrite(R3, LOW);
    }
  
    if (j==2) {  
      digitalWrite(R1, LOW);
      //delay(1000);
      digitalWrite(R2, LOW);
      //delay(1000);
      digitalWrite(R3, HIGH);
    }
    
    if(j==3) {   //For valve number 3 to be turned on only Relay 1 needs to be turned down. 
      digitalWrite(R1, LOW);
      //delay(1000);
      digitalWrite(R3, LOW);
      //delay(1000);
      digitalWrite(R2, HIGH);
    }
    
    if(j==4) {  // In order for valve four to be opened two realys need to be turned down, 1 and 3
      digitalWrite(R3, LOW);
    //  delay(1000);
      digitalWrite(R2, HIGH);
     // delay(1000);
      digitalWrite(R1, HIGH);
    }
  
}
