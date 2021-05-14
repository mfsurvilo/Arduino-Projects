/*
  Conditionals - If statement

  This example demonstrates the use of if() statements.
  It reads the state of a potentiometer (an analog input) and turns on an LED
  only if the potentiometer goes above a certain threshold level. It prints the
  analog value regardless of the level.

  The circuit:
  - potentiometer
    Center pin of the potentiometer goes to analog pin 0.
    Side pins of the potentiometer go to +5V and ground.
  - LED connected from digital pin 13 to ground

  - Note: On most Arduino boards, there is already an LED on the board connected
    to pin 13, so you don't need any extra components for this example.

  created 17 Jan 2009
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/IfStatement
*/

// These constants won't change:

int buttonPin = 6;       // pin that the LED is attached to


void setup() {
  // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT);
  // initialize serial communications:
  Serial.begin(9600);
}

void loop() {


  // if the analog value is high enough, turn on the LED:
  if (!digitalRead(buttonPin)) {
    Serial.println("button was not pushed");
  } else {
    Serial.println("button was pushed");
  }

  // print the analog value:



  
  delay(10);        // delay in between reads for stability
}
