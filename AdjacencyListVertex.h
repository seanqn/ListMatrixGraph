#ifndef ADJACENCYLISTVERTEX_H
#define ADJACENCYLISTVERTEX_H

#include <vector>
#include "Vertex.h"

class AdjacencyListVertex : public Vertex {
public:
   // Vector of vertices adjacent to this vertex. For each vertex V in this
   // vector, V is adjacent to this vertex, meaning an edge from this vertex to
   // V exists in the graph.
   std::vector<Vertex*> adjacent;
   
   AdjacencyListVertex(const std::string label) : Vertex(label) {
   }
    
   virtual ~AdjacencyListVertex() {
   }
};

#endif