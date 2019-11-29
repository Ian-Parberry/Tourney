/// \file Warnsdorf.cpp
/// \brief Code for Warnsdorf's generator CWarnsdorf.

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

#include "Warnsdorf.h"
#include "Defines.h"

extern std::atomic_bool g_bFinished; ///< Search termination flag.
extern MoveDeltas g_vecDeltas; ///< Move deltas for all possible knight's moves. 

/// The default constructor seeds the PRNG.

CWarnsdorf::CWarnsdorf(int seed){
  ::srand(seed); //seed the default PRNG
  m_cRandom.srand(); //seed our PRNG
} //constructor

/// Attempt to generate a random knight's tour.
/// \param b [out] Board for generated tour.
/// \return true if generation is successful.

bool CWarnsdorf::GenerateTour(CBoard& b){
  const int w = b.GetWidth();
  const int n = b.GetSize();
  
  b.Clear(); 

  int target = m_cRandom.randn(0, n - 1);
  int current = target;

  int next; 
  int nVisited = 1;
  
  std::set<int> m_bEgress;
  
  for(MoveDelta delta: g_vecDeltas)
   if(b.IsOnBoard(target, delta))
     m_bEgress.insert(target + delta.second*w + delta.first);

  int available[8];
  int preferred[8];
  int preferredcount = 0;

  int exitcount[8];
  int count = 0;

  do{
    count = 0;

    //enumerate all possible places you could jump to from current and
    //record them in array "available", setting "count" to the number of them
    
    for(MoveDelta delta: g_vecDeltas){  
      int next = current + delta.second*w + delta.first; 
      if(b.IsOnBoard(current, delta) && b[next] == UNUSED)
        available[count++] = next;
    } //for

    //count the number of moves out of all possible squares
    //in array "available" and record them in array "exitcount"
    //also record the smallest of these in "min"

    int prefcount = 0;

    int min = 9999;

    for(int i=0; i<count; i++){
      exitcount[i] = b.GetAvailableMoveCount(available[i]);

      if(exitcount[i] < min)
        min = exitcount[i];
    } //for

    //transfer to array "preferred" all of those entries in "available"
    //that have exitcount equal to the minimum
    //record the number of these in "prefcount"

    for(int i=0; i<count; i++)
      if(exitcount[i] <= min && b[available[i]] != current)
        preferred[prefcount++] = available[i];
    
    //choose a random one of the minima

    if(prefcount > 0){ 
      if(prefcount == 1)
        next = preferred[0]; //unique minimum

      else do{
        const int index = m_cRandom.randn(0, prefcount - 1);
        next = preferred[index]; //pick preferred next move
      }while(next == target && prefcount > 1); //can't be target unless there's no choice  

      //move there

      b.InsertUndirectedMove(current, next); 
      current = next; //move from current to next
      nVisited++; //one more square visited
    } //if

    m_bEgress.erase(current); //we've used up 1 exit point
  }while(count > 0 && m_bEgress.size() > 0 && !g_bFinished);

  if(!g_bFinished && b.IsKnightMove(current, target) && nVisited >= n){
    b.InsertUndirectedMove(current, target); 
    return true;
  } //if

  return false;
} //GenerateTour

/// Attempt to generate a random tourney.
/// \param b [in, out] Chessboard.
/// \return true if generation is successful.

bool CWarnsdorf::GenerateTourney(CBoard& b){
  const int w = b.GetWidth();
  const int n = b.GetSize();

  bool bCycleCover = false; 

  b.Clear(); 

  for(int i=0; i<4*w && !bCycleCover; i++){
    bool bFinished = false;
    int first = 0;

    //find an unused cell
    while(b[first] != UNUSED && first < n)
      first++;

    bCycleCover = first >= n;
    bFinished = bCycleCover;

    if(!bFinished){
      int last = RandomClosedWalk(b, first); //do a random walk

      //finished if we can close the cycle and it has length > 1
      bFinished = b.IsKnightMove(first, last) && 
        b[first] != last && b[last] != first;

      if(bFinished)
        b.InsertUndirectedMove(last, first);

      else{ //erase attempt at cycle
        int cur = first;
        int next = b[cur];

        while(cur != last){
          b.DeleteMove(cur, next);
          cur = next;
          next = b[cur];
        } //while
      } //else
    } //if
  } //while

  return bCycleCover;
} //GenerateTourney

/// Take a random walk and close it into a cycle at the first opportunity.
/// \param b [in, out] Chessboard.
/// \param start Index of cell from which to begin cycle.

int CWarnsdorf::RandomClosedWalk(CBoard& b, int start){
  const int w = b.GetWidth();
  const int n = b.GetSize();
  
  int next = 0; 
  int current = start;
  int exitcount[8];

  int nVisited = 1;
  int nNextMoveCount = 1;

  int nNumTrials = 0;

  int available[8];

  while(nNumTrials < 4*w && (!b.IsKnightMove(current, start) || nVisited < 6)){
    nNumTrials++;
    nNextMoveCount = 0;

    //enumerate all possible places you could jump to from current and
    //record them in array "available", setting "count" to the number of them
    
    for(MoveDelta delta: g_vecDeltas) {  
      int next = current + delta.second*w + delta.first; 
      if(b.IsOnBoard(current, delta) && b[next] == UNUSED)
        available[nNextMoveCount++] = next;
    } //for

    //count the number of moves out of all possible squares
    //in array "available" and record them in array "exitcount"
    //also record the smallest of these in "min"

    int min = 9999;

    for(int i=0; i<nNextMoveCount; i++){
      exitcount[i] = b.GetAvailableMoveCount(available[i]);
      if(exitcount[i] < min)
        min = exitcount[i];
    } //for

    //transfer to array "preferred" all of those entries in "available"
    //that have exitcount equal to the minimum
    //record the number of these in "preferredcount"

    int preferredcount = 0;
    int preferred[8];

    for(int i=0; i<nNextMoveCount; i++){
      if(exitcount[i] <= min && b[available[i]] != current)
        preferred[preferredcount++] = available[i];
    } //for

    //choose a random one of the minima

    if(preferredcount > 0){ 
      if(preferredcount == 1)
        next = preferred[0]; //unique minimum

      else{ //pick a preferred move
        const int n = m_cRandom.randn(0, preferredcount - 1);
        next = preferred[n];
      } //else

      //move there

      b.InsertUndirectedMove(current, next);
      current = next; 
      nVisited++; 
    } //if
  } //while

  return current;
} //RandomClosedWalk

/// Generate a knight's tour or tourney using Warnsdorf's algorithm.
/// \param b [out] Board.
/// \param t Cycle type. 

void CWarnsdorf::Generate(CBoard& b, CycleType t){
  switch(t){
    case CycleType::Tour:
      while(!GenerateTour(b) && !g_bFinished); //generate tour
      break;
      
    case CycleType::Tourney:
      while(!GenerateTourney(b) && !g_bFinished); //generate tourney
      break;

    case CycleType::TourFromTourney:
      while(!GenerateTourney(b) && !g_bFinished); //generate tourney
      b.Join(); //make tour from tourney
      break;
  } //switch
} //Generate
