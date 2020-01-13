/// \file Structs.h
/// \brief Header for the structs CSearchRequest, CSearchResult, and CRect.

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

#ifndef __Structs__
#define __Structs__

#include "Includes.h"
#include "Defines.h"

class CBoard; //forward declaration

/////////////////////////////////////////////////////////////////////////

/// \brief Tourney descriptor.
///
/// Describes the characteristics of a tourney.

struct CTourneyDesc{
  GeneratorType m_eGenerator = GeneratorType::Unknown; ///< Generator type.
  CycleType m_eCycle =  CycleType::Unknown; ///< Cycle type.
  bool m_bBlur = false; ///< Whether to blur.

  CTourneyDesc(GeneratorType gen, CycleType c, bool blur=false); ///< Constructor.
  CTourneyDesc(); ///< Default constructor.
}; //CTourneyDesc

/////////////////////////////////////////////////////////////////////////

/// \brief Search request.
///
/// The information needed by a search thread.

struct CSearchRequest{
  CTourneyDesc m_cTourneyDesc; ///< Tourney descriptor.

  int m_nWidth = 0; ///< Board width.
  int m_nHeight = 0; ///< Board height.
  int m_nSize = 0; ///< Board size.

  bool m_bDiscard = false; ///< Discard result.

  int m_nSeed = 0; ///< PRNG seed.

  CSearchRequest(const CTourneyDesc& t, int w, int h, int s); ///< Constructor.
  CSearchRequest(); ///< Default constructor.
}; //CSearchRequest

/////////////////////////////////////////////////////////////////////////

/// \brief Search result.
///
/// The information found by a search thread.

struct CSearchResult{
  CBoard* m_pBoard = nullptr; ///< Pointer to chessboard.

  CTourneyDesc m_cTourneyDesc; ///< Tourney descriptor.

  int m_nWidth = 0; ///< Board width.
  int m_nHeight = 0; ///< Board height.
  int m_nSize = 0; ///< Board size.

  UINT64 m_nSingleMove[8] = {0}; ///< Single move count.
  UINT64 m_nDoubleMove[8] = {0}; ///< Double move count.
    
  CSearchResult(CBoard* b, const CTourneyDesc& t); ///< Constructor.
  CSearchResult(); ///< Default constructor.
}; //CSearchResult

/////////////////////////////////////////////////////////////////////////

/// \brief Rectangle.
///
/// A rectangle consists of the coordinates of its left, right,
/// top, and bottom. Yes, I know that the Windows API has the RECT
/// structure, but this will be cross-platform.

struct CRect{
  int m_nLeft = -1; ///< Left-most horizontal co-ordinate.
  int m_nRight = -1; ///< Right-most horizontal co-ordinate.
  
  int m_nTop = -1; ///< Top-most vertical co-ordinate.
  int m_nBottom = -1; ///< Bottom-most vertical co-ordinate.

  CRect(int left, int right, int top, int bottom); ///< Constructor.
}; //CRect

#endif
