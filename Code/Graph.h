/// \file Graph.h
/// \brief Header for the graph CGraph and its vertices CVertex and edges CEdge.

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

#ifndef __Graph__
#define __Graph__

class CVertex; //forward declaration

/// \brief Graph edge.
///
/// An edge consists of pointers to two vertices, an index into the
/// edge list, and a flag that indicates whether the edge is marked or not.
/// There are also some neuron-related values for when the graph is used
/// to define a neural network.

class CEdge{
  protected:
    CVertex* m_pVertex0 = nullptr; ///< Vertex at one end of the edge,
    CVertex* m_pVertex1 = nullptr; ///< Vertex at the other end of the edge.

    UINT m_nIndex = 0; ///< Index into edge list.
    bool m_bMarked = false; ///< Mark flag.

  public:
    CEdge(CVertex* p0, CVertex* p1, UINT index); ///< Constructor.

    CVertex* GetNextVertex(CVertex* p); ///< Get vertex at other end of edge.

    UINT GetIndex(); ///< Get index;

    void Mark(bool b=true); ///< Mark or unmark.
    bool Marked(); ///< Get mark.

    void GetVertexIndices(UINT& i0, UINT& i1); ///< Get vertex indices.
}; //CEdge

//////////////////////////////////////////////////////////////////////////

/// \brief Graph vertex.
///
/// A vertex consists of an adjacency list, an index into
/// the vertex list and a flag that indicates
/// whether the vertex is marked or not.

class CVertex{
  private:
    std::vector<CEdge*> m_vAdjacencyList; ///< Adjacency list.

    UINT m_nIndex = 0; ///< Index into vertex list.
    bool m_bMarked = false; ///< Mark flag.

  public:
    CVertex(); ///< Constructor.

    void InsertAdjacency(CEdge* pEdge); ///< Add an edge to the edge list.

    void SetIndex(const UINT n); ///< Set index.
    UINT GetIndex(); ///< Get index;

    void Mark(bool b=true); ///< Mark or unmark.
    bool Marked(); ///< Get mark.

    std::vector<CEdge*>* GetAdjacencyList(); ///< Get adjacency list.
    size_t GetDegree(); ///< Get degree.
}; //CVertex

//////////////////////////////////////////////////////////////////////////

/// \brief Undirected multi-graph.
///
/// A multi-graph stored as a list of edges and an incidence list that
/// consists of a list of indices into the edge list for each vertex
/// consisting of the indices of the edges incident with it. The term
/// *multi-graph* means that there can be multiple edges between two
/// vertices.

class CGraph{
  protected:
    std::vector<CEdge*> m_vEdgeList; ///< Edge list.
    UINT m_nNumEdges = 0; ///< Number of edges.

    CVertex* m_pVertexList = nullptr; ///< Vertex list.
    UINT m_nNumVerts = 0; ///< Number of vertices.

    std::queue<CVertex*> m_qBFSQueue; ///< Queue for breadth-first search.

    CRandom m_cRandom; ///< Random number generator.
    
  public:
    CGraph(const UINT n); ///< Constructor.
    ~CGraph(); ///< Destructor.

    void InsertEdge(const UINT i, const UINT j); ///< Insert an edge.
    void BFSF(std::vector<UINT>& result); ///< Breadth-first spanning forest.

    void PrintGraph(); ///< Print the graph to a text file.
}; //CGraph

#endif
