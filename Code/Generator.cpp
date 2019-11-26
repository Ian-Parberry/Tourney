/// \file Generator.cpp
/// \brief Code for the tourney and knight's tour generator CGenerator.

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

#include "Generator.h"
#include "Random.h"
#include "Defines.h"
#include "Timer.h"
#include "Board.h"
#include "DivideAndConquer.h"
#include "ConcentricBraid.h"

/// Create a very empty chessboard.

CGenerator::CGenerator(){ 
} //constructor

/// Create an empty rectangular chessboard.
/// \param w Board width.
/// \param h Board width.

CGenerator::CGenerator(int w, int h):
  m_nWidth(w), m_nHeight(h), m_nSize(w*h){ 
} //constructor

/// Create an empty square chessboard.
/// \param n Width and height of a square chessboard.

CGenerator::CGenerator(int n): CGenerator(n, n){ 
} //constructor

///////////////////////////////////////////////////////////////////
// Code for Task::Generate

#pragma region Task::Generate

/// Generate a single knight's tour or tourney.
/// Fill the request queue, launch the search threads, then
/// wait for them to terminate and output the resulting
/// tour or tourney to a file.
/// \param t Tourney descriptor.
/// \param nThreads Number of search threads to use.

void CGenerator::Generate(const CTourneyDesc& t, int nThreads){ 
  const GeneratorType gentype = t.m_eGenerator;
  const CycleType cycletype = t.m_eCycle;

  if(gentype == GeneratorType::DivideAndConquer){ //no search necessary
    CBoard b(m_nWidth, m_nHeight); //board for the tour
    CDivideAndConquer().Generate(b, cycletype); //generate it
    if(t.m_bBlur)b.Blur(); //blur if necessary

    std::string s = MakeFileNameBase(t, b.GetWidth()); //file name

    b.Save(s); //save to text file
    b.SaveToSVG(s); //save to SVG file
  } //if

  else if(gentype == GeneratorType::ConcentricBraid){ //no search necessary
    CBoard b(m_nWidth, m_nHeight); //board for the tour
    CConcentricBraid().Generate(b); //generate it
    if(cycletype == CycleType::TourFromTourney)b.Join(); //make tour if required
    if(t.m_bBlur)b.Blur(); //blur if necessary

    std::string s = MakeFileNameBase(t, b.GetWidth()); //save file name
    b.Save(s); //save to text file
    b.SaveToSVG(s); //save to SVG file
  } //if

  else{ //need multithreaded search
    for(int i=0; i<nThreads; i++) //queue up search requests
      m_cSearchRequest.push(CSearchRequest(t, m_nWidth, m_nHeight));  
  
    for(int i=0; i<nThreads; i++) //launch the search threads
      m_vecThreadList.push_back(std::thread((CSearchThread())));

    //start timing CPU and elapsed time

    CTimer Timer;
    Timer.Start();
    printf("Starting %d theads at: %s", nThreads, Timer.GetCurrentDateAndTime());

    //wait for all search threads to terminate
  
    std::for_each(m_vecThreadList.begin(), m_vecThreadList.end(), 
      std::mem_fn(&std::thread::join));

    Timer.Finish(); //stop the timer
    m_vecThreadList.clear(); //clear the thread list for next use

    //process results of search

    CSearchResult result; //current search result

    const int nResults = (int)m_cSearchResult.size();

    if(nResults == 0)
      printf("\n**** Error: Search failed, nothing to print.\n");

    if(m_cSearchResult.pop(result)){ //get current search result
      CBoard& b = *(result.m_pBoard);
      std::string s = MakeFileNameBase(result.m_cTourneyDesc, b.GetWidth());

      b.Save(s); //save tour to text file
      b.SaveToSVG(s); //save to SVG file

      delete result.m_pBoard;
    } //if
  } //else
} //Generate

#pragma endregion generation task

///////////////////////////////////////////////////////////////////
// Code for Task::Measure

#pragma region Task::Measure

/// Measure statistics on multiple knight's tours or tourneys.
/// Fill the request queue, launch the search threads, then
/// wait for them to terminate. Count the number of occurrences
/// of the 8 single moves and 8 double moves possible and write
/// the results to a text file.
/// \param t Type of tour to generate.
/// \param nThreads Number of search threads to use.
/// \param n Number of tours to generate.

