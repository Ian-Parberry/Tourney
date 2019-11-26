/// \file SearchThread.h
/// \brief Header for the search thread CSearchThread.

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

#ifndef __SearchThread__
#define __SearchThread__

#include "Includes.h"

#include "SearchThreadQueues.h"
#include "Random.h"
#include "Helpers.h"

/// \brief Search thread.
///
/// The code that is run in each search thread, consisting of the generation
/// code and an override for operator() so that threads can be created
/// in the approved C++ manner.

class CSearchThread: public CSearchThreadQueues{
  private:
    void Generate(CSearchRequest& request); ///< Generate knight's tour or tourney.

  public:
    void operator()(); ///< The code that gets run by each thread.
}; //CSearchThread

#endif
