#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
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
float LinuxParser::MemoryUtilization() { 
  float MemTot, MemFree, MemUtl;
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value){
        if(key=="MemTotal:")
        MemTot = std::stof(value);
        if(key=="MemFree:")
        MemFree = std::stof(value);
      }
    }
  }
  MemUtl = (MemTot-MemFree)/MemTot;
  return MemUtl; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long UpTime;
  std::string line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream>>UpTime;
  }
  return UpTime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long tot = LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies();
  return tot; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::string value,line;
  long utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    for(int i=1;i<14;i++){
      linestream>>value;
    }
    linestream>>utime>>stime>>cutime>>cstime;
  }  
  return utime+stime+cutime+cstime; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> CpuUtl = LinuxParser::CpuUtilization();
  long Act = std::stol(CpuUtl[0])+std::stol(CpuUtl[1])+std::stol(CpuUtl[2])+std::stol(CpuUtl[5])
             +std::stol(CpuUtl[6])+std::stol(CpuUtl[7]);
  return Act; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> CpuUtl = LinuxParser::CpuUtilization();
  long idle = std::stol(CpuUtl[3])+std::stol(CpuUtl[4]);
  return idle; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> CpuUtl;
  std::string line, key;
  std::string a,b,c,d,e,f,g,h,i,j;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>a>>b>>c>>d>>e>>f>>g>>h>>i>>j){
        if(key=="cpu"){
            CpuUtl.push_back(a);CpuUtl.push_back(b);
            CpuUtl.push_back(c);CpuUtl.push_back(d);
            CpuUtl.push_back(e);CpuUtl.push_back(f);
            CpuUtl.push_back(g);CpuUtl.push_back(h);
            CpuUtl.push_back(i);CpuUtl.push_back(j);
          }
        }
      }
    }
   return CpuUtl; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int TProcess;
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value){
        if(key=="processes")
        TProcess = std::stoi(value);

      }
    }
  }
  return TProcess; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int RProcess;
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value){
        if(key=="procs_running")
        RProcess = std::stoi(value);

      }
    }
  }  
  return RProcess; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string line, key, value, mem;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
  }  
  return line; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string line, key, value, mem;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value){
        if(key=="VmSize:")
        mem = value;

      }
    }
  }
  return mem; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string line, key, value, uid;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value){
        if(key=="Uid:")
        uid = value;

      }
    }
  } 
  return uid; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user;
  string key, x;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == LinuxParser::Uid(pid)) {
          user = key;
        }
      }
    }
  }
  return user; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string value,line;
  long uptime;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    for(int i=1;i<22;i++){
      linestream>>value;
    }
    linestream>>uptime;
    uptime = uptime/sysconf(_SC_CLK_TCK);
  }    
  return uptime; }