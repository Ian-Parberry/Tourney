/// \file SearchThread.cpp
/// \brief Code for the search thread CSearchThread.

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

#include "SearchThread.h"

#include "TakefujiLee.h"
#include "Warnsdorff.h"
#include "DivideAndConquer.h"
#include "ConcentricBraid.h"
#include "FourCover.h"

extern std::atomic_bool g_bFinished; ///< Search termination flag.

/// The function executed by a search thread, which repeatedly pops
/// a search request from the thread-safe request queue m_cSearchRequest
/// and calls Generate() to perform the requested search.
/// The thread terminates when the request queue is empty.

void CSearchThread::operator()(){
  CSearchRequest request; //current search request

  while(m_cSearchRequest.pop(request)) //grab a search request from the queue
    Generate(request); //perform search
} //operator()()

/// Generate a knight's tour or tourney according to a search request.
/// \param request Search request.

void CSearchThread::Generate(CSearchRequest& request){ 
  const int w = request.m_nWidth;
  const int h = request.m_nHeight;
  const int n = request.m_nSize;

  CBoard* pBoard = new CBoard(w, h); //pointer to chessboard

  const GeneratorType gentype = request.m_cTourneyDesc.m_eGenerator; //generator
  const CycleType cycletype = request.m_cTourneyDesc.m_eCycle; //tour or tourney
  const bool obfuscate = request.m_cTourneyDesc.m_bObfuscate; //whether to obfuscate
  const int seed = request.m_nSeed; //PRNG seed
 
  switch(gentype){
    case GeneratorType::Warnsdorff:
      CWarnsdorff(seed).Generate(*pBoard, cycletype);
      break; 

    case GeneratorType::TakefujiLee:
      CTakefujiLee(w, h, seed).Generate(*pBoard); //can only generate tourneys
      break; 

    case GeneratorType::DivideAndConquer: 
      CDivideAndConquer().Generate(*pBoard, cycletype);
      break;

    case GeneratorType::ConcentricBraid: //can only generate tourneys
      CConcentricBraid().Generate(*pBoard);

    case GeneratorType::FourCover: //can only generate tourneys
      CFourCover().Generate(*pBoard);
  } //switch

  //post-processing tourney

  if(cycletype == CycleType::TourFromTourney) //make tour from tourney
    pBoard->JoinUntilTour();

  if(obfuscate)pBoard->Obfuscate(); //obfuscate
 
  if(request.m_bDiscard){ //report statistics
    CSearchResult result(nullptr, request.m_cTourneyDesc);

    for(int i=0; i<n; i++){ //for each cell
      int dest = (*pBoard)[i]; //destination after one move
      int dest2 = (*pBoard)[dest]; //destination after two moves

      const int n = pBoard->GetMoveIndex(i, dest); //1st move index in 0..7

      if(0 <= n && n < 8){ //safety
        result.m_nSingleMove[n]++; //record single move

        int n2 = pBoard->GetMoveIndex(dest, dest2) - n;
        if(n2 < 0)n2 += 8;  //2nd move index in 0..7, relative to 1st move
        result.m_nRelativeMove[n2]++; //record double move
      } //if
    } //for
    
    m_cSearchResult.push(result); 
    delete pBoard;
  } //if

  else{ //we are tasked with generating a single tour
    if(!g_bFinished){ //report generated tour
      g_bFinished = true; //signal other threads to terminate 
      m_cSearchResult.push(CSearchResult(pBoard, request.m_cTourneyDesc)); 
    } //if

    else delete pBoard;
  } //else
} //Generate

