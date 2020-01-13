/// \file ConcentricBraid.h
/// \brief Header for the concentric braided tourney generator CConcentricBraid.

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

#ifndef __Braid__
#define __Braid__

#include "Board.h"
#include "Defines.h"
#include "Structs.h"

/// \brief Concentric braided tourney generator.
/// 
/// Concentric braided tourneys have a \f$4 \times 4\f$ or 
/// \f$6 \times 6\f$ center surrounded by concentric rings of
/// braids. The following image shows the \f$16 \times 16\f$
/// and \f$18 \times 18\f$ concentric braided tourneys.
///
/// \image html BraidedConcentric.png

class CConcentricBraid{
  private:
    int m_nMove4x4[16] = ///< Undirected move table for 4x4 center.\image html bc4x4.png
      {
        6,  7,  4,  5, 
        13, 12, 15, 14, 
         1,  0,  3,  2, 
        10, 11,  8,  9
      }; //m_nMove4x4

    int m_nMove6x6[36] =  
      {
        8, 12,  6,  7, 17,  9, 
        19, 18,  4,  1,  2,  3, 
        25,  0, 22, 11,  5, 28, 
        31, 30, 16, 34, 35, 10, 
        13, 33, 15, 14, 32, 21, 
        26, 20, 24, 29, 23, 27
      }; ///< Undirected move table for 6x6 center. \image html bc6x6.png

    CBoard* m_pBoard4x4 = nullptr; ///< Pointer to 4x4 center.
    CBoard* m_pBoard6x6 = nullptr; ///< Pointer to 6x6 center.

  public:
    CConcentricBraid(); ///< Constructor.
    ~CConcentricBraid(); ///< Destructor.

    void Generate(CBoard& b); ///< Generate a concentric braided tourney.
}; //CConcentricBraid

#endif