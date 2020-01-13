/// \file TakefujiLee.cpp
/// \brief Code for the neural network tourney generator CTakefujiLee.

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

#include "TakefujiLee.h"
#include "Helpers.h"
#include "Random.h"
#include "Includes.h"
#include "Defines.h"
#include "Board.h"

extern MoveDeltas g_vecDeltas; ///< Move deltas for all possible knight's moves.
extern std::atomic_bool g_bFinished; ///< Search termination flag.

/// Initialize the neural network.
/// \param w Board width.
/// \param h Board height.
/// \param seed PRNG seed.

CTakefujiLee::CTakefujiLee(int w, int h, int seed):
  CNeuralNet(w*h, seed), m_nWidth(w), m_nHeight(h), m_nSize(w*h)
{ 
  for(int srcy=0; srcy<m_nHeight; srcy++)
    for(int srcx=0; srcx<m_nWidth; srcx++){
      const int src = srcy*m_nWidth + srcx;

      for(auto delta: g_vecDeltas){
        const int destx = srcx + delta.first;

        if(0 <= destx && destx < m_nWidth){
          const int desty = srcy + delta.second;
          
          if(0 <= desty && desty < m_nHeight){
            const int dest = desty*m_nWidth + destx;

            if(src < dest)
              InsertNeuron(src, dest);
          } //if
        } //if
      } //for
    } //for

  Reset();
} //constructor

/// Reset all neuron outputs to zero and all neuron states
/// to a random value.

void CTakefujiLee::Reset(){
  for(CEdge* pEdge: m_vEdgeList){
    CNeuron* pNeuron = (CNeuron*)pEdge;
    pNeuron->SetOutput(m_cRandom.randf() < 0.5f);
    pNeuron->SetState(0);
  } //for

  RandomizeEdgeList();
} //Reset

/// Update all neurons.
/// \return true If the network has stabilized.

bool CTakefujiLee::Update(){
  for(CEdge* pEdge: m_vEdgeList){ //update neuron states
    CNeuron* pNeuron = (CNeuron*)pEdge;
    int newstate = pNeuron->GetState() + 4;

    UINT v0, v1;
    pEdge->GetVertexIndices(v0, v1);

    for(UINT v: {v0, v1}){
      std::vector<CEdge*>* adj = m_pVertexList[v].GetAdjacencyList();
    
      for(CEdge* pEdge2: *adj)
        if(((CNeuron*)pEdge2)->GetOutput())
          newstate -= 1;
    } //for

    pNeuron->SetState(newstate);

    if(newstate > 3)pNeuron->SetOutput(true);
    else if(newstate < 0)pNeuron->SetOutput(false);
  } //for

  return IsStable();
} //Update

/// The network is stable if all neurons are stable. 
/// \return true If all neurons are stable.

bool CTakefujiLee::IsStable(){
  for(CEdge* pEdge: m_vEdgeList)
    if(((CNeuron*)pEdge)->IsStable())
      return false;

  return true;
} //IsStable

/// The neural network may converge to a state in which not all vertices have
/// degree 2, which is a requirement for knight's tours and tourneys.
/// \return true If all vertices have degree 2.

bool CTakefujiLee::HasDegree2(){
  int* degree = new int[m_nNumVerts];

  for(UINT i=0; i<m_nNumVerts; i++)
    degree[i] = 0;

  for(CEdge* pEdge: m_vEdgeList){
    CNeuron* pNeuron = (CNeuron*)pEdge;

    if(pNeuron->GetOutput()){
      UINT i, j;
      pNeuron->GetVertexIndices(i, j);
      degree[i]++;
      degree[j]++;
    } //if
  } //for

  bool bDegree2 = true;

  for(UINT i=0; i<m_nNumVerts && bDegree2; i++)
    bDegree2 = bDegree2 && degree[i] == 2;

  delete [] degree;

  return bDegree2;
} //HasDegree2

/// Generate a tourney.
/// \param b [out] Chessboard.

void CTakefujiLee::Generate(CBoard& b){
  bool bFinished = false;
  bool bStable = false;
  
  while(!bFinished && !g_bFinished){
    Reset();
    bStable = false;

    for(int j=0; j<400 && !bStable; j++)
      bStable = Update();

    bFinished = HasDegree2();
  } //while

  if(bFinished)
    GraphToBoard(b);
} //Generate

/// Assuming the neural network has converged, convert the outputs
/// of its neurons to a move table.
/// \param b [out] Chessboard for the results.

void CTakefujiLee::GraphToBoard(CBoard& b){
  b.Clear();

  for(UINT i=0; i<m_nNumVerts; i++)
    m_pVertexList[i].Mark(false);

  UINT startindex = 0;
  while(startindex < m_nNumVerts  && m_pVertexList[startindex].Marked())
    startindex++;

  while(startindex < m_nNumVerts){
    CVertex* start = &m_pVertexList[startindex];
    CVertex* prev = start;
    prev->Mark();

    CEdge* pEdge = nullptr;
    auto adj = prev->GetAdjacencyList();

    for(auto it=(*adj).begin(); it!=(*adj).end(); it++){
      CNeuron* pNeuron = (CNeuron*)*it;
      if(pNeuron->GetOutput() == 1){
        if(!pNeuron->GetNextVertex(prev)->Marked()){
          pEdge = *it;
          break;
        } //if
      } //if
    } //for

    CVertex* cur = pEdge->GetNextVertex(prev);
    b.InsertUndirectedMove(prev->GetIndex(), (int)cur->GetIndex());

    while(cur != start && !cur->Marked()){
      cur->Mark();

      CEdge* pNextEdge = nullptr;
      auto adj = cur->GetAdjacencyList();

      for(auto it=(*adj).begin(); it!=(*adj).end(); it++){
        CNeuron* pNeuron = (CNeuron*)*it;
        if(pNeuron->GetOutput() == 1 && (*it) != pEdge){
          pNextEdge = *it;
          break;
        } //if
      } //for

      CVertex* next = pNextEdge->GetNextVertex(cur);
      prev = cur; 
      cur = next;
      b.InsertUndirectedMove(prev->GetIndex(), (int)cur->GetIndex());

      pEdge = pNextEdge;
    } //while
    
    while(startindex < m_nNumVerts && m_pVertexList[startindex].Marked())
      startindex++;
  } //while
} //GraphToBoard

/// Get vector of vertices adjacent to a given vertex.
/// \param p Pointer to a vertex.
/// \param v [out] Vector of vertices adjacent to the vertex pointed to by p.

void CTakefujiLee::GetAdjacentVertices(std::vector<CVertex*>& v, CVertex* p){
  v.clear();
  if(p == nullptr)return;

  std::vector<CEdge*>* adjacencylist = p->GetAdjacencyList();

  for(CEdge* pEdge: *adjacencylist){
    CVertex* pNext = pEdge->GetNextVertex(p);
    if(pNext != nullptr)
      v.push_back(pNext);
  } //for
} //GetAdjacentVertices

/// Permute the edge list into pseudorandom order for update.

void CTakefujiLee::RandomizeEdgeList(){
  const size_t n = m_vEdgeList.size();

  for(size_t i=0; i<n; i++){
    const int j = m_cRandom.randn((UINT)i, (UINT)n - 1);
    std::swap(m_vEdgeList[i], m_vEdgeList[j]);
  } //for
} //RandomizeEdgeList