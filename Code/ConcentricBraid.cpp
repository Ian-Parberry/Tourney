/// \file ConcentricBraid.cpp
/// \brief Code for the concentric braided tourney generator CConcentricBraid.

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

#include "ConcentricBraid.h"

/// Create boards for the \f$4 \times 4\f$ and \f$6 \times 6\f$
/// centers of concentric tourneys.

/// \image html BraidedConcentricCenters.png

CConcentricBraid::CConcentricBraid(){ 
  m_pBoard4x4 = new CBoard(m_nMove4x4, 4, 4);
  m_pBoard6x6 = new CBoard(m_nMove6x6, 6, 6);
} //constructor

/// Delete the boards for the \f$4 \times 4\f$ and \f$6 \times 6\f$
/// centers of concentric tourneys created in the constructor.

CConcentricBraid::~CConcentricBraid(){ 
  delete m_pBoard4x4;
  delete m_pBoard6x6;
} //destructor

/// Generate a concentric tourney.
/// \param b [in, out] Chessboard.

void CConcentricBraid::Generate(CBoard& b){
  const int w = b.GetWidth(); //board width
  const int h = b.GetHeight(); //board height
  if((w & 1) || w != h)return; //board must be square with even width
  
  const unsigned limit = (w%4 == 2)? w/2 - 3: w/2 - 2;

  for(unsigned offset=0; offset<limit; offset+=2)
    for(unsigned k=0; k<4; k++){ 
      unsigned i = offset;
      unsigned j = offset + k;

      while(j < w - offset - 2){
        unsigned cur = i*w + j;
        i = (i == offset)? offset + 1: offset; 
        j += 2;
        b.InsertUndirectedMove(cur, i*w + j);
      } //while

      while(i < w - offset - 2){
        unsigned cur = i*w + j;
        i += 2;
        j = (j == w - offset - 1)? w - offset - 2: w - offset - 1; 
        b.InsertUndirectedMove(cur, i*w + j);
      } //while

      while(j >= offset + 2){
        unsigned cur = i*w + j;
        i = (i == w - offset - 1)? w - offset - 2: w - offset - 1; 
        j -= 2;
        b.InsertUndirectedMove(cur, i*w + j);
      } //while

      while(i >= offset + 2){
        unsigned cur = i*w + j;
        i -= 2;
        j = (j == offset)? offset + 1: offset; 
        b.InsertUndirectedMove(cur, i*w + j);
      } //while

      if(i != offset)
        b.InsertUndirectedMove(i*w + j, (i - 1)*w + j + 2);
    } //for

  //generate center, either 4x4 or 6x6

  const int m = 4 + w%4; //width of center, either 4 or 6
  const int offset = (w - m)/2; //offset from top and left

  if(m == 4)
    b.CopyToSubBoard(*m_pBoard4x4, offset, offset);

  else if(m == 6)
    b.CopyToSubBoard(*m_pBoard6x6, offset, offset);
} //GenerateConcentric
