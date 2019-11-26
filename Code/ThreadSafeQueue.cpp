/// \file ThreadSafeQueue.cpp
/// \brief Code for the thread safe queue CThreadSafeQueue.

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

#include "ThreadSafeQueue.h"
#include "Generator.h"

/// Insert an element at the end of the queue.
/// \param element An element.

template <class data> void CThreadSafeQueue<data>::push(const data& element){
  m_mutex.lock();
  m_stdQueue.push(element); 
  m_mutex.unlock();
} //push

/// Delete and return the element at the head of the queue.
/// \param element [out] The element at the head of the queue.
/// \return true If the queue was non-empty.

template <class data> bool CThreadSafeQueue<data>::pop(data& element){
  bool success = false; //true if there was something to delete
  m_mutex.lock();

  if(!m_stdQueue.empty()){ //queue has something in it
    element = m_stdQueue.front(); //get element from front of queue
    m_stdQueue.pop(); //delete from front of queue
    success = true; //success
  } //if

  m_mutex.unlock();
  return success;
} //pop

/// Get the number of things in the queue.
/// \return the number of things in the queue.

template <class data> size_t CThreadSafeQueue<data>::size(){
  return m_stdQueue.size();
} //size

/////////////////////////////////////////////////////////////////////////////

//explicit template instantiations

template class CThreadSafeQueue<CSearchRequest>; ///< Thread safe queue of requests.
template class CThreadSafeQueue<CSearchResult>; ///< Thread safe queue of results.