#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_;}
//return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float act = (float)LinuxParser::ActiveJiffies(Pid());
    act = act/sysconf(_SC_CLK_TCK);
    float pupT = (float)LinuxParser::UpTime(Pid());
    float supT = (float)LinuxParser::UpTime();
    float per = act/(supT-pupT);
    cpu_=per;
    return per;}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid());}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    long mem = 0;
    try{
        mem = std::stol(LinuxParser::Ram(Pid()))/1000;
    }
    catch(...){
        mem = 0;
    }
    
    return std::to_string(mem);}


// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid());}


// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid());}


// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    if(this->cpu_<a.cpu_)
    return false;
    else
    return true; }