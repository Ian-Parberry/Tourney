/// \file NeuralNet.h
/// \brief Header for CNeuralNet and its edges CNeuron.

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

#include "Includes.h"
#include "Random.h"

#ifndef __NeuralNet__
#define __NeuralNet__

#include "Graph.h"

/// \brief Neuron in a Hopfield network.
///
/// A neuron in a Hopfield network is a graph edge with
/// the extra properties of *state* and *output*. The state
/// from the previous update is also stored in order to
/// check for convergence.

class CNeuron: public CEdge{
  private:
    int m_nState = 0; ///< Neuron state.
    int m_nOldState = 0; ///< Old neuron state.
    int m_bOutput = false; ///< Neuron output.

  public:
    CNeuron(CVertex* p0, CVertex* p1, unsigned int index); ///< Constructor.

    int GetState(); ///< Get neuron state.
    bool IsStable(); ///< Stability test.
    void SetState(int n); ///< Get neuron state.

    bool GetOutput(); ///< Get neuron output.
    void SetOutput(bool b); ///< Set neuron output.
}; //CNeuron

///////////////////////////////////////////////////////////////////////////

/// \brief Hopfield network.
///
/// A Hopfield network is a graph in which the edges represent neurons.

class CNeuralNet: public CGraph{
  public:
    CNeuralNet(unsigned n, int seed); ///< Constructor.
    void InsertNeuron(unsigned i, unsigned j); ///< Insert a neuron.
}; //CNeuralNet

#endif
