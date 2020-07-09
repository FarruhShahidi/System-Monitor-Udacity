#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
using std::stoi;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
	string line, value, key;
    float total_mem = 0.0;
  	float free_mem = 0.0;
  	float ans = 0;
  	std::ifstream stream_mem(kOSPath + kMeminfoFilename);
  	if (stream_mem.is_open()){
    	while (std::getline(stream_mem, line)){
        	std::istringstream linestream(line);
          	while (linestream >> key >> value){
            	if (key == "MemTotal:")
                  	total_mem = stof(value);
              	if (key == "MemFree:"){
                  	free_mem = stof(value);
                  	break;
                }
            }

        }
     ans = (total_mem - free_mem) / total_mem;
     return (float)ans;
    }

 return (float)ans;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  string line;
  string first, second;
  std::ifstream stream_uptime(kOSPath + kUptimeFilename);
  if(stream_uptime.is_open()){
  	std::getline(stream_uptime, line);
    std::istringstream linestream(line);
    linestream >> first >> second;
  }
  return stol(first);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()

{
  vector<string> cpu_util {};
  string line, key;
  string user, nice ,system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream_cpu(kOSPath + kStatFilename);
  if (stream_cpu.is_open()){
  	std::getline(stream_cpu, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    cpu_util.push_back(user);
    cpu_util.push_back(nice);
    cpu_util.push_back(system);
    cpu_util.push_back(idle);
    cpu_util.push_back(iowait);
    cpu_util.push_back(irq);
    cpu_util.push_back(softirq);
    cpu_util.push_back(steal);
    cpu_util.push_back(guest);
    cpu_util.push_back(guest_nice);
    return cpu_util;
  }
  return {};

}
/*
Once CpuUtilization() is determined, we can extract
the number of idle,active and total jiffies according to
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
*/
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  long ans = 0;
  vector<string> cpu_util = CpuUtilization();
  if (cpu_util.size() > 0){
  	ans += stol(cpu_util[0]);
  	ans += stol(cpu_util[1]);
  	ans += stol(cpu_util[2]);
  	ans += stol(cpu_util[5]);
  	ans += stol(cpu_util[6]);
  	ans += stol(cpu_util[7]);
  	return ans;}
  return ans;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long ans = 0;
  vector<string> cpu_util = CpuUtilization();
  if (cpu_util.size() > 0){
  ans += stol(cpu_util[3]);
  ans += stol(cpu_util[4]);
  return ans;
  }
  return 0; }


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  return ActiveJiffies() + IdleJiffies();
}



// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  string line, word;
  long total = 0;

  std::ifstream stream_pidjiff(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream_pidjiff.is_open()){
    // I should be able to find the numbers from the first line
    std::getline(stream_pidjiff, line);
    std::istringstream linestream(line);
    //skip the first 13 words
    for (int i = 1; i < 14; i++){
    	linestream >> word;
    }
    for (int i = 1; i < 5; i++){
    	linestream >> word;
      	total += stol(word);
    }
    return total;


  }
  return 0;
}




/*TotalProcesses and RunningProcesses should be easy*/
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line;
  string key,value;
  std::ifstream stream_processes(kProcDirectory + kStatFilename);
  if (stream_processes.is_open()){
    while(std::getline(stream_processes, line)){
   		std::istringstream linestream(line);
      	while(linestream >> key >> value){
          if (key == "processes"){
          	return stoi(value);
          }
        }
    }

  }
  return 0;

}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
  {
  string line;
  string key,value;
  std::ifstream stream_rprocesses(kProcDirectory + kStatFilename);
  if (stream_rprocesses.is_open()){
    while(std::getline(stream_rprocesses, line)){
   		std::istringstream linestream(line);
      	while(linestream >> key >> value){
          if (key == "procs_running"){
          	return stoi(value);
          }
        }
    }

  }
  return 0;
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string line;
  std::ifstream stream_cmd(kProcDirectory + to_string(pid) +  kCmdlineFilename);
  if (stream_cmd.is_open())
  {
  	std::getline(stream_cmd, line);
    return line;
  }
  return "x";

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// cat /proc/pid/status, then VmSize:
string LinuxParser::Ram(int pid)
{
  string line;
  string key,value;
  std::ifstream stream_ram(kProcDirectory + to_string(pid) + kStatusFilename );
  if (stream_ram.is_open()){
  	while(std::getline(stream_ram, line)){
    	std::istringstream linestream(line);
      	while(linestream >> key >> value){
        if (key == "VmSize:"){
          // converting to megabytes
          long val = stol(value);
          val /= 1000;
          return to_string(val); }
        }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)

{
  string line;
  string key,value;
  std::ifstream stream_uid(kProcDirectory + to_string(pid) + kStatusFilename );
  if (stream_uid.is_open()){
  	while(std::getline(stream_uid, line)){
    	std::istringstream linestream(line);
      	while(linestream >> key >> value){
        if (key == "Uid:"){
        	return value; }
        }
    }
  }

  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string uid = LinuxParser::Uid(pid);
  string line, id, value, temp;
  std::ifstream stream_user(kProcDirectory + to_string(pid) + kStatusFilename );
  if (stream_user.is_open())
  {
    while (std::getline(stream_user, line))
    {
    	std::replace(line.begin(), line.end(), ':', ' ');
      	std::istringstream linestream(line);
      	while (linestream >> value >> temp >> id)
        {
        	if(id == uid)
            {
            	return value;
            }
        }
    }

  }

  return "Ohh";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{

  long uptime;
  string line, time;
  std::ifstream stream_piduptime(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream_piduptime.is_open())
  {
  	while (std::getline(stream_piduptime, line))
    {
    	std::istringstream linestream(line);
      	for (int i = 1; i < 22; i++)
        {
        	linestream >> time;
        }
      	linestream >> time;
      	uptime = stol(time) /  sysconf(_SC_CLK_TCK);
      	return uptime;
    }
  }


  return 0;
}
