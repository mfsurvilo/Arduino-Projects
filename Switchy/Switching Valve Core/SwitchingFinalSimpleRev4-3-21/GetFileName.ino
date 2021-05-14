void GetFileName(){
  Wire.begin(); //Important for RTClib.h
  rtc.begin();
  DateTime now = rtc.now();
  filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
  filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
  filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
  filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
  filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
  filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
  filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
  filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()

  return ;
}
