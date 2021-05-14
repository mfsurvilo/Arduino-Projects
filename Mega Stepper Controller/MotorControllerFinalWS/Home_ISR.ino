// Sets a new buttonstate if the Home button is tripped.
  void Home_ISR() {
    buttonState = digitalRead(buttonPin);
    if (valve == 1) {
      buttonState = 1;
    }
  }
