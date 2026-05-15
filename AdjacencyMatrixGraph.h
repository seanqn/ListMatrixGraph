#ifndef ADJACENCYMATRIXGRAPH_H
#define ADJACENCYMATRIXGRAPH_H

#include "DirectedGraph.h"
#include <unordered_map>

class AdjacencyMatrixGraph : public DirectedGraph {
protected:
   std::vector<Vertex*> vertices;
   
   // If matrixRows[X][Y] is true, then an edge exists from vertices[X] to
   // vertices[Y]
   // vertices and matrixRows should have corresponding indices

   // matrix does not inherit AdjacencyListVertex, so no adjacent vector member
   // matrixRows will still "store" edges and can be used to return direct edges
   std::vector<std::vector<bool>> matrixRows;

   // resolves vertices to their respective index in the matrix
   // indices are inserted when the matrix is updated
   std::unordered_map<Vertex*, int> matrixIndexMap;
   
private:
   void populateMatrix(int vertexIndex) {
      std::vector<bool> rowVector;
      // vertex index is a unique int that is both its row and column

      // first addition is a 1x1 matrix, avoid traversal
      if (vertexIndex == 0) {
        rowVector = {false};
        matrixRows.push_back(rowVector);
        return;
      }

      //   matrixRows.push_back(rowVector);
      
      // add new column of (vertexIndex)nth index to previous rows
      for (int row = 0; row < vertexIndex; ++row) {
        matrixRows.at(row).push_back(false);
      }
    
      // populate new row at vertexIndex
      for (int col = 0; col <= vertexIndex; ++col) {
        rowVector.push_back(false);
        // matrixRows.at(vertexIndex).push_back(false);
      }

      matrixRows.push_back(rowVector);
   }

   // testing purposes
   void printMatrix() {
      for (int row = 0; row < matrixRows.size(); ++row) {
        std::cout << "[";
        for (int col = 0; col < matrixRows.size(); ++col) {
            std::cout << matrixRows[row][col] << " ";
        }
      }
   }

public:    
   virtual ~AdjacencyMatrixGraph() {
      for (Vertex* vertex : vertices) {
         delete vertex;
      }
   }

   // Creates and adds a new vertex to the graph, provided a vertex with the
   // same label doesn't already exist in the graph. Returns the new vertex on
   // success, nullptr on failure.
   virtual Vertex* AddVertex(std::string newVertexLabel) override {
      for (Vertex* vertex : vertices) {
        if (vertex->GetLabel() == newVertexLabel) {
            return nullptr;
        }
      }
      
      Vertex* newVertex = new Vertex(newVertexLabel);
      vertices.push_back(newVertex);
      matrixIndexMap[newVertex] = vertices.size() - 1;
      int newVertexIndex = matrixIndexMap[newVertex];

      // passes the corresponding vertex index so matrix can be updated
      populateMatrix(newVertexIndex);

      return newVertex;
   }
   
   // Adds a directed edge from the first to the second vertex. If the edge
   // already exists in the graph, no change is made and false is returned.
   // Otherwise the new edge is added and true is returned.

   // function can be refactored to implement the matrixIndexMap for faster indexing
   virtual bool AddDirectedEdge(Vertex* fromVertex, Vertex* toVertex) override {
      if (HasEdge(fromVertex, toVertex)) {
        return false;
      }

      int fromVertexIndex = matrixIndexMap[fromVertex];
      int toVertexIndex = matrixIndexMap[toVertex];
      matrixRows[fromVertexIndex][toVertexIndex] = true;

      return true;
   }
    
   // Returns a vector of edges with the specified fromVertex.
   virtual std::vector<Edge> GetEdgesFrom(Vertex* fromVertex) override {
      std::vector<Edge> outgoingEdges;
      if (matrixIndexMap.contains(fromVertex)) {
        int fromIndex = matrixIndexMap.at(fromVertex);
        for (int col = 0; col < vertices.size(); ++col) {
            if (matrixRows[fromIndex][col]) {
                Vertex* adjacentVertex = vertices[col];
                Edge edge(fromVertex, adjacentVertex);
                outgoingEdges.push_back(edge);
            }
        }
      }

      return outgoingEdges;
   }
    
   // Returns a vector of edges with the specified toVertex.
   // lookup of matrix firstly by column then row shows incoming edges
   virtual std::vector<Edge> GetEdgesTo(Vertex* toVertex) override {
      std::vector<Edge> incomingEdges;
      if (matrixIndexMap.contains(toVertex)) {
        int toIndex = matrixIndexMap.at(toVertex);
        for (int row = 0; row < vertices.size(); ++row) {
            if (matrixRows[row][toIndex]) {
                Vertex* adjacentVertex = vertices[row];
                Edge edge(adjacentVertex, toVertex);
                incomingEdges.push_back(edge);
            }
        }
      }

      return incomingEdges;
   }
    
   // Returns a vertex with a matching label, or nullptr if no such vertex
   // exists
   virtual Vertex* GetVertex(std::string vertexLabel) override {
      for (Vertex* vertex : vertices) {
        if (vertex->GetLabel() == vertexLabel) {
            return vertex;
        }
      }

      return nullptr;
   }
    
   // Returns true if this graph has an edge from fromVertex to toVertex
   virtual bool HasEdge(Vertex* fromVertex, Vertex* toVertex) override {
      if (matrixIndexMap.contains(fromVertex) && matrixIndexMap.contains(toVertex)) {
        int fromVertexIndex = matrixIndexMap.at(fromVertex);
        int toVertexIndex = matrixIndexMap.at(toVertex);

        return matrixRows[fromVertexIndex][toVertexIndex];
      }

      return false;
   }
};

#endif