/// \file Tile.h
/// \brief Header for CTile.

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

#if !defined(__Tile__)
#define __Tile__

#include "Board.h"

/// \brief Base tours for the divide-and-conquer generation algorithm.
///
/// These are the closed knight's tours at the base of the recursive generation
/// algorithm in CDivideAndConquer. See I. Parberry, "An efficient algorithm
/// for the knight's tour problem", Discrete Applied Mathematics, 73:251-260,
/// 1997. There are hard-coded move tables for each of the tiles, but it's more
/// convenient to have them in board form in the rest of the program.
///
/// \image html Tiles.png

class CTile{
  private:  
    static int m_nTile6x6  [36]; ///<  6x6 move table.\image html tile6x6.png
    static int m_nTile6x8  [48]; ///<  6x8 move table.\image html tile6x8.png

    static int m_nTile8x6  [48]; ///<  8x6 move table.\image html tile8x6.png
    static int m_nTile8x8  [64]; ///<  8x8 move table.\image html tile8x8.png
    static int m_nTile8x10 [80]; ///<  8x10 move table.\image html tile8x10.png

    static int m_nTile10x8 [80];  ///< 10x8 move table.\image html tile10x8.png
    static int m_nTile10x10[100]; ///< 10x10 move table.\image html tile10x10.png
    static int m_nTile10x12[120]; ///< 10x12 move table.\image html tile10x12.png

    static int m_nTile12x10[120]; ///< 12x10 move table.\image html tile12x10.png 

  protected:   
    CBoard* m_pTile6x6   = nullptr; ///< Pointer to a  6x6 chessboard. 
    CBoard* m_pTile6x8   = nullptr; ///< Pointer to a  6x8 chessboard.

    CBoard* m_pTile8x6   = nullptr; ///< Pointer to an 8x6 chessboard. 
    CBoard* m_pTile8x8   = nullptr; ///< Pointer to an 8x8 chessboard. 
    CBoard* m_pTile8x10  = nullptr; ///< Pointer to an 8x10 chessboard.

    CBoard* m_pTile10x8  = nullptr; ///< Pointer to a 10x8 chessboard. 
    CBoard* m_pTile10x10 = nullptr; ///< Pointer to a 10x10 chessboard. 
    CBoard* m_pTile10x12 = nullptr; ///< Pointer to a 10x12 chessboard.

    CBoard* m_pTile12x10 = nullptr; ///< Pointer to a 12x10 chessboard. 

  public:
    CTile(); ///< Constructor.
    ~CTile(); ///< Destructor.
}; //CTile

#endif
