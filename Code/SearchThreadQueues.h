/// \file SearchThreadQueues.h
/// \brief Header for the search thread queues CSearchThreadQueues.

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

#ifndef __SearchThreadQueues__
#define __SearchThreadQueues__

#include "ThreadSafeQueue.h"
#include "Structs.h"

/// \brief Search thread queues.
///
/// A pair of thread-safe input and output queues for the search threads,
/// These queues are declared static protected because, like *The Highlander*,
/// there can be only one. This monostate design pattern is apparently called 
/// the "Borg idiom" by some members of the Python community, which for some
/// reason I find curiously compelling.

class CSearchThreadQueues{
  protected:
    static CThreadSafeQueue<CSearchRequest> m_cSearchRequest; ///< Search request queue.
    static CThreadSafeQueue<CSearchResult> m_cSearchResult; ///< Search result queue.
}; //CSearchThreadQueues

#endif
