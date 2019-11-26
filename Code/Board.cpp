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

CBoard::CBoard(unsigned n): 
  CBaseBoard(n){
} //constructor

/// Construct a rectangular undirected board.
/// \param w Board width.
/// \param h Board height.

CBoard::CBoard(unsigned w, unsigned h):
  CBaseBoard(w, h){
} //constructor

/// Construct an undirected board from a move table.
/// \param w Board width.
/// \param h Board height.
/// \param move A \f$w \times h\f$ move table.

CBoard::CBoard(int move[], unsigned w, unsigned h):
  CBaseBoard(move, w, h){
} //constructor

/// Test whether a rail is valid, that is, the rail moves are knight's moves
/// and are present on the board, and the cross moves are not present. Calls
/// CBaseBoard::IsRail(int, int, int, int) to do the actual work.
/// \param r Rail to be tested.
/// \return true if the rail is valid.

bool CBoard::IsRail(CRail& r){
  int s0, d0, s1, d1; //source and destination cells

  r.GetEdge0(s0, d0); //get them
  r.GetEdge1(s1, d1);

  return IsRail(s0, d0, s1, d1); //test them
} //IsRail

/// Test whether four cells form a rail, that is, the four cells are separated
/// by knight's moves, the rail moves are present on the board, and the cross
/// moves are not present.
/// \param s0 Index of source of first move.
/// \param d0 Index of destination of first move.
/// \param s1 Index of source of second move.
/// \param d1 Index of destination of second move.
/// \return true if the cells form a rail.

bool CBoard::IsRail(int s0, int d0, int s1, int d1){
  return IsKnightMove(s0, d0) && IsKnightMove(s1, d1) && //rails are knight's moves
    IsKnightMove(s0, s1) && IsKnightMove(d0, d1) && //flipped rails are knight's moves
    IsMove(s0, d0) && IsMove(s1, d1) && //moves that need to be there
    !IsMove(s0, s1) && !IsMove(d0, d1); //moves that need to be not there 
} //IsRail

/// A rail spans different cycles if both vertices of each edge
/// are in the same cycle, but it's a different cycle for each edge.
/// \param r A rail.
/// \param id Array of tourney identifiers.
/// \return true if the rail spans different cycles.

bool CBoard::RailSpansCycles(CRail& r, int id[]){
  int src0, dest0, src1, dest1; //rail vertices

  r.GetEdge0(src0, dest0); //rail edge
  r.GetEdge1(src1, dest1); //rail edge

  return id == nullptr || //either we don't actually care, or
    (id[src0] == id[dest0] && //vertices of edge 0 are in a cycle
     id[src1] == id[dest1] && //vertices of edge 1 are in a cycle
     id[src0] != id[src1]); //edges are in different cycles
} //RailSpansCycles

/// Find all rails. Assumes that the board is directed. The algorithm used is
/// as follows. Suppose we have an \f$n \times n\f$ board. Let \f$0 < i \leq n\f$
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

/// Flip a rail. Assumes that the board is directed. The rails in the top row
/// of this image get flipped to the corresponding rails in the bottom row 
/// (and vice-versa).
///
/// \image html rails.png
///
/// \param r A rail.

void CBoard::FlipRail(CRail& r){
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
} //FlipRail

/// Shatter a tourney by flipping a set of non-overlapping rails.
/// Assumes that the board is directed.

void CBoard::Shatter(){ 
  assert(IsDirected()); //safety

  std::vector<CRail> rails; //rail list.
  FindRails(rails); //find rails and put them in the rail list
  
  for(CRail& r: rails){ //for each rail
    int src0, src1, dest0, dest1; //source and destination cells of rail moves

    r.GetEdge0(src0, dest0); //rail move 0
    r.GetEdge1(src1, dest1); //rail move 1

    //flip valid rails (validity is affected by rails already flipped).

    if(IsMove(src0, dest0) && IsMove(src1, dest1) &&
      !IsMove(src0, src1) && !IsMove(dest0, dest1))  
        FlipRail(r);
  } //for
} //Shatter

/// Blur a tourney by shattering it a few times. The board can be directed or
/// undirected initially, but it will be undirected after blurring. The result
/// should be a knight's tour in most cases.

