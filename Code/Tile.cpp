/// \file Tile.cpp
/// \brief Code and declarations for CTile.

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

#include "Tile.h"

////////////////////////////////////////////////////////////////
// Hard-coded CTile move tables

int CTile::m_nTile6x6[36] = {
  13, 12, 6, 11, 8, 9, 
  14, 3, 0, 22, 2, 15, 
  25, 21, 1, 23, 5, 4, 
  7, 32, 28, 10, 35, 34, 
  20, 33, 30, 31, 17, 16, 
  19, 18, 24, 29, 26, 27
}; //6x6

int CTile::m_nTile8x6[48] = {
  10, 11, 8, 9, 14, 20, 12, 13, 
  18, 24, 25, 21, 2, 3, 31, 5, 
  1, 0, 35, 4, 37, 15, 7, 6, 
  41, 40, 32, 17, 43, 44, 36, 46, 
  42, 16, 28, 45, 19, 47, 23, 22, 
  34, 26, 27, 33, 38, 39, 29, 30
}; //8x6;

int CTile::m_nTile6x8[48]{
  13, 14, 6, 11, 17, 16, 
  19, 3, 0, 5, 2, 15, 
  1, 9, 22, 4, 27, 21, 
  7, 32, 24, 8, 35, 10, 
  37, 12, 34, 23, 20, 40, 
  26, 18, 43, 29, 47, 46, 
  25, 45, 42, 28, 44, 33, 
  31, 30, 36, 41, 38, 39 
}; //6x8

int CTile::m_nTile8x8[64] = {
  10, 18, 8, 13, 21, 11, 23, 22, 
  25, 3, 4, 26, 6, 7, 31, 5, 
  1, 0, 24, 2, 37, 15, 39, 38, 
  9, 42, 43, 12, 34, 14, 20, 46, 
  17, 16, 40, 45, 19, 27, 55, 29, 
  57, 58, 52, 28, 54, 62, 63, 30, 
  33, 32, 56, 36, 35, 59, 60, 61, 
  41, 51, 48, 49, 50, 44, 47, 53
}; //8x8

int CTile::m_nTile10x8[80] = {
  12, 13, 10, 15, 16, 24, 14, 19, 29, 28, 
  22, 3, 4, 5, 2, 7, 8, 9, 6, 38, 
  1, 0, 41, 42, 36, 37, 47, 46, 49, 17, 
  11, 23, 20, 52, 26, 56, 48, 45, 59, 18, 
  21, 60, 50, 35, 25, 33, 54, 39, 27, 68, 
  71, 30, 31, 32, 75, 34, 44, 69, 66, 78, 
  72, 40, 70, 55, 43, 73, 74, 79, 76, 77, 
  51, 63, 53, 61, 62, 67, 64, 65, 57, 58
}; //10x8

int CTile::m_nTile8x10[80]{
  17, 18, 8, 9, 10, 15, 23, 13, 
  25, 24, 0, 26, 6, 3, 4, 30, 
  1, 32, 12, 2, 5, 11, 7, 29, 
  41, 40, 20, 21, 43, 19, 45, 14, 
  49, 16, 51, 50, 42, 47, 28, 22, 
  34, 56, 59, 53, 27, 35, 31, 62, 
  33, 64, 60, 68, 37, 36, 39, 38, 
  73, 72, 52, 44, 75, 78, 79, 46, 
  74, 48, 76, 77, 58, 54, 55, 61, 
  66, 67, 57, 65, 70, 71, 63, 69
}; //8x10

