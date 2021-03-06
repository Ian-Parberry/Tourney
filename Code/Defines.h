/// \file Defines.h
/// \brief Defines, enumerated types, and typedefs.

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

#ifndef __Defines__
#define __Defines__

#include "Includes.h"

#define UNUSED -1 ///< Contents of unused square on the chessboard.

#define sqr(x) ((x)*(x)) ///< Squaring function.

/////////////////////////////////////////////////////////////////////////

#if !defined(_MSC_VER) //*nix 
  typedef uint64_t UINT64; ///< Typedef of UINT64 for *NIX.
#endif

/////////////////////////////////////////////////////////////////////////

/// \brief Generator type.
///
/// Tourney generation algorithm.

enum class GeneratorType{
  Unknown, Warnsdorff, TakefujiLee, DivideAndConquer, ConcentricBraid,
  FourCover
}; //GeneratorType

/////////////////////////////////////////////////////////////////////////

/// \brief Cycle type.
///
/// Type of cycles in tourney.

enum class CycleType{
  Unknown, Tour, Tourney, TourFromTourney
}; //CycleType

/////////////////////////////////////////////////////////////////////////

/// \brief Task type.
///
/// Type of task that this program can perform.

enum class Task{
  Unknown, Generate, Measure, Time
}; //Task

/////////////////////////////////////////////////////////////////////////

/// \brief Parity.
///
/// Even or odd parity, or don't care at all.

enum class Parity{
  DontCare, Even, Odd, ZeroMod4
}; //Task

/////////////////////////////////////////////////////////////////////////

typedef unsigned int UINT; ///< Abbreviation for unsigned integer.

#endif