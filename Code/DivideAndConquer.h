/// \file DivideAndConquer.h
/// \brief Header for the divide-and conquer generator CDivideAndConquer.

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

#ifndef __DivideAndConquer__
#define __DivideAndConquer__

#include "Board.h"
#include "Defines.h"
#include "Tile.h"
#include "Structs.h"

/// \brief Divide-and-conquer knight's tour and tourney generator.
///
/// This divide-and-conquer algorithm used in this class was invented by me,
/// the creator of this software, back in the 1990s. For more details,
/// see Parberry, "An efficient algorithm for the knight's tour problem",
/// Discrete Applied Mathematics, 73:251-260, 1997. It was also described in
/// Parberry, "Scalability of a neural network for the knight's tour problem",
/// Neurocomputing, 12:19-34, 1996. Loosely speaking, you divide a square
/// chessboard into four quadrants, recursively create a closed knight's tour
/// in each quadrant, and then join the four knight's tours by deleting four
/// carefully chosen moves and inserting four to replace them.  
///
/// \image html Tiled.png

class CDivideAndConquer: public CTile{
  private:
    int Split(int left, int right); ///< Split coordinates nearly in half.
    void Join(CBoard& b, int midx, int midy); ///< Join 4 sub-boards.
    void Generate(CBoard& b, CycleType t, const CRect& rect); ///< Recursion.
    void GenerateBaseCase(CBoard& b, const CRect& rect); ///< Base of recursion.

  public:
    CDivideAndConquer(); ///< Constructor.

    void Generate(CBoard& b, CycleType t); ///< Generate a tour or tourney.
}; //CDivideAndConquer

#endif