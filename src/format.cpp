#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hr = seconds/3600;
    int min = (seconds - hr*3600)/60;
    int sec = seconds-hr*3600-min*60;
    string date = std::to_string(hr)+":"+std::to_string(min)+":"+std::to_string(sec);
    return date; }