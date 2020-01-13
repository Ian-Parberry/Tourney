/// \file NeuralNet.cpp
/// \brief Code for CNeuralNet and its edges CNeuron.

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

#include "NeuralNet.h"

////////////////////////////////////////////////////////////////
// CNeuron functions.

/// Creates  neuron between two vertices.
/// \param p0 Pointer to vertex incident with neuron.
/// \param p1 Pointer to vertex incident with neuron.
/// \param index Index of neuron.

CNeuron::CNeuron(CVertex* p0, CVertex* p1, UINT index):
  CEdge(p0, p1, index){
} //constructor

/// Reader function for state.
/// \return State.

int CNeuron::GetState(){
  return m_nState;
} //GetState

/// A neuron is stable if its state hasn't changed since the last
/// time it was updated.
/// \return true if stable.

bool CNeuron::IsStable(){
  return m_nOldState == m_nState;
} //GetOldState

/// Set state.
/// \param n New state.

void CNeuron::SetState(int n){
  m_nOldState = m_nState; 
  m_nState = n;
} //SetState

/// Reader function for output.
/// \return Output.

bool CNeuron::GetOutput(){
  return m_bOutput;
} //GetOutput

/// Set output.
/// \param b New output.

void CNeuron::SetOutput(bool b){
  m_bOutput = b;
} //SetOutput

////////////////////////////////////////////////////////////////
// CNeuralNet functions.

/// Allocate space for and initialize the vertex list by calling 
/// CGraph constructor to do the actual work.
/// \param n Number of vertices.
/// \param seed PRNG seed.

CNeuralNet::CNeuralNet(UINT  n, int seed): CGraph(n){
  ::srand(seed); //seed the default PRNG
  m_cRandom.srand(); //seed our PRNG
} //constructor

/// This is the equivalent of CGraph::InsertEdge for CNeuralNet.
/// Insert a neuron between two vertices by appending an instance 
/// of CNeuron at the end of the edge list and appending the index
/// of that neuron (which is the size of the edge list before insertion)
/// to the incidence lists of the vertices. Self-edges and vertices
/// out of range are ignored.
/// \param i The vertex at one end of the edge to be inserted.
/// \param j The vertex at the other end of the edge to be inserted.

void CNeuralNet::InsertNeuron(UINT i, UINT j){
  if(i >= m_nNumVerts || j >= m_nNumVerts || i == j)return; //bail out

  CVertex* p0 = &m_pVertexList[i]; //first vertex
  CVertex* p1 = &m_pVertexList[j]; //second vertex

  CNeuron* pEdge = new CNeuron(p0, p1, m_nNumEdges++); //edge between them

  m_vEdgeList.push_back(pEdge); //append edge to edge list
  p0->InsertAdjacency(pEdge); //append edge to adjacency list of first vertex
  p1->InsertAdjacency(pEdge); //append edge to adjacency list of second vertex
} //InsertNeuron
