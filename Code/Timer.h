/// \file Timer.h
/// \brief Header for the timer CTimer.

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

#ifndef __Timer__
#define __Timer__

#include "Includes.h"
#include "Defines.h"

typedef std::chrono::system_clock sysclock; ///< System clock.
typedef std::chrono::time_point<sysclock> systime_point; ///< System time point.

/// \brief Timer for elapsed time and CPU time.
///
/// The timer is charged with everything that is time-related, in particular,
/// elapsed time, CPU time, and clock time. The member functions return time
/// intervals measured from the time Start() is called to the current time.
/// The CPU time reported is the total summed over all threads. Uses std::chrono
/// to do the heavy lifting. 

class CTimer{
  private:
    systime_point m_tpElapsedTimeStart; ///< Clock time.
    float m_fCPUTimeStart; ///< CPU time in seconds.
    std::string m_strTimeAndDate; ///< Container for time and date string.

    const char*  GetDateAndTime(const systime_point p); ///< Get time and date.
    UINT CPUTime(); ///< Platform independent CPU time.

  public:  
    const char* GetStartDateAndTime(); ///< Get start time and date.
    const char* GetCurrentDateAndTime(); ///< Get current time and date.

    void Start(); ///< Start timing.
    float GetElapsedTime(); ///< Get elapsed time in seconds.
    float GetCPUTime(); ///< Get CPU time in seconds.
    void Finish(); ///< Print CPU and elapsed time.
}; //CTimer

#endif