void CGenerator::Measure(const CTourneyDesc& t, int nThreads, int n){ 
  //queue up search requests

  for(int i=0; i<n; i++){
    CSearchRequest request(t, m_nWidth, m_nHeight);
    request.m_bDiscard = true; //we're measuring stats, so throw them away
    m_cSearchRequest.push(request); //submit request
  } //for

  //start timing CPU and elapsed time

  CTimer Timer;
  Timer.Start();

  printf("Starting %d theads at: %s", nThreads, Timer.GetCurrentDateAndTime());

  //launch the search threads
  
  for(int i=0; i<nThreads; i++)
    m_vecThreadList.push_back(std::thread((CSearchThread())));

  //wait for all search threads to terminate
  
  std::for_each(m_vecThreadList.begin(), m_vecThreadList.end(), 
    std::mem_fn(&std::thread::join));

  Timer.Finish();
  m_vecThreadList.clear(); //clear the thread list for next use

  //process the measurements that were made by the threads

  //you can't iterate through std::queue, so transfer results to an std::vector

  std::vector<CSearchResult> results; //result list

  CSearchResult r; //current search result

  while(m_cSearchResult.pop(r)) //get current search result
    results.push_back(r);

  //now process the results

  double fSingleMove[8] = {0}; ///< Single move count
  double fDoubleMove[8] = {0}; ///< Double move count
  
  double fSingleMean[8] = {0}; ///< Single move observed mean
  double fDoubleMean[8] = {0}; ///< Double move observed mean 
    
  double fSingleStdev[8] = {0}; ///< Single move standard deviation
  double fDoubleStdev[8] = {0}; ///< Double move standard deviation

  for(CSearchResult r: results)
    for(int i=0; i<8; i++){
      fSingleMove[i] += (double)r.m_nSingleMove[i];
      fDoubleMove[i] += (double)r.m_nDoubleMove[i];
    } //for

  for(int i=0; i<8; i++){
    fSingleMove[i] /= (double)m_nSize;
    fDoubleMove[i] /= (double)m_nSize;
  } //for

  //compute observed mean
  
  for(int i=0; i<8; i++){
    fSingleMean[i] = fSingleMove[i]/(double)n;
    fDoubleMean[i] = fDoubleMove[i]/(double)n;
  } //for

  //compute observed standard deviation

  const double denom = (double)m_nSize; //denominator
  
  for(CSearchResult r: results){
    for(int i=0; i<8; i++){
      fSingleStdev[i] += sqr((double)r.m_nSingleMove[i]/denom - fSingleMean[i]);
      fDoubleStdev[i] += sqr((double)r.m_nDoubleMove[i]/denom - fDoubleMean[i]);
    } //for
  } //for
  
  if(n > 1){
    const double denom = (double)n - 1; //denominator for standard deviation

    for(int i=0; i<8; i++){
      fSingleStdev[i] = sqrt(fSingleStdev[i]/denom);
      fDoubleStdev[i] = sqrt(fDoubleStdev[i]/denom);
    } //for
  } //if

  //write mean and standard deviation to a file
  
  std::string strFileName = "Stats" + MakeFileNameBase(t, m_nWidth) //file name
    + "-" + std::to_string(n) + ".txt";

  FILE* output = fopen(strFileName.c_str(), "wt"); //open file

  if(output != nullptr){ //success
    fprintf(output, "Single\n");
    fprintf(output, "Mean\t");  OutputStat(output, fSingleMean);
    fprintf(output, "Stdev\t"); OutputStat(output, fSingleStdev);
    fprintf(output, "\n");
    
    fprintf(output, "Double\n");    
    fprintf(output, "Mean\t");  OutputStat(output, fDoubleMean);    
    fprintf(output, "Stdev\t"); OutputStat(output, fDoubleStdev);

    fclose(output); //close file
  } //if
} //Measure

/// Output statistic from the generation multiple knight's tours
/// or tourneys to a file.
/// \param output File pointer.
/// \param a Array of 8 numbers.

void CGenerator::OutputStat(FILE* output, double a[8]){
  assert(output != 0); //safety
  fprintf(output, "%0.4f", a[0]);
  for(int i=1; i<8; i++)
    fprintf(output, "\t%0.4f", a[i]);
  fprintf(output, "\n");
} //OutputStat

#pragma endregion Task::Measure

///////////////////////////////////////////////////////////////////
// Code for Task::Time

#pragma region Task::Time

/// Report the CPU and elapsed time required to generate multiple knight's 
/// tours or tourneys. Fill the request queue, launch the search threads,
/// wait for them to terminate, then append the CPU and elapsed times to
/// a text file.
/// \param t Tourney descriptor.
/// \param nThreads Number of search threads to use.
/// \param n Number of tours to generate.

void CGenerator::Time(const CTourneyDesc& t, int nThreads, int n){
  //queue up search requests

  for(int i=0; i<n; i++){
    CSearchRequest request(t, m_nWidth, m_nHeight);
    request.m_bDiscard = true;
    m_cSearchRequest.push(request);
  } //for

  //start timing CPU and elapsed time

  CTimer Timer;
  Timer.Start();

  //launch the search threads
  
  for(int i=0; i<nThreads; i++)
    m_vecThreadList.push_back(std::thread((CSearchThread())));

  //wait for all search threads to terminate
  
  std::for_each(m_vecThreadList.begin(), m_vecThreadList.end(), 
    std::mem_fn(&std::thread::join));
  
  m_vecThreadList.clear(); //clear the thread list for next use
  
  //append cpu and elapsed time to a file

  std::string strFileName = "Time" + MakeFileNameBase(t);
  strFileName += "-" + std::to_string(n) + ".txt";

  FILE* output = fopen(strFileName.c_str(), "at");

  if(output != nullptr){
    OutputTimes(output, Timer.GetCPUTime(), Timer.GetElapsedTime());
    fclose(output);
  } //if
} //Time

/// Append times (cpu time and elapsed time) from the generation of multiple
/// knight's tours or tourneys to a text file. The file name contains the 
/// tourney type.
/// \param output Pointer to file.
/// \param fCpu CPU time in seconds.
/// \param fElapsed Elapsed time in seconds.

void CGenerator::OutputTimes(FILE* output, float fCpu, float fElapsed){
  assert(output != 0); //safety

  if(output != nullptr)
    fprintf(output, "%d\t%0.2f\t%0.2f\n", m_nWidth, fCpu, fElapsed);
} //OutputTimes

#pragma endregion Task::Time

