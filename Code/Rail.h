/// \file Rail.h
/// \brief Header for CRail.

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

#ifndef __Rail__
#define __Rail__

#include "Defines.h"

/// \brief A rail.
///
/// A rail consists of two knight's moves in which the pair of source
/// vertices and the pair of destination vertices are both separated by
/// a knight's move. There are 12 types of rail, shown in the image below
/// (the rail edges are shown in black and the knight's moves between their
/// end points are shown in gray).
///
/// \image html rails.png

class CRail{
  private:
    int m_nSrc0 = UNUSED; ///< Index of cell at one end of the first edge.
    int m_nDest0 = UNUSED; ///< Index of cell at the other end of the first edge.
  
    int m_nSrc1 = UNUSED; ///< Index of cell at one end of the second edge.
    int m_nDest1 = UNUSED; ///< Index of cell at the other end of the second edge.

    int m_nWidth = 0; ///< Width of chessboard.

    bool IsKnightMove(int i, int j); ///< Knight's move test.

  public:
    CRail(int src0, int dest0, int src1, int dest1, unsigned w); ///< Constructor.

    void GetEdge0(int& src, int& dest); ///< Get first edge.
    void GetEdge1(int& src, int& dest); ///< Get second edge.
  }; //CRail

#endif
