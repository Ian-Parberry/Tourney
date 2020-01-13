/// \file Input.cpp
/// \brief Code for input functions.

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

#include "Input.h"
#include "Includes.h"

/// This function will return only if the user types an even
/// unsigned integer or the letter 'q' to quit.
/// \param n [out] The unsigned integer typed by the user.
/// \param parity The parity of the unsigned integer to be read.
/// \param lo Lower bound for the unsigned integer to be read.
/// \return true If the user wants to quit instead.

bool ReadUnsigned(UINT& n, Parity parity, UINT lo){
  char b[256]; //buffer for input line
  bool ferror; //format error
  bool nerror; //numerical error

  do{ //prompt user and get input
    ferror = false; //no format error yet
    nerror = false; //no numerical error yet

    printf("> "); //prompt user
    const char* result = fgets(b, sizeof(b), stdin); //read a line of text

    if(result != nullptr && b[0] == 'r' || b[0] == 'R')
      return true; //'r' means restart
    
    else{
      const int args = sscanf(b, "%u\n", &n); //parse input
      ferror = args != 1; //format error if number of arguments is not 1
      const bool perror = parity == Parity::Even && (n & 1) == 1 ||
        parity == Parity::Odd && (n & 1) == 0;
      nerror = perror || n < lo; //n has the wrong parity or is too small

      if(ferror) //format error
        printf("**** Error: Invalid input. Please try again.\n");

      else if(nerror) //numerical error
        printf("**** Error: Number unsuitable. Please try again.\n");
    } //else
  }while(ferror || nerror); //try again until we get an acceptable input
  
  return false; //got a valid input, don't quit
} //ReadUnsigned

/// \brief Read a single character from stdin.
///
/// This function will return only when the user types a
/// character from a particular set.
/// \param s Set of allowable characters.
/// \return The allowable character that the user typed (eventually).

char ReadCharacter(std::set<char>& s){
  char b[256]; //buffer for input line
  bool ferror; //format error
  bool cerror; //character error
  char c; //result

  do{ //prompt user and get input
    ferror = false; //no format error yet
    cerror = false; //no numerical error yet

    printf("> "); //prompt user
    const char* result = fgets(b, sizeof(b), stdin); //read a line of text

    if(result != nullptr){
      const int args = sscanf(b, "%c\n", &c); //parse input
      ferror = args != 1; //format error if number of arguments is not 1
      cerror = s.find(c) == s.end(); 

      if(ferror) //format error
        printf("**** Error: Invalid input. Please try again.\n");

      else if(cerror) //character error
        printf("**** Error: Invalid character. Please try again.\n");
    } //if
  }while(ferror || cerror); //try again until we get an acceptable input

  if('A' <= c && c <= 'Z') //upper case
    c += 'a' - 'A'; //remap to lower case

  return c;
} //ReadCharacter

/// Print keyboard mapping for generator.

void PrintGeneratorTypeHelp(){
  printf("   w: random walk with Warnsdorff's heuristic\n");
  printf("   t: Takefuji-Lee neural network\n");
  printf("   d: divide-and-conquer\n");
  printf("   c: concentric braid\n");
  printf("   4: 4-cover\n");
} //PrintGeneratorTypeHelp

/// Read a character from stdin and decode it into a generator type.
/// \param t [out] The type of generator that the user wants.
/// \return true If the user wants to restart instead.

bool ReadGeneratorType(GeneratorType& t){
  bool finished = false;

  while(!finished){
    printf("Enter generation algorithm [wtdc4], h for help, r to restart.\n");
    finished = true;

    std::set<char> s = {'w', 't', 'd', 'c', 'h', '4', 'r'}; //admissible chars
    const char c = ReadCharacter(s); //read admissible character from user
    
    //decode character entered by user into a generator type and print response
    
    t = GeneratorType::Unknown;

    switch(c){
      case 'w': 
        t = GeneratorType::Warnsdorff; 
        printf("Warnsdorff's algorithm selected.\n");
        break;

      case 't': 
        t = GeneratorType::TakefujiLee; 
        printf("Takefuji-Lee neural network algorithm selected.\n");
        break;

      case 'd':
        t = GeneratorType::DivideAndConquer;
        printf("Divide-and-conquer algorithm selected.\n");
        break;

      case 'c': 
        t = GeneratorType::ConcentricBraid; 
        printf("Concentric braid algorithm selected.\n");
        break;

      case '4': 
        t = GeneratorType::FourCover; 
        printf("Four-cover algorithm selected.\n");
        break;

      case 'h': 
        PrintGeneratorTypeHelp();
        finished = false;
        break;

      case 'r': return true; //restart
    } //switch
  } //if

  return false; //don't restart
} //ReadGeneratorType

