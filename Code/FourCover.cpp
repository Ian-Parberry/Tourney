/// \file FourCover.cpp
/// \brief Code for the four-cover tourney generator CFourCover.

// MIT License
//
// Copyright (c) 2020 Ian Parberry
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

#include "FourCover.h"

/// Generate a 4-cycle, that is, a cycle of length 4, given four cell indices
/// which represent the four vertices in the cycle. Assumes that the
/// vertices are separated by knight's moves.
/// \param b [in, out] Chessboard.
/// \param v Array of four vertices.

void CFourCover::Generate4Cycle(CBoard& b, int v[4]){
  b.InsertUndirectedMove(v[0], v[1]);
  b.InsertUndirectedMove(v[1], v[2]);
  b.InsertUndirectedMove(v[2], v[3]);
  b.InsertUndirectedMove(v[3], v[0]);
} //Generate4Cycle

/// Generate a four-cover tourney. Assumes that the board has both width and
/// height divisible by 4. Does nothing otherwise.
/// \param b [in, out] Chessboard.

void CFourCover::Generate(CBoard& b){
  const int w = b.GetWidth(); //board width
  const int h = b.GetHeight(); //board height

  if(w%4 != 0 || h%4 != 0)return; //width and height must be divisible by 4

  for(int i=0; i<h; i+=4)
    for(int j=0; j<w; j+=4){
      int v[4]; //four vertices in the cycle

      v[0] = i*w + j;
      v[1] = v[0] + w + 2;
      v[2] = v[1] + 2*w + 1;
      v[3] = v[2] - w - 2;

      Generate4Cycle(b, v); //first edge
      
      v[0]++;
      v[1] = v[0] + w + 2;
      v[2] = v[1] + 2*w - 1;
      v[3] = v[2] - w - 2;

      Generate4Cycle(b, v); //second edge 
      
      v[0]++;
      v[1] = v[0] + 2*w + 1;
      v[2] = v[1] + w - 2;
      v[3] = v[2] - 2*w - 1;

      Generate4Cycle(b, v); //third edge 
      
      v[0]++;
      v[1] = v[0] + 2*w - 1;
      v[2] = v[1] + w - 2;
      v[3] = v[2] - 2*w + 1;

      Generate4Cycle(b, v); //fourth edge 
    } //for
} //Generate