int CTile::m_nTile10x10[100] = {
  21, 20, 10, 15, 12, 13, 14, 19, 16, 17, 
  22, 23, 0, 1, 35, 7, 4, 29, 37, 38, 
  32, 40, 3, 2, 5, 44, 45, 6, 9, 8, 
  11, 50, 51, 25, 46, 47, 24, 56, 59, 18, 
  61, 62, 30, 55, 63, 66, 27, 26, 36, 28, 
  42, 70, 31, 41, 33, 34, 75, 65, 39, 67, 
  52, 80, 54, 71, 43, 73, 74, 48, 49, 77, 
  91, 90, 93, 81, 53, 83, 57, 58, 97, 87, 
  92, 60, 94, 95, 96, 64, 98, 99, 69, 68, 
  82, 72, 84, 85, 86, 76, 88, 89, 79, 78
}; //10x10

int CTile::m_nTile10x12[120]{
  21, 20, 10, 11, 25, 13, 14, 26, 16, 17, 
  31, 30, 0, 1, 22, 36, 4, 29, 37, 7, 
  12, 2, 3, 35, 32, 6, 5, 15, 9, 8, 
  51, 50, 40, 41, 46, 27, 48, 45, 19, 18, 
  52, 60, 61, 24, 23, 66, 65, 55, 56, 28, 
  71, 70, 33, 34, 73, 74, 77, 49, 39, 38, 
  72, 53, 54, 42, 43, 44, 47, 75, 87, 57, 
  91, 90, 80, 81, 62, 94, 84, 89, 59, 58, 
  101, 93, 63, 64, 105, 97, 67, 95, 69, 68, 
  111, 110, 100, 85, 82, 83, 117, 76, 79, 78, 
  112, 113, 114, 115, 116, 86, 118, 119, 96, 88, 
  102, 103, 104, 92, 106, 107, 108, 109, 99, 98
}; //10x12

int CTile::m_nTile12x10[120] = {
  14, 15, 25, 17, 29, 19, 31, 21, 22, 23, 35, 34, 
  2, 3, 24, 5, 6, 7, 41, 9, 45, 11, 47, 46, 
  1, 0, 36, 4, 18, 39, 53, 8, 42, 10, 59, 58, 
  13, 12, 63, 16, 50, 51, 28, 20, 54, 68, 71, 33, 
  62, 26, 27, 76, 38, 43, 40, 30, 70, 32, 83, 82, 
  37, 75, 72, 86, 78, 55, 52, 57, 93, 44, 80, 94, 
  49, 48, 60, 100, 66, 91, 88, 56, 90, 95, 107, 106, 
  74, 110, 61, 64, 65, 112, 67, 116, 69, 79, 119, 118, 
  73, 87, 108, 109, 77, 111, 92, 113, 114, 115, 81, 117, 
  85, 84, 96, 97, 98, 99, 89, 101, 102, 103, 104, 105
}; //12x10

////////////////////////////////////////////////////////////////
// CTile constructor and destructor

/// Create the CBoard instances from the move tables.

CTile::CTile(){
  m_pTile6x6 =   new CBoard(m_nTile6x6,    6, 6); 
  m_pTile6x8 =   new CBoard(m_nTile6x8,    6, 8);

  m_pTile8x6 =   new CBoard(m_nTile8x6,    8, 6); 
  m_pTile8x8 =   new CBoard(m_nTile8x8,    8, 8); 
  m_pTile8x10 =  new CBoard(m_nTile8x10,   8, 10);

  m_pTile10x8 =  new CBoard(m_nTile10x8,  10, 8);  
  m_pTile10x10 = new CBoard(m_nTile10x10, 10, 10); 
  m_pTile10x12 = new CBoard(m_nTile10x12, 10, 12);

  m_pTile12x10 = new CBoard(m_nTile12x10, 12, 10); 
} //constructor

/// Delete all of the CBoard instances created by the constructor.

CTile::~CTile(){    
  delete m_pTile6x6; 
  delete m_pTile8x6; 
  delete m_pTile6x8; 
  delete m_pTile8x8; 
  delete m_pTile10x8; 
  delete m_pTile8x10; 
  delete m_pTile10x10; 
  delete m_pTile10x12; 
  delete m_pTile12x10; 
} //destructor