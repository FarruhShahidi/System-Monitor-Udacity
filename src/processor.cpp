

#include<iostream>
#include "processor.h"
#include "linux_parser.h"
 
float Processor::Utilization() 
{
  //(float) (LinuxParser::Jiffies() - LinuxParser::IdleJiffies())/(LinuxParser::Jiffies());
  float jiff = LinuxParser::Jiffies();
  float jiff_idle = LinuxParser::IdleJiffies();
  float jiff_delta = jiff - jiff_prev;
  float jiff_idle_delta = jiff_idle - jiff_idle_prev;
  float ans = (jiff_delta - jiff_idle_delta) / ( jiff_delta);
  jiff_prev = jiff;
  jiff_idle_prev = jiff_idle;
  
  
  //std::cout << ans << std::endl;
  return ans;
 }
