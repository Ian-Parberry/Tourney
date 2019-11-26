/// \file Structs.cpp
/// \brief Code for the structs CSearchRequest, CSearchResult, and CRect.

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

#include "Structs.h"
#include "Helpers.h"

extern MoveDeltas g_vecDeltas; ///< Move deltas for all possible knight's moves. 

/////////////////////////////////////////////////////////////////////////
// CTourneyDesc constructors.

/// The tourney descriptor constructor.
/// \param gen Generator type.
/// \param c Cycle type.
/// \param blur True to blur (defaults to false).

CTourneyDesc::CTourneyDesc(GeneratorType gen, CycleType c, bool blur):
  m_eGenerator(gen), m_eCycle(c), m_bBlur(blur){
} //CTourneyDesc

/// The default tourney descriptor constructor. 

CTourneyDesc::CTourneyDesc(){
} //constructor

///////////////////////////////////////////////////////////////////////////
// CSearchRequest constructors.

/// The search request constructor.
/// \param t Tourney descriptor.
/// \param w Board width.
/// \param h Board height.

CSearchRequest::CSearchRequest(const CTourneyDesc& t, int w, int h):
  m_cTourneyDesc(t), m_nWidth(w), m_nHeight(h), m_nSize(w*h){
} //constructor

/// The default search request constructor.

CSearchRequest::CSearchRequest(){
} //constructor

///////////////////////////////////////////////////////////////////////////
// CSearchResult constructors.

/// The search result constructor.
/// \param b Chessboard.
/// \param t Tourney descriptor.

CSearchResult::CSearchResult(CBoard* b, const CTourneyDesc& t):
  m_pBoard(b), m_cTourneyDesc(t){
} //constructor

/// The default search result constructor. 

CSearchResult::CSearchResult(){
} //constructor

/////////////////////////////////////////////////////////////////////////
// CRect constructors.

/// The rectangle constructor.
/// \param left Left-most horizontal co-ordinate.
/// \param right Right-most horizontal co-ordinate.
/// \param top Top-most vertical co-ordinate.
/// \param bottom Bottom-most vertical co-ordinate.

CRect::CRect(int left, int right, int top, int bottom):
  m_nLeft(left), m_nRight(right), m_nTop(top), m_nBottom(bottom){
} //constructor