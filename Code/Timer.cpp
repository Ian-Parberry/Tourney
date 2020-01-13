/// \file Timer.cpp
/// \brief Code for the timer CTimer.

// MIT License
//
// Copyright (c) 2019 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#define _CRT_SECURE_NO_WARNINGS ///< Make Visual Studio shut up about ctime().

#include "Timer.h"

/// Start timing by saving the current elapsed and CPU times.

void CTimer::Start(){
  m_tpElapsedTimeStart = sysclock::now(); //time_point
  m_fCPUTimeStart = CPUTime()/1000.0f;
} //Start

/// Get time and date string into m_strTimeAndDate from a systime_point and
/// return it as a null-terminated string. We're using a member variable to
/// store an std::string so that its c_str() pointer is persistent.
/// \param p An instance of systime_point.
/// \return The corresponding time and date string.

const char*  CTimer::GetDateAndTime(const systime_point p){
  const time_t t = sysclock::to_time_t(p);
  m_strTimeAndDate = ctime(&t);
  return m_strTimeAndDate.c_str();
} //GetStartDateAndTime

/// Get the start date and time as a null-terminated string. Uses 
/// GetDateAndTime() to do the heavy lifting.
/// \return Pointer to first character in a null-terminated string.

const char* CTimer::GetStartDateAndTime(){
  return GetDateAndTime(m_tpElapsedTimeStart);
} //GetStartDateAndTime

/// Get the current date and time as a null-terminated string. Uses 
/// GetDateAndTime() to do the heavy lifting.
/// \return Pointer to first character in a null-terminated string.

const char* CTimer::GetCurrentDateAndTime(){
  return GetDateAndTime(sysclock::now());
} //GetCurrentDateAndTime

/// Get the amount of time elapsed since the timer was started.
/// \return Elapsed time in seconds.

float CTimer::GetElapsedTime(){
  std::chrono::duration<float> dt = sysclock::now() - m_tpElapsedTimeStart;
  return dt.count();
} //GetElapsedTime

/// Get the amount of CPU time used since the timer was started.
/// \return CPU time in seconds.

float CTimer::GetCPUTime(){
  return CPUTime()/1000.0f - m_fCPUTimeStart;
} //GetCPUTime

/// Print the elapsed and CPU time in seconds since the start of
/// the search to stdout.

void CTimer::Finish(){
  printf("Finished at: %s", GetCurrentDateAndTime());
  printf("Elapsed time %0.1f sec, CPU time %0.1f sec\n", 
    GetElapsedTime(), GetCPUTime());
} //Finish 

/// Get CPU time for this process in seconds.
/// It's annoying that std::chron has no cross-platform support for this.
/// \return User CPU time in seconds.

#if defined(_MSC_VER) //Windows and Visual Studio 

  UINT CTimer::CPUTime(){ 
    UINT64 llCNS = 0; //for CPU time in centinanoseconds
    const DWORD pid = GetCurrentProcessId(); //process id
    HANDLE hp = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid); //proc handle

    if(hp != nullptr){ //handle is valid
      FILETIME ft0, ft1, ft2; //unused
      FILETIME ftUser; //this one we want 

      if(GetProcessTimes(hp, &ft0, &ft1, &ft2, &ftUser))
        llCNS = *((UINT64*)&ftUser);  //get time values
    
      CloseHandle(hp); //close the process handle
    } //if

    return UINT(llCNS/10000ULL); //round to milliseconds
  } //CPUTime

#else //*nix and g++

  #include <time.h>

  UINT CTimer::CPUTime(){ //*nix version
    return UINT(1000.0*(double)(clock())/CLOCKS_PER_SEC);
  } //UINT

#endif

