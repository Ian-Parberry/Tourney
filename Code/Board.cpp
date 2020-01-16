/// \file Board.cpp
/// \brief Code for the chessboard CBaseBoard.

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

#include "Board.h"

#include "Defines.h"
#include "Includes.h"
#include "Graph.h"

extern MoveDeltas g_vecDeltas; ///< Move deltas for all possible knight's moves. 

/// Construct an empty board.

CBoard::CBoard(){
} //constructor

/// Construct a square undirected board.
/// \param n Board width and height.

CBoard::CBoard(UINT n): 
  CBaseBoard(n){
} //constructor

/// Construct a rectangular undirected board.
/// \param w Board width.
/// \param h Board height.

CBoard::CBoard(UINT w, UINT h):
  CBaseBoard(w, h){
} //constructor

/// Construct an undirected board from a move table.
/// \param w Board width.
/// \param h Board height.
/// \param move A \f$w \times h\f$ move table.

CBoard::CBoard(int move[], UINT w, UINT h):
  CBaseBoard(move, w, h){
} //constructor

/// Test whether a rail is valid, that is, all moves are knight's moves, the
/// primary moves are present, and the cross moves are absent.
/// Calls CBaseBoard::IsRail(int, int, int, int) to do the actual work.
/// \param r Rail to be tested.
/// \return true if the rail is valid.

bool CBoard::IsRail(CRail& r){
  int s0, d0, s1, d1; //source and destination cells

  r.GetEdge0(s0, d0); //get them
  r.GetEdge1(s1, d1);

  return IsRail(s0, d0, s1, d1);
} //IsRail

/// Test whether four cells form a rail, that is, they are separated by knight's
/// moves, the primary moves are present, and the cross moves are absent.
/// \param s0 Index of source of first move.
/// \param d0 Index of destination of first move.
/// \param s1 Index of source of second move.
/// \param d1 Index of destination of second move.
/// \return true if the cells form a rail.

bool CBoard::IsRail(int s0, int d0, int s1, int d1){
  return 
    IsKnightMove(s0, d0) && IsKnightMove(s1, d1) && //primary knight's moves
    IsKnightMove(s0, s1) && IsKnightMove(d0, d1) && //cross knight's moves
    IsMove(s0, d0) && IsMove(s1, d1) && //primary moves are present
    !IsMove(s0, s1) && !IsMove(d0, d1); //cross moves are absent 
} //IsRail

/// Find all rails. Assumes that the board is directed. The algorithm used is as
/// follows. Suppose we have an \f$n \times n\f$ board. Let \f$0 < i \leq n\f$
/// be a cell index. The potential downward moves from cell \f$i\f$ have move
/// index \f$4\f$, \f$5\f$, \f$6\f$, or \f$7\f$.
///
/// \image html bird-4-7.png 
///
/// If the move from cell \f$i\f$ has index \f$4\f$, then there are three 
/// potential rails incident with cell \f$i\f$ and the corresponding parallel
/// moves are separated from cell \f$i\f$ by moves \f$5\f$, \f$6\f$, or 
/// \f$7\f$, as shown below. 
///
/// \image html bird-4.png 
///
/// If the move from cell \f$i\f$ has index \f$5\f$, then there are three
/// potential rails incident with cell \f$i\f$, and the corresponding parallel
/// moves are separated from cell \f$i\f$ by moves \f$4\f$, \f$6\f$ or 
/// \f$7\f$, as shown below.
///
/// \image html bird-5.png 
///
/// If the move from cell \f$i\f$ has index \f$6\f$, then there are three
/// potential rails incident with cell \f$i\f$, and the corresponding parallel
/// moves are separated from cell \f$i\f$ by moves \f$4\f$, \f$5\f$ or 
/// \f$7\f$, as shown below.
///
/// \image html bird-6.png 
///
/// If the move from cell \f$i\f$ has index \f$7\f$, then there are three
/// potential rails incident with cell \f$i\f$, and the corresponding parallel
/// moves are separated from cell \f$i\f$ by moves \f$4\f$, \f$5\f$ or 
/// \f$6\f$, as shown below.
///
/// \image html bird-7.png 
///
/// We therefore iterate through \f$0 < i \leq n\f$ and consider the two moves
/// from cell \f$i\f$. If one of these moves has index \f$4\f$, then we check
/// whether moves \f$5\f$, \f$6\f$, or \f$7\f$ lead to a cell that also has
/// move \f$4\f$ from it. If so, then we add these rails to the rail vector
/// provided to this function as a call-by-reference parameter. Similarly, we
/// perform the respective checks when one of the moves from cell \f$i\f$ has
/// index \f$5\f$, \f$6\f$, or \f$7\f$.
///
/// The rail list is permuted into random order before returning.
///
/// \param rails [out] Rail list.

void CBoard::FindRails(std::vector<CRail>& rails){
  assert(IsDirected()); //safety

  for(int s0=0; s0<(int)m_nSize; s0++) //source of move 0
    for(int d0: {m_nMove[s0], m_nMove2[s0]}){ //destination of move 0
      const int i = GetMoveIndex(s0, d0); //index of move 0
      
      if(i >= 4) //move 0 is forwards wrt the first for-loop
        for(int j=4; j<8; j++) //downwards cross move from s0
          if(i != j){ //eliminate the only forwards move that isn't a rail
            const int s1 = GetDest(s0, g_vecDeltas[j]); //source of move 1

            if(s1 != UNUSED) //move 1 stays on board
              for(int d1: {m_nMove[s1], m_nMove2[s1]}) //destination of move 1
                if(IsRail(s0, d0, s1, d1)) //we have a rail
                  rails.push_back(CRail(s0, d0, s1, d1, m_nWidth)); //record it
          } //if
    } //for

  //randomize the rail list by applying a pseudo-random permutation
  //using the standard random permutation generation algorithm
  
  const int n = (int)rails.size(); //number of rails

  for(int i=0; i<n; i++) //yes, the indices on the next line are correct...
    std::swap(rails[i], rails[m_cRandom.randn(i, n - 1)]); //...because math
} //FindRails

