/// \file Graph.cpp
/// \brief Code for the graph CGraph and its vertices CVertex and edges CEdge.

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

#include "Graph.h"
#include "Random.h"
#include "Helpers.h"

////////////////////////////////////////////////////////////////
// CEdge functions.

/// The edge constructor creates an edge between two vertices.
/// \param p0 Pointer to vertex incident with edge.
/// \param p1 Pointer to vertex incident with edge.
/// \param index Index of edge.

CEdge::CEdge(CVertex* p0, CVertex* p1, UINT index):
  m_pVertex0(p0), m_pVertex1(p1), m_nIndex(index){
} //constructor

/// Get vertex at other end of edge from a vertex. Returns nullptr if this edge
/// is not incident on the given vertex.
/// \param p Pointer to a vertex.
/// \return Pointer to the vertex at the other end of the edge.

CVertex* CEdge::GetNextVertex(CVertex* p){
  if(m_pVertex0 == p)return m_pVertex1;
  else if(m_pVertex1 == p)return m_pVertex0;
  else return nullptr;
} //GetNextVertex

/// Reader function for the index.
/// \return The index of this edge in the edge list.

UINT CEdge::GetIndex(){
  return m_nIndex;
} //GetIndex

/// Set or unset the mark flag.
/// \param b true to set mark flag (default), false to unset it

void CEdge::Mark(bool b){
  m_bMarked = b;
} //Mark

/// Reader function for the mark flag.
/// \return true if marked, false if unmarked.

bool CEdge::Marked(){
  return m_bMarked;
} //Marked

/// Get a pair of vertex indices.
/// \param i0 [out] Index of a vertex incident with this edge.
/// \param i1 [out] Index of the other vertex incident with this edge.

void CEdge::GetVertexIndices(UINT& i0, UINT& i1){
  i0 = m_pVertex0->GetIndex();
  i1 = m_pVertex1->GetIndex();
} //GetVertexIndices

////////////////////////////////////////////////////////////////
// CVertex functions.

CVertex::CVertex(){
} //constructor

/// Add an edge to the edge list.
/// \param pEdge Pointer to an edge.

void CVertex::InsertAdjacency(CEdge* pEdge){
  if(pEdge == nullptr)return;
  m_vAdjacencyList.push_back(pEdge);
} //InsertAdjacency

/// Set index.
/// \param n The index of this vertex in the vertex list.

void CVertex::SetIndex(const UINT n){
  m_nIndex = n;
} //SetIndex

/// Reader function for the index.
/// \return The index of this vertex in the vertex vertex.

UINT CVertex::GetIndex(){
  return m_nIndex;
} //GetIndex

/// Set or unset the mark flag.
/// \param b true to set mark flag (default), false to unset it

void CVertex::Mark(bool b){
  m_bMarked = b;
} //Mark

/// Reader function for the mark flag.
/// \return true if marked, false if unmarked.

bool CVertex::Marked(){
  return m_bMarked;
} //Marked

/// Get a pointer to the adjacency list.
/// \return A pointer to the adjacency list.

std::vector<CEdge*>* CVertex::GetAdjacencyList(){
  return &m_vAdjacencyList;
} //GetAdjacencyList

/// Get the degree, that is, the number of adjacent vertices.
/// \return The degree.

size_t CVertex::GetDegree(){
  return m_vAdjacencyList.size();
} //GetDegree

////////////////////////////////////////////////////////////////
// CGraph functions.

/// Allocate space for and initialize the vertex list.
/// \param n Number of vertices.

CGraph::CGraph(const UINT n):
  m_nNumVerts(n)
{
  m_pVertexList = new CVertex[n];

  for(UINT i=0; i<n; i++)
    m_pVertexList[i].SetIndex(i);

  m_cRandom.srand();
} //constructor

/// Free up space used in incidence list and edge list.

CGraph::~CGraph(){
  delete [] m_pVertexList;

  for(auto p: m_vEdgeList)
    delete p;
} //destructor

/// Insert an edge between two vertices by appending an instance of CEdge
/// at the end of the edge list and appending the index of that edge
/// (which is the size of the edge list before insertion) to the incidence
/// lists of the vertices. Self-edges and vertices out of range are ignored.
/// \param i The vertex at one end of the edge to be inserted.
/// \param j The vertex at the other end of the edge to be inserted.

void CGraph::InsertEdge(const UINT i, const UINT j){
  if(i >= m_nNumVerts || j >= m_nNumVerts || i == j)return; //bail out

  CVertex* p0 = &m_pVertexList[i];
  CVertex* p1 = &m_pVertexList[j];

  CEdge* pEdge = new CEdge(p0, p1, m_nNumEdges++);

  m_vEdgeList.push_back(pEdge); //append edge to edge list
  p0->InsertAdjacency(pEdge); //append edge to adjacency list of first vertex
  p1->InsertAdjacency(pEdge); //append edge to adjacency list of second vertex
} //InsertEdge

/// Find a random breadth-first spanning forest (BFSF).
/// \param [out] result A vector of indices of the edges in a BFSF.
/// \return Number of trees in the spanning forest.

UINT CGraph::BFSF(std::vector<UINT>& result){
  UINT numtrees = 0; //number of trees in spanning forest

  for(UINT i=0; i<m_nNumVerts; i++){
    if(!m_pVertexList[i].Marked()){
      m_qBFSQueue.push(&m_pVertexList[i]);
      m_pVertexList[i].Mark();

      while(!m_qBFSQueue.empty()){
        CVertex* current = m_qBFSQueue.front(); //get next queue element
        m_qBFSQueue.pop(); //remove from queue

        std::vector<CEdge*>* pAdjList = current->GetAdjacencyList(); //shorthand

        //random permutation of adjacency list

        const int n = (int)pAdjList->size();
        auto& adjacencylist = *pAdjList;

        for(int i=0; i<n-1; i++)
          std::swap(adjacencylist[i], adjacencylist[m_cRandom.randn(i, n-1)]);
    
        for(CEdge* pEdge: *pAdjList){ //for edges incident with current vertex
          CVertex* next = pEdge->GetNextVertex(current); //vertex at other end

          if(next != nullptr && !next->Marked()){ //if next vertex is unvisited
            result.push_back(pEdge->GetIndex()); //append edge index to  result
            m_qBFSQueue.push(next); //recurse on next vertex 
            next->Mark();
          } //if
        } //for
      } //while
    } //if

    numtrees++; //one more tree in the forest
  } //for

  //PrintGraph();

  return numtrees;
} //BFSF

/// Print the graph to graph.txt, used to get the example in the paper.

void CGraph::PrintGraph(){
  FILE* output = nullptr;
  fopen_s(&output, "graph.txt", "wt");

  if(output != 0){
    fprintf_s(output, "%d vertices, %d edges\n", m_nNumVerts, m_nNumEdges);

    for(CEdge* p: m_vEdgeList){
      UINT i, j;
      p->GetVertexIndices(i, j);
      fprintf(output, "(%u, %u)\n", i, j);
    } //for

    fclose(output);
  } //if
} //PrintGraph
