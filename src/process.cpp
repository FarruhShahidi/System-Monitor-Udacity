#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):  pid_(pid) {}


// Neeed to change Process::Pid() and Process::CpuUtilization() to const so
// the comparion operator runs.
int Process::Pid() const { return pid_; }


// following https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
// with slight adjustment
float Process::CpuUtilization() const 
{
  // calculate total time.
  float total_time = LinuxParser::ActiveJiffies(Process::Pid())/sysconf(_SC_CLK_TCK);
  // get the starttime, it is already divided by sysconf
  long starttime = LinuxParser::UpTime(Process::Pid());
  long seconds = LinuxParser::UpTime() - starttime;
  float cpu_usage = total_time/seconds;
  
  
  return 100 * cpu_usage;
}


string Process::Command() { return LinuxParser::Command(Process::Pid()); }


string Process::Ram()  { return LinuxParser::Ram(Process::Pid()); }

 
string Process::User() { return LinuxParser::User(Process::Pid()); }

 
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()); }

 
 
//sort processes by the most active
bool Process::operator<(Process const& a) const
{ 
  return this->CpuUtilization() > a.CpuUtilization() ; 
}
  


