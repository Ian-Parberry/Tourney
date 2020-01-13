/// \file Rail.cpp
/// \brief Code for CRail.

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

#include "Defines.h"
#include "Includes.h"
#include "Helpers.h"

#include "Rail.h"

extern MoveDeltas g_vecDeltas; ///< Move deltas for all possible knight's moves. 

/// The rail constructor stores the indexes of the source and
/// destination cells of two knight's moves in which the two source
/// cells are separated by a knight's move and the two destination
/// are separated by a knight's move. If the parameters fail these
/// constraints then all four indices are set to UNUSED.
/// \param src0 Index of cell at source end of first move.
/// \param dest0 Index of cell at destination end of first move.
/// \param src1 Index of cell at source end of second move.
/// \param dest1 Index of cell at the destination end of second move.
/// \param w Board width.

CRail::CRail(int src0, int dest0, int src1, int dest1, UINT w):
  m_nSrc0(src0), m_nDest0(dest0), m_nSrc1(src1), m_nDest1(dest1), m_nWidth(w){   
} //constructor

/// Test whether two cells are separated by a knight's move. Assumes that
/// the board width m_nWidth has been set by the constructor.
/// \param i Board cell index.
/// \param j Board cell index.
/// \return true if j is a knight's move from i (and vice-versa)

bool CRail::IsKnightMove(int i, int j){
  for(MoveDelta delta: g_vecDeltas){
    const int x = i%m_nWidth + delta.first;
    const int y = i/m_nWidth + delta.second;

    if(j == y*m_nWidth + x)
      return true;
  } //for

  return false;
} //IsKnightMove

/// Reader function for the first edge.
/// \param src [out] Source vertex for the first edge.
/// \param dest [out] Source vertex for the first edge.

void CRail::GetEdge0(int& src, int& dest){
  src = m_nSrc0;
  dest = m_nDest0;
} //GetEdge0

/// Reader function for the second edge.
/// \param src [out] Source vertex for the second edge.
/// \param dest [out] Source vertex for the second edge.

void CRail::GetEdge1(int& src, int& dest){
  src = m_nSrc1;
  dest = m_nDest1;
} //GetEdge1