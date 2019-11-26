/// \file DivideAndConquer.cpp
/// \brief Code for the divide-and conquer generator CDivideAndConquer.

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

#include "DivideAndConquer.h"

/// Default constructor.

CDivideAndConquer::CDivideAndConquer(): CTile(){ 
} //constructor

/// Generate a knight's tour or tourney. 
/// \param b [in, out] Board.
/// \param t Tourney descriptor.

void CDivideAndConquer::Generate(CBoard& b, CycleType t){
  b.MakeDirected(); //the generation algorithm requires a directed board
  Generate(b, t, CRect(0, b.GetWidth(), 0, b.GetHeight())); //the whole board
  b.MakeUndirected(); //return an undirected board
} //Generate

/// Generate a tour or tourney in a sub-board using divide-and-conquer.
/// Requires that the board is directed, otherwise this function does nothing.
/// This is a recursive function, but the depth of recursion is only
/// \f$\log_2 n + O(1)\f$ and the total work done is \f$O(n^2)\f$ for an
/// \f$n \times n\f$ board (that is, time linear in the size of the board).
/// \param b [in, out] Board.
/// \param t Tourney descriptor. 
/// \param rect Rectangle defining sub-board.

void CDivideAndConquer::Generate(CBoard& b, CycleType t, const CRect& rect){
  if(b.IsUndirected())return; //enforce requirement that the board is directed

  const int w = rect.m_nRight - rect.m_nLeft;
  const int h = rect.m_nBottom - rect.m_nTop;

  if(w < 12 || h < 12) //base of recursion
    GenerateBaseCase(b, rect);

  else{ //split into four quadrants and generate a knight's tour in each
    const int midx = Split(rect.m_nLeft, rect.m_nRight);
    const int midy = Split(rect.m_nTop, rect.m_nBottom);

    const CRect r0(rect.m_nLeft, midx, rect.m_nTop, midy); //quadrant 0
    const CRect r1(midx, rect.m_nRight, rect.m_nTop, midy); //quadrant 1
    const CRect r2(rect.m_nLeft, midx, midy, rect.m_nBottom); //quadrant 2
    const CRect r3(midx, rect.m_nRight, midy, rect.m_nBottom); //quadrant 3
    
    Generate(b, t, r0); //recurse on quadrant 0
    Generate(b, t, r1); //recurse on quadrant 1
    Generate(b, t, r2); //recurse on quadrant 2
    Generate(b, t, r3); //recurse on quadrant 3

    if(t == CycleType::Tour) //join the quadrants
      Join(b, midx, midy);
  } //else
} //Generate

/// Splits a pair of coordinates nearly in half such that both parts
/// have even parity.
/// \param left Left coordinate.
/// \param right Right coordinate.
/// \return The split of the two coordinates.

int CDivideAndConquer::Split(int left, int right){
  return (left + right)/2 - ((right - left)%4 == 2? 1: 0);
} //Split

/// Join together four tourneys created by the divide-and-conquer algorithm.
/// Requires that the board is directed, otherwise this function does nothing.
/// The base tours in CTour were carefully chosen so that they include
/// the moves shown in the image below. This implies that all recursively
/// created tours also include these moves. 
///
/// \image html corners.png
///
/// We choose four of those edges \f$A, B, C, D\f$ as shown at left in the
/// image below and replace them with the edges \f$E, F, G, H\f$ shown at right.
///
/// \image html divide-conquer.png
///
/// \param b [in, out] Board.
/// \param midx Split x-coordinate.
/// \param midy Split y-coordinate.

void CDivideAndConquer::Join(CBoard& b, int midx, int midy){
  if(b.IsUndirected())return; //enforce requirement that the board is directed

  const int w = b.GetWidth();

  //delete moves A, B, C, D (see the figure in the Doxygen-generated docs).

  const int A_left = (midy - 1)*w + midx - 3; 
  const int A_right = (midy - 2)*w + midx - 1;

  const int B_left = (midy - 1)*w + midx; 
  const int B_right = (midy - 3)*w + midx + 1;

  const int C_left = (midy + 1)*w + midx; 
  const int C_right = midy*w + midx + 2; 

  const int D_left = (midy + 2)*w + midx - 2; 
  const int D_right = midy*w + midx - 1;

  b.DeleteMove(A_left, A_right);
  b.DeleteMove(B_left, B_right);
  b.DeleteMove(C_left, C_right);
  b.DeleteMove(D_left, D_right);

  //replace them with new moves
  
  b.InsertDirectedMove(A_right, B_right);
  b.InsertDirectedMove(B_left,  C_right);
  b.InsertDirectedMove(C_left,  D_left);
  b.InsertDirectedMove(D_right, A_left);
} //Join

/// Copy one of the base tiles in the base class CTile to a sub-board.
/// \param b [in, out] Board.
/// \param rect Rectangle in which to place tile.

void CDivideAndConquer::GenerateBaseCase(CBoard& b, const CRect& rect){
  const int w = rect.m_nRight - rect.m_nLeft; //width of tile
  const int h = rect.m_nBottom - rect.m_nTop; //height of tile

  const int x = rect.m_nLeft; //column of cell for left of tile
  const int y = rect.m_nTop; //row of cell for top of tile

  //copy the correct tile on a case-by-case basis

  if(w == 6 && h == 6)
    b.CopyToSubBoard(*m_pTile6x6, x, y);

  else if(w == 8 && h == 6)
    b.CopyToSubBoard(*m_pTile8x6, x, y);

  else if(w == 6 && h == 8)
    b.CopyToSubBoard(*m_pTile6x8, x, y);

  else if(w == 8 && h == 8)
    b.CopyToSubBoard(*m_pTile8x8, x, y);

  else if(w == 10 && h == 8)
    b.CopyToSubBoard(*m_pTile10x8, x, y);

  else if(w == 8 && h == 10)
    b.CopyToSubBoard(*m_pTile8x10, x, y);

  else if(w == 10 && h == 10)
    b.CopyToSubBoard(*m_pTile10x10, x, y);

  else if(w == 12 && h == 10)
    b.CopyToSubBoard(*m_pTile12x10, x, y);

  else if(w == 10 && h == 12)
    b.CopyToSubBoard(*m_pTile10x12, x, y);
} //GenerateBaseCase