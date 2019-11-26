/// \file Task.cpp
/// \brief Code for task functions.

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

#include "Includes.h"
#include "Defines.h"
#include "Input.h"
#include "Generator.h"

std::atomic_bool g_bFinished(false); ///< Search termination flag.
 
/// Get the board width and height, then perform the task.
/// \param t Tourney descriptor.
/// \param nNumThreads Number of concurrent threads.
/// \return true If the user opts to restart instead.

bool StartGenerateTask(const CTourneyDesc& t, int nNumThreads){
  unsigned n = 0;
  printf("Enter board width (even and >= 6).\n");
  bool bRestart = ReadUnsigned(n, Parity::Even, 6);

  if(!bRestart)
    CGenerator(n, n).Generate(t, nNumThreads); //perform the task

  return bRestart;
} //StartGenerateTask

/// Get the board width and height and the number of samples, 
/// then perform the task.
/// \param t Tourney descriptor.
/// \param nNumThreads Number of concurrent threads.
/// \return true If the user opts to restart instead.

bool StartMeasureTask(const CTourneyDesc& t, int nNumThreads){
  unsigned n = 0;
  printf("Enter board width (even and >= 6).\n");
  bool bRestart = ReadUnsigned(n, Parity::Even, 6);

  if(!bRestart){
    unsigned nSamples = 0;
    printf("Enter number of samples.\n");
    bRestart = ReadUnsigned(nSamples, Parity::DontCare, 1);

    if(!bRestart)
      CGenerator(n, n).Measure(t, nNumThreads, nSamples); //perform the task
  } //if

  return bRestart;
} //StartGenerateTask

/// Get the number of samples per board size and lower and upper
/// bounds on the range of board sizes to time, then perform the task.
/// \param t Tourney descriptor.
/// \param nNumThreads Number of concurrent threads.
/// \return true If the user opts to restart instead.

bool StartTimeTask(const CTourneyDesc& t, int nNumThreads){
  unsigned nSamples = 0;
  printf("Enter number of samples.\n");
  bool bRestart = ReadUnsigned(nSamples, Parity::DontCare, 1);

  if(!bRestart){
    unsigned lo = 0; //lower end of the range of sizes to be timed
    printf("Enter lowest board size in range.\n");
    bRestart = ReadUnsigned(lo, Parity::Even, 6);

    if(!bRestart){
      unsigned hi = 0; //upper end of the range of sizes to be timed
      printf("Enter highest board size in range.\n");
      bRestart = ReadUnsigned(hi, Parity::Even, 6);

      if(hi < lo) //in case of smart-aleck
        std::swap(lo, hi);

      if(!bRestart){ //perform task
        printf("This may take a while");

        for(unsigned n=lo; n<=hi; n+=2){
          CGenerator(n, n).Time(t, nNumThreads, nSamples); //generate
          putchar('.');
        } //for

        putchar('\n');
      } //if
    } //if
  } //if

  return bRestart;
} //StartTimeTask

/// Get task-appropriate parameters from the user and run the task.
/// \param task Task to be performed.
/// \param t Tourney descriptor.
/// \param nNumThreads Number of concurrent threads.
/// \return true If the user opts to restart instead.

bool StartTask(Task task, const CTourneyDesc& t, int nNumThreads){
  bool bRestart = false;
  g_bFinished = false;

  switch(task){
    case Task::Generate:
      bRestart = StartGenerateTask(t, nNumThreads);
      break;
              
    case Task::Measure:
      bRestart = StartMeasureTask(t, nNumThreads);
      break;
              
    case Task::Time: 
      bRestart = StartTimeTask(t, nNumThreads);
      break;
  } //switch

  return bRestart;
} //StartTask
