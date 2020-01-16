/// \file Input.h
/// \brief Header for input functions.

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

#ifndef __Input__
#define __Input__

#include "Defines.h"
#include "Structs.h"

bool ReadUnsigned(UINT& n, Parity parity, UINT lo); ///< Read a UINT from stdin.

bool ReadBoardSize(UINT& n, const CTourneyDesc& t); ///< Read board size from stdin.
bool ReadGeneratorType(GeneratorType& t); ///< Read the generator type from stdin.
bool ReadCycleType(CycleType& t); ///< Read the cycle type from stdin.
bool ReadObfuscate(bool& obfuscate); ///< Read obfuscate status from stdin.
bool ReadTask(Task& t); ///< Read the task from stdin.

#endif