/// Print keyboard mapping for cycle type.

void PrintCycleTypeHelp(){
  printf("   t: knight's tour\n");
  printf("   y: tourney\n");
  printf("   j: joined tourney\n");
} //PrintCycleTypeHelp

/// Read a character from stdin and decode it into a cycle type.
/// \param t [out] The type of tour that the user wants.
/// \return true If the user wants to restart instead.

bool ReadCycleType(CycleType& t){
  bool finished = false;

  while(!finished){
    printf("Enter tourney type [tyj], h for help, r to restart.\n");
    finished = true;

    std::set<char> s = {'t', 'y', 'j', 'h', 'r'}; //admissible characters
    const char c = ReadCharacter(s); //read admissible character from user

    //decode character entered by user into a cycle type and print response
  
    t = CycleType::Unknown;

    switch(c){
      case 't': 
        t = CycleType::Tour; 
        printf("Knight's tour selected.\n");
        break;

      case 'y':
        t = CycleType::Tourney;
        printf("Tourney selected.\n");
        break;

      case 'j':
        t = CycleType::TourFromTourney; 
        printf("Joined tourney selected.\n");
        break;

      case 'h': 
        PrintCycleTypeHelp(); 
        finished = false; 
        break;

      case 'r': return true; //restart
    } //switch
  } //if

  return false; //don't restart
} //ReadCycleType

/// Read a character from stdin and decode it into blur status.
/// \param blurred [out] true if the user requests blurring.
/// \return true If the user wants to restart instead.

bool ReadBlur(bool& blurred){
  printf("Blurred [yn], r to restart?\n");

  std::set<char> s = {'y', 'n', 'r'}; //admissible characters
  const char c = ReadCharacter(s); //read admissible character from user
  blurred = c == 'y'; //yes to blur

  const bool bRestart = c == 'r'; //r to restart

  if(bRestart){
    if(blurred)
      printf("Tourney(s) will be blurred.\n");
    else
      printf("Tourney(s) will not be blurred.\n");
  } //if

  return bRestart; 
} //ReadBlur

/// Print keyboard mapping for tasks.

void PrintTaskHelp(){
  printf("   g: generate and save a single tourney\n");
  printf("   m: measure statistics on many tourneys of the same size\n");
  printf("   t: time the generation of many tourneys for a size range\n");
} //PrintTaskHelp

/// Read a character from stdin and decode it into a task.
/// \param t [out] The task that the user wants.
/// \return true If the user wants to quit instead.

bool ReadTask(Task& t){ 
  bool finished = false;

  while(!finished){
    printf("Select task [gmt], h for help, q to quit.\n");
    finished = true;

    std::set<char> s = {'g', 'm', 't', 'h', 'q'}; //admissible characters
    const char cTask = ReadCharacter(s); //read admissible character from user

    //decode character entered by user into a task and print response

    t = Task::Unknown;

    switch(cTask){
      case 'g': 
        t = Task::Generate; 
        printf("Generating a single tourney.\n");
        break;

      case 'm':
        t = Task::Measure;
        printf("Measuring single and double move statistics.\n");
        break;

      case 't':
        t = Task::Time;
        printf("Timing the generation of tourneys.\n");
        break;

      case 'h': 
        PrintTaskHelp(); 
        finished = false; 
        break;

      case 'q': return true; //quit
    } //switch
  } //while

  return false; //don't quit
} //ReadTask