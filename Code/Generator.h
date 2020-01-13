/// \file Generator.h
/// \brief Header for the tourney and knight's tour generator CGenerator.

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

#ifndef __Generator__
#define __Generator__

#include "ThreadSafeQueue.h"

#include "Structs.h"
#include "SearchThreadQueues.h"
#include "SearchThread.h"
#include "Includes.h"

#include "Defines.h"

/// \brief Knight's tour and tourney generator.
///
/// The generator constructs tourneys and knight's tours.
/// Those that use randomized algorithms are multi-threaded.
/// The resulting tourneys and knight's tours can either be output
/// (single tourneys and knight's tours only), or the generation
/// of large numbers of them can be timed, or various
/// statistics can be measured.

class CGenerator: public CSearchThreadQueues{
  private:
    std::vector<std::thread> m_vecThreadList; ///< Thread list for search.
    
    int m_nWidth = 0; ///< Board width.
    int m_nHeight = 0; ///< Board height.
    int m_nSize = 0; ///< Board size.
   
    void OutputStat(FILE* output, double a[8]); ///< Output a statistic.
    void OutputTimes(FILE* output, float fCpu, float fElapsed); ///< Output times.

  public:
    CGenerator(int w, int h); ///< Constructor.
    CGenerator(int n); ///< Constructor.
    CGenerator(); ///< Default constructor.
    
    void Generate(const CTourneyDesc& t, int nThreads); ///< Generate.
    void Measure(const CTourneyDesc& t, int nThreads, int n); ///< Measure.
    void Time(const CTourneyDesc& t, int nThreads, int n); ///< Time.
}; //CGenerator

#endif
