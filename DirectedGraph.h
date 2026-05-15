#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <string>
#include <vector>
#include "Vertex.h"
#include "Edge.h"

// Abstract base class for a directed, unweighted graph
class DirectedGraph {
public:
   virtual ~DirectedGraph() {
   }

   // Creates and adds a new vertex to the graph, provided a vertex with the
   // same label doesn't already exist in the graph. Returns the new vertex on
   // success, nullptr on failure.
   virtual Vertex* AddVertex(std::string newVertexLabel) = 0;
    
   // Adds a directed edge from the first to the second vertex. No change is
   // made and false is returned if the edge already exists in the graph.
   // Otherwise the new edge is added and true is returned.
   virtual bool AddDirectedEdge(Vertex* fromVertex, Vertex* toVertex) = 0;
    
   // Returns a vector of edges with the specified fromVertex.
   virtual std::vector<Edge> GetEdgesFrom(Vertex* fromVertex) = 0;
    
   // Returns a vector of edges with the specified toVertex
   virtual std::vector<Edge> GetEdgesTo(Vertex* toVertex) = 0;
    
   // Returns a vertex with a matching label, or nullptr if no such vertex
   // exists
   virtual Vertex* GetVertex(std::string vertexLabel) = 0;
    
   // Returns true if this graph has an edge from fromVertex to toVertex
   virtual bool HasEdge(Vertex* fromVertex, Vertex* toVertex) = 0;
};

#endif