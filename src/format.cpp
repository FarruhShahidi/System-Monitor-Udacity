#include <string>

#include "format.h"

using std::string;
using std::to_string;

 
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
 
string Format::ElapsedTime(long seconds) {
  int hours;
  int minutes;
  string ans;
  hours = seconds / 3600;
  seconds = seconds % 3600;
  minutes = seconds / 60;
  seconds = seconds %60;
  ans = to_string(hours) + ':' + to_string(minutes) + ':' + to_string(seconds);
  return ans; }