void CBoard::Blur(){
  MakeDirected(); //need a directed board
  
  for(int i=0; i<11; i++) //shatter many times
    Shatter();
  for(int i=0; i<8; i++) //hopefully one of these joins will stick
    Join();
  
  MakeUndirected(); //make it undirected before returning
} //Blur

/// Join a tourney, that is, attempt to make it into a knight's tour by 
/// flipping rails. Maintains directedness. First a rail (multi-) graph is
/// created with a vertex for each tourney and an edge for each cell-disjoint
/// rail that connects them. Flipping the rails in a spanning tree of the rail
/// graph connects up as many cycles as it can. This should result in fewer
/// cycles. For example, the following image (left) shows the rail graph of 
/// a tourney. The numbers on the edges indicate the number of cell-disjoint
/// rails that intersect  two cycles. A breadth-first spanning tree of this
/// graph is shown on the right.
///
/// \image html bfst22.png

void CBoard::Join(){
  if(IsTour())return; //bail out, it's a knight's tour already

  //make board directed, if it isn't already

  bool bWasUndirected = false;

  if(IsUndirected()){
    bWasUndirected = true;
    MakeDirected();
  } //if
  
  std::vector<CRail> rails; //rail list.
  FindRails(rails); //find rails and put them in the rail list

  //identify which tourney each cell is in

  int numcycles = 0; //number of cycles seen in tourney

  int *id = new int[m_nSize]; //identifies which tourney a cell is in

  for(unsigned i=0; i<m_nSize; i++) //for each cell
    id[i] = UNUSED; //set identifier to indicate "no tourney"

  for(unsigned start=0; start<m_nSize; start++){ //for each start cell
    if(id[start] == UNUSED){ //if not already in a tourney
      id[start] = numcycles; //identify as in a new tourney

      int prev = start; //previous cell
      int cur = m_nMove[start]; //current cell

      while(CellIndexInRange(cur) && cur != start){ //until it loops back to start
        id[cur] = numcycles; //identify each cell in the new cycle
        
        const int dest0 = m_nMove[cur]; //dest0 move

        if(dest0 == prev){ //dest0 move in m_nMoveTable moves us back
          prev = cur;
          cur = m_nMove2[cur]; //use m_nMoveTable2 instead 
        } //if

        else{ //safe to use m_nMoveTable
          prev = cur;
          cur = dest0;
        } //else
      } //while

      ++numcycles; //one more cycle seen in tourney
    } //if
  } //for

  //construct the rail graph

  bool* used = new bool[m_nSize];

  for(unsigned i=0; i<m_nSize; i++)
    used[i] = false;

  CGraph g(numcycles); //rail graph

  unsigned count = 0;

  std::vector<unsigned> vecEdgeToRail;

  for(auto& r: rails){ //for each rail
    int src0, dest0, src1, dest1; //rail vertices

    r.GetEdge0(src0, dest0); //rail edge
    r.GetEdge1(src1, dest1); //rail edge

    if(!used[src0] && !used[dest0] && !used[src1] && !used[dest1]){
      const int idsrc0 = id[src0]; //id of tourney that src0 is in
      const int iddest0 = id[dest0]; //id of tourney that dest0 is in

      const int idsrc1 = id[src1]; //id of tourney that src1 is in
      const int iddest1 = id[dest1]; //id of tourney that dest1 is in

      if(idsrc0 == iddest0 && idsrc1 == iddest1 && idsrc0 != idsrc1){ //rail connects tourneys
        g.InsertEdge(idsrc0, idsrc1); //add rail edge to rail graph
        used[src0] = used[src1] = used[dest0] = used[dest1] = true;
        vecEdgeToRail.push_back(count); 
      } //if
    } //if

    count++;
  } //for

  delete [] used;
  delete [] id; 

  //g.PrintGraph(); //use to get a graph for a figure

  //flip rails in a spanning tree of the rail graph

  std::vector<unsigned> spanningtree; //rails in spanning tree of rail graph
  g.BFST(spanningtree); //could use g.DFST instead

  for(unsigned i: spanningtree){ //for each rail in the spanning tree
    unsigned j = vecEdgeToRail[i]; //index in rail list
    FlipRail(rails[j]); //flip it
  } //for

  //clean up and exit

  if(bWasUndirected) //if the board came in to this function undirected
    MakeUndirected(); //make it undirected again
} //Join
