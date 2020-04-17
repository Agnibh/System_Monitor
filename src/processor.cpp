#include "processor.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float Act = (float)LinuxParser::ActiveJiffies();
    float Tot = (float)LinuxParser::Jiffies();
    float CpuUtl = Act/Tot;
    return CpuUtl;}
