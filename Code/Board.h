/// \file Board.h
/// \brief Header for the chessboard CBoard.

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

#ifndef __Board__
#define __Board__

#include "Includes.h"
#include "Structs.h"
#include "Helpers.h"
#include "Rail.h"
#include "BaseBoard.h"


/// \brief Chessboard.
///
/// CBoard adds to CBaseBoard the additional functionality needed to shatter,
/// join, and obfuscate tourneys. It contains an implementation of the new
/// algorithms presented in the paper.

class CBoard: public CBaseBoard{
  private:
    void FindRails(std::vector<CRail>& rails); ///< Find all rails.
    void Switch(CRail& r); ///< Switch a rail.

    bool IsRail(int s0, int d0, int s1, int d1); ///< Rail test.
    bool IsRail(CRail& r); ///< Rail test.

    bool Join(); ///< Join cycles to reduce tourney size.
    
  public:
    CBoard(); ///< Constructor.
    CBoard(UINT n); ///< Constructor.
    CBoard(UINT w, UINT h); ///< Constructor.
    CBoard(int move[], UINT w, UINT h); ///< Constructor.

    void Shatter(); ///< Shatter tourneys into more tourneys.
    void JoinUntilTour(); ///< Join cycles to reduce tourney size.

    void Obfuscate(); ///< Obfuscate function.
}; //CBoard

#endif

