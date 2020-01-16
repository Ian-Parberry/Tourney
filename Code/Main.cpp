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
#include "Helpers.h"

/// \brief Main.
///
/// \return 0 (what could possibly go wrong?)

int main(){
  const int nNumThreads = 
    std::thread::hardware_concurrency() - 1; //number of threads
 
  srand(timeGetTime()); //we'll use ::rand() later to seed a better PRNG

  //print banner

  printf("Ian Parberry's square tourney generator");
  printf(" with %d concurrent threads.\n", nNumThreads);
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

        if((gentype == GeneratorType::ConcentricBraid ||
            gentype == GeneratorType::FourCover) &&
          cycletype == CycleType::Tour)
        {
          printf("Substituting joined tourney for knight's tour.\n");
          cycletype = CycleType::TourFromTourney;
        } //if

        if(!bRestart){
          bool obfuscate = false; //obfuscate flag
          bool bRestart = ReadObfuscate(obfuscate); //get the obfuscate flag
          
          if(!bRestart) //start the task
            StartTask(task, CTourneyDesc(gentype, cycletype, obfuscate), nNumThreads);       
        } //if
      } //if
    } //while
  } //while

  return 0; //what could possibly go wrong?
} //main