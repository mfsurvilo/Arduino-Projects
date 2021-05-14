//To Add

// yellow 10, green 11, blue 9, red 5V, purple GND, white A0

int muxpin_1 = A0;     //
int muxpin_2 = A1;
int addressA_1 = 11;   //Pins for multiplexer. These correspond to A,B,C on the multiplexer
int addressB_1 = 10;
int addressC_1 = 9;

int A_1 = 0;      //Address pin A
int B_1 = 0;      //Address pin B
int C_1 = 0;      //Address pin C
float val = 0;
float D[8][10]; //Temporary data storage matrix
int Add_1 = 0;
int Add_2 = 0;
int Avg_1 = 0;
int Avg_2 = 0;
void setup() {
  Serial.begin(9600);
  // Prepare address pins for output
  pinMode(addressA_1, OUTPUT);
  pinMode(addressB_1, OUTPUT);
  pinMode(addressC_1, OUTPUT);
  pinMode(addressA_2, OUTPUT);
  pinMode(addressB_2, OUTPUT);
  pinMode(addressC_2, OUTPUT);
  // Prepare read pin 
  pinMode(muxpin_1, INPUT); //using A0 on MEGA for reading. This goes to COM OUT/IN on Multiplexer
  pinMode(muxpin_2, INPUT);
}

void loop() {
    

  
  //Select each pin and read value
  for (int m=0; m<10; m++){                //Grabs ten data points. 
    for(int i=0; i<totalChannels; i++){    //Each channel is one analog input.
      A_1 = bitRead(i,0); //Take first bit from binary value of i channel.
      B_1 = bitRead(i,1); //Take second bit from binary value of i channel.
      C_1 = bitRead(i,2); //Take third bit from value of i channel.
  
      //Write address to mux
      digitalWrite(addressA_1, A_1);
      digitalWrite(addressB_1, B_1);
      digitalWrite(addressC_1, C_1);
      digitalWrite(addressA_2, A_1);
      digitalWrite(addressB_2, B_1);
      digitalWrite(addressC_2, C_1;
  
      //Read and print value
      Serial.print("Channel ");
      Serial.print(i);
      Serial.print(" value: ");
      val = analogRead(A0);
      D[i][m] = val/1024*5;      // Convert to a voltage and store in the temp data matrix
      Serial.println(D[i][m]);
    } 
    delay(500);
  }
  
 
      
}
