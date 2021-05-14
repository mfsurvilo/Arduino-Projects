// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday",
                "Wednesday", "Thursday", "Friday", "Saturday"
                            };

void setup () {
  while (!Serial);
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
//////////////////////////////////////////////////////////////////////////////////////
/// Enter Time IN PST even if it is daylight savings time. PST is UTC-8hrs!! /////////
//////////////////////////////////////////////////////////////////////////////////////

  // This line sets the RTC with an explicit date & time, for example to set
  //  Enter the time at which you will send the code. After uploading this code immediately comment out the following 7 lines and reupload.
  // This will keep the RTC from resetting the time upon restart. 

  /*
  int years = 2021;
  int months = 4;
  int days = 9;
  int hours = 1;
  int minutes = 24;
  int seconds = 35;
  rtc.adjust(DateTime(years, months, days, hours, minutes , (seconds + 15)));
 */

}
// the plus 20 above refers to the time it takes to compile and uplad the code
void loop () {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print("  PST (UTC-8hrs)");
  Serial.println();
  delay(100);
}