/// Switch a rail. Assumes that the board is directed. The rails in the top row
/// of this image get switched to the corresponding rails in the bottom row 
/// (and vice-versa).
///
/// \image html rails.png
///
/// \param r A rail.

void CBoard::Switch(CRail& r){
  assert(IsDirected()); //safety

  int s0, d0, s1, d1; //rail vertices

  r.GetEdge0(s0, d0); //rail edge
  r.GetEdge1(s1, d1); //rail edge

  //delete the moves that are there

  DeleteMove(s0, d0);
  DeleteMove(s1, d1);

  //insert the cross moves
            
  InsertDirectedMove(s0, s1);
  InsertDirectedMove(d0, d1);
} //Switch

/// Shatter a tourney by switching a set of non-overlapping rails.
/// Assumes that the board is directed.

void CBoard::Shatter(){ 
  assert(IsDirected()); //safety

  std::vector<CRail> rails; //rail list
  FindRails(rails); //find rails and put them in the rail list
  
  for(CRail& r: rails) //for each rail
    if(IsRail(r)) //if it's still a rail, that is, it hasn't been flipped
      Switch(r); //flip it
} //Shatter

/// Obfuscate a tourney by shattering it a few times. The board can be directed
/// or undirected initially, but it will be undirected after obfuscating. The
/// result should be a knight's tour in most cases.

void CBoard::Obfuscate(){
  MakeDirected(); //need a directed board
  
  for(int i=0; i<16; i++)
    Shatter();

  JoinUntilTour();

  MakeUndirected(); //make it undirected before returning
} //Obfuscate

/// Join a tourney, that is, attempt to make it into a knight's tour by 
/// switching rails. Assumes that the tourney is directed. First a rail
/// (multi-) graph is created with a vertex for each tourney and an edge for
/// each cell-disjoint rail that connects them. Switching the rails in a
/// spanning forest of the rail graph connects up as many cycles as it can
/// (which may may not necessarily result in a knight's tour). For example, the
/// following image shows the rail graph of a tourney at left. The numbers on
/// the edges indicate the number of cell-disjoint rails that intersect two
/// cycles. A breadth-first spanning tree of this graph is shown on the right.
///
/// \image html bfst22.png
///
/// \return true if the join succeeeded.

bool CBoard::Join(){
  std::vector<CRail> rails; //rail list.
  FindRails(rails); //find rails and put them in the rail list

  //get tourney identifiers for each cell
  
  int* id = new int[m_nSize]; //tourney identifiers
  const UINT numcycles = GetTourneyIds(id); //get tourney id for each cell
  if(numcycles == 1)return true; //bail out, we're good

  //construct the rail graph

  bool* used = new bool[m_nSize];

  for(UINT i=0; i<m_nSize; i++)
    used[i] = false;

  CGraph g(numcycles); //rail graph
  UINT count = 0;
  std::vector<UINT> vecEdgeToRail;

  for(auto& r: rails){ //for each rail
    int src0, dest0, src1, dest1; //rail vertices

    r.GetEdge0(src0, dest0); //rail edge
    r.GetEdge1(src1, dest1); //rail edge

    if(!used[src0] && !used[dest0] && !used[src1] && !used[dest1]){
      const int idsrc0 = id[src0]; //id of tourney that src0 is in
      const int iddest0 = id[dest0]; //id of tourney that dest0 is in

      const int idsrc1 = id[src1]; //id of tourney that src1 is in
      const int iddest1 = id[dest1]; //id of tourney that dest1 is in

      if(idsrc0 == iddest0 && idsrc1 == iddest1 && idsrc0 != idsrc1){ 
        //rail connects tourneys
        g.InsertEdge(idsrc0, idsrc1); //add rail edge to rail graph
        used[src0] = used[src1] = used[dest0] = used[dest1] = true;
        vecEdgeToRail.push_back(count); 
      } //if
    } //if

    count++;
  } //for

  delete [] used;
  delete [] id; 

  //g.PrintGraph(); //uncomment to output a graph so you can make a figure

  //switch rails in a spanning tree of the rail graph

  std::vector<UINT> spanningforest; //rails in spanning forest of rail graph
  const UINT numtrees = g.BFSF(spanningforest); 

  for(UINT i: spanningforest){ //for each rail in the spanning forest
    UINT j = vecEdgeToRail[i]; //index of rail in rail list
    Switch(rails[j]); //switch a spanning tree rail
  } //for

  return numtrees == 1;
} //Join

/// Join a tourney until it becomes a knight's tour. Maintains directedness. 
/// Uses Join() to do the heavy lifting.

void CBoard::JoinUntilTour(){
  if(IsTour())return; //bail out, it's a knight's tour already

  //make board directed, if it isn't already

  bool bWasUndirected = false;

  if(IsUndirected()){
    bWasUndirected = true;
    MakeDirected();
  } //if

  UINT numcycles; //number of cycles
  
  do{
    numcycles = Join();
  }while(numcycles > 1);

  //clean up and exit

  if(bWasUndirected) //if the board came in to this function undirected
    MakeUndirected(); //make it undirected again
} //JoinUntilTour
