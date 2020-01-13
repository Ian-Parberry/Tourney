/// \file BaseBoard.h
/// \brief Header for the base chessboard CBaseBoard.

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

#ifndef __BaseBoard__
#define __BaseBoard__

#include "Includes.h"
#include "Defines.h"
#include "Structs.h"
#include "Helpers.h"

/// \brief Base chessboard.
///
/// The base chessboard provides the basic functionality needed to manipulate
/// knight's tours and contains no new ideas from the paper. It records for 
/// each cell the two cells reachable from there via a knight's move from that
/// cell while on a knight's tour or tourney. It is intended that the moves be
/// knight's moves and that they form a tourney (a cycle cover of the knight's
/// graph).
///
/// A board can be undirected or directed. The preferred state of the board
/// is to be undirected, however, some of the algorithms that operate on
/// knight's tours or tourneys run faster ondirected boards than on undirected
/// ones up to a linear factor. We will use the convention that such algorithms
/// will begin with a undirected board which they will first make directed, and 
/// that they will return an undirected board. There are obvious linear time
/// algorithms for converting a tourney on an undirected board to a tourney on
/// a directed board which maintaining the number of cycles, and vice-versa.
///
/// A directed board has two move tables m_nMove and m_nMove2. A knight's tour
/// or tourney in a directed board can be found by starting at an unvisited
/// cell i, moving to cell j = m_nMove[i], then repeatedly moving to either
/// cell j = m_nMove[j] or m_nMove2[j] (whichever doesn't take us back to the
/// previous cell) until cell i is reached again. For a tourney, repeat this
/// until all cells have been visited. For a closed knight's tour it is 
/// sufficient to do this once.
///
/// An undirected board has only one move table m_nMove, and m_nMove2 is NULL.
/// A knight's tour or tourney in an undirected board can be found by starting
/// at an unvisited cell i, moving to cell j = m_nMove[i], then repeatedly
/// moving to cell j = m_nMove[j] until cell i is reached again. For a tourney,
/// repeat this until all cells have been visited. For a closed knight's tour
/// it is sufficient to do this once.

#include "Random.h"

class CBaseBoard{
  protected:
    CRandom m_cRandom; ///< PRNG.

    UINT m_nWidth = 0; ///< Board width in cells.
    UINT m_nHeight = 0; ///< Board height in cells.
    UINT m_nSize = 0; ///< Board size in cells.

    int* m_nMove = nullptr; ///< Primary move table.
    int* m_nMove2 = nullptr; ///< Secondary move table.

    //helper functions

    bool CellIndexInRange(int index); ///< Index in range test.
    bool InRangeX(int x); ///< X coordinate in range test.
    bool InRangeY(int y); ///< Y coordinate in range test.

    bool IsMove(int i, int j); ///< Move test.
    int GetDest(int i, const MoveDelta& delta); ///< Get destination of move.

    int GetTourneyIds(int*& id); ///< Get tourney identifier for each cell.
    
  public:
    CBaseBoard(); ///< Constructor.
    CBaseBoard(UINT n); ///< Constructor.
    CBaseBoard(UINT w, UINT h); ///< Constructor.
    CBaseBoard(int move[], UINT w, UINT h); ///< Constructor.

    ~CBaseBoard(); ///< Destructor.

    void Clear(); ///< Clear the board of moves.
    
    void MakeDirected(); ///< Make into a directed board.
    void MakeUndirected(); ///< Make into an undirected board.

    bool IsTour(); ///< Knight's tour test.
    bool IsTourney(); ///< Tourney test.
    
    bool IsDirected(); ///< Directed board test.
    bool IsUndirected(); ///< Undirected board test.

    bool IsKnightMove(int i, int j); ///< Knight's move test.
    bool IsUnused(int index); ///< Test for unused cell.
    bool IsUnused(int pos, const MoveDelta& delta); ///< Move is to unused cell.
    bool IsOnBoard(int pos, const MoveDelta& delta); ///< Move stays on board.
    
    int GetAvailableMoveCount(int index); ///< Get number of moves from a cell.

    bool InsertUndirectedMove(int src, int dest); ///< Insert an undirected move.
    bool InsertDirectedMove(int src, int dest); ///< Insert a directed move.
    bool DeleteMove(int src, int dest); ///< Delete a move.

    void Save(std::string& name); ///< Save board to a text file.
    void SaveToSVG(std::string& name); ///< Save to an SVG file.

    int GetMoveIndex(int src, int dest); ///< Get move index.
    
    void CopyToSubBoard(CBaseBoard& b, int x, int y); ///< Copy to sub-board.

    int GetWidth(); ///< Get width.
    int GetHeight(); ///< Get height.
    int GetSize(); ///< Get size.

    int operator[](int index); ///< Get a move from the board.
}; //CBaseBoard

#endif

