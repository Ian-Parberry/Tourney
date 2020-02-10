/// \file Warnsdorff.h
/// \brief Header for Warnsdorff's generator CWarnsdorff.

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

#ifndef __Warnsdorf__
#define __Warnsdorf__

#include "Includes.h"
#include "Random.h"
#include "Helpers.h"
#include "Defines.h"
#include "Board.h"

/// \brief Knight's tour and tourney generator using Warnsdorff's heuristic.
///
/// Warnsdorff's Algorithm is a random walk that optimistically joins up
/// to make one or more cycles aided by Warnsdorff's Heuristic which works.
/// as follows Instead of choosing a random next move, it chooses a move at
/// random from the set of moves that lead to a cell that has remaining the
/// smallest number ofavailable moves to an unvisited square. The tourney
/// version attempts to make a cycle as soon as possible, while the tour
/// version holds off doing so for as long as possible. This results in
/// distinctive patterns in the knight's tours and tourneys generated.
/// For example, the image below shows a tourney with the cycles in color
/// (left) and a knight's tour (right) generated by this algorithm.
///
/// \image html Warnsdorf32.png
///
/// See Conrad, Hindrichs, Morsy, and Wegener, "Wie es dem springer gelang
/// schachbretter beliebiger groesse und zwischen beliebig vorgegebenen anfangs
/// und endfeldern vollstaendig abzuschreiten", Spektrum der Wissenschaft,
/// pages 10-14, 1992, and Conrad, Hindrichs, Morsy, and Wegener, "Solution of
/// the knight's Hamiltonian path problem on chessboards", Discrete Applied
/// Mathematics, 50(2):125-134, 1994.

class CWarnsdorff{
  private:    
    CRandom m_cRandom; ///< PRNG.

    int RandomClosedWalk(CBoard& b, int start); ///< Create closed random walk.

    bool GenerateTour(CBoard& b); ///< Generate a knight's tour.
    bool GenerateTourney(CBoard& b); ///< Generate a tourney.

  public:
    CWarnsdorff(int seed); ///< Constructor.

    void Generate(CBoard& b, CycleType t); ///< Generate a tour or tourney.
}; //CWarnsdorff

#endif