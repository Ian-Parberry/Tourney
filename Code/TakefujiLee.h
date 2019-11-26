/// \file TakefujiLee.h
/// \brief Header for the neural network tourney generator CTakefujiLee.

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

#ifndef __TakefujiLee__
#define __TakefujiLee__

#include "NeuralNet.h"
#include "Includes.h"
#include "Board.h"

/// \brief Neural network tourney generator.
///
/// Neural network tourney generator by Takefuji and Lee, see Takefuji and Lee,
/// "Neural network computing for knight's tour problems", Neurocomputing, 
/// 4(5):249--254, 1992. Unfortunately the paper doesn't completely specify
/// the method used, so I've taken best guesses in a couple of places. This
/// method is slow at generating closed knight's tours (for more details, see
/// Parberry, "Scalability of a neural network for the knight's tour problem",
/// Neurocomputing, 12:19-34, 1996), but it's much faster at generating 
/// tourneys. It's essentially just a Hopfield network with a custom update
/// function, so convergence is guaranteed.
///
/// \image html Takefuji-Lee.png

class CTakefujiLee: public CNeuralNet{
  private:
    int m_nWidth = 0; ///< Board width.
    int m_nHeight = 0; ///< Board height.
    int m_nSize = 0; ///< Board size.

    CRandom m_cRandom; ///< PRNG.

    bool Update(); ///< Update all neurons.
    void GetAdjacentVertices(std::vector<CVertex*>& v, CVertex* p); ///< Get adjacent vertices.
    bool IsStable(); ///< Stability test.
    bool HasDegree2(); ///< Degree test.
    void Reset(); ///< Reset.
    void RandomizeEdgeList(); ///< Randomize the edge list.
    void GraphToBoard(CBoard& b); ///< Convert graph to board.

  public:
    CTakefujiLee(const int w, const int h); ///< Constructor.

    void Generate(CBoard& b); ///< Generate a tourney.
}; //CTakefujiLee

#endif