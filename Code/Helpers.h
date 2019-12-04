/// \file Helpers.h
/// \brief Header for helper functions.

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

#ifndef __Helpers__
#define __Helpers__

#include "Includes.h"
#include "Defines.h"
#include "Structs.h"

#if !defined(_MSC_VER)
  void fopen_s(FILE** stream, const char* name, const char* fmt); ///< fopen_s for *NIX.
  
  #define fprintf_s fprintf
  #define sprintf_s sprintf
#endif

using MoveDelta = std::pair<int, int>; ///< Move delta for a knight's move.
using MoveDeltas = std::vector<MoveDelta>; ///< Move deltas for knight's moves.

std::string MakeFileNameBase(const CTourneyDesc& t, int w=-1); ///< Make file name base.
std::string NumString(float x); ///< Make string from number.

#endif