int CheckDST() {

  int DST;
  int MarchDay;
  int NovemberDay;

  // Is it daylight savings time?
  int DateMatrix[12][3] = {
    //year, date in march that DST starts, date in november when DST ends.
    {2021, 14, 7},
    {2022, 13, 6},
    {2023, 12, 5},
    {2024, 10, 3},
    {2025, 9,  2},
    {2026, 8, 1},
    {2027, 14, 7},
    {2028, 12, 5},
    {2029, 11, 4},
    {2030, 10, 3},
    {2031, 9, 2},
    {2032, 14, 7}
  };

  DateTime now = rtc.now();
  int Month = now.month();

  if ((Month <= 11) && (Month >= 3)) { // Is it a month March through November??
    for (int ai = 0; ai <= 11; ai++) {
      if (now.year() == DateMatrix[ai][0]) {
         MarchDay = DateMatrix[ai][1];
         NovemberDay = DateMatrix[ai][2];
      }
    }

    if (Month == 3) {
      if (now.day() >= MarchDay) { // Check what day in March
        if (now.hour() >= 2) {
          DST = 1;
        }
        else {
          DST = 0;
        }
      }
    }
    else if (Month == 11) {
      if (now.day() <= NovemberDay) { // Check what day in November
        if (now.hour() <= 2) {
          DST = 1;
        }
        else {
          DST = 0;
        }
      }
    }
    else {
      DST = 1;
    }
  }

  else {
    DST = 0;
  }
  return DST;

}
