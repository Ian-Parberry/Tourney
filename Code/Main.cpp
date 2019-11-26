/// \file Main.cpp
/// \brief Main() and a couple of useful functions.

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

#ifdef _DEBUG
  #include <vld.h> //Visual Leak Detector from http://vld.codeplex.com/
#endif

#include "Includes.h"
#include "Defines.h"
#include "Input.h"
#include "Generator.h"
#include "Task.h"

#if !defined(_MSC_VER) //*nix 
  #include <sys/time.h>

  /// \brief *nix equivalent of the Windows function timeGetTime().
  ///
  /// Well, it kinda-sorta is. It will be used to seed a PRNG, so all that
  /// really matters is that it returns something different each time this
  /// program is run. It may be slow but we'll only used it a few times
  /// during initialization.
  /// \return Number of milliseconds since the start of the current epoch.

  unsigned int timeGetTime(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec/1000;
  } //timeGetTime
#endif

/// \brief Main.
///
/// \return 0 (what could possibly go wrong?)

int main(){
  const int nNumThreads = std::thread::hardware_concurrency() - 1; //number of threads
 
  srand(timeGetTime()); //we'll use ::rand() later to seed a better PRNG

  //print banner

  printf("Ian Parberry's square tourney generator");
  printf(" (%d concurrent threads).\n", nNumThreads);
  printf("-------------------------------------------------------------------");
  putchar('\n');

  //read user input and perform corresponding tasks

  bool bQuit = false; //true when the user wants to exit
  
  while(!bQuit){ //loop until the user types "q" to quit
    putchar('\n');

    Task task = Task::Unknown; //task to be performed
    bQuit = ReadTask(task); //get the task

    if(!bQuit){
      GeneratorType gentype = GeneratorType::Unknown; //generator type
      bool bRestart = ReadGeneratorType(gentype); //get the generator type

      if(!bRestart){
        CycleType cycletype = CycleType::Unknown; //cycle type
        bRestart = ReadCycleType(cycletype); //get the cycle type

        if(!bRestart){
          bool blur = false; //blur flag
          bool bRestart = ReadBlur(blur); //get the blur flag
          
          if(!bRestart) //start the task
            StartTask(task, CTourneyDesc(gentype, cycletype, blur), nNumThreads);       
        } //if
      } //if
    } //while
  } //while

  return 0; //what could possibly go wrong?
} //main