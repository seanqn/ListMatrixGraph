#ifndef ADJACENCYLISTGRAPH_H
#define ADJACENCYLISTGRAPH_H

#include "DirectedGraph.h"
#include "AdjacencyListVertex.h"
#include <unordered_map>
#include <unordered_set>

class AdjacencyListGraph : public DirectedGraph {
protected:
   std::vector<AdjacencyListVertex*> vertices;

   // Vertex* objects are passed in as parameters for most of this class's methods
   // mapping Vector* objects to AdjacencyListVertex* resolves type complications
   std::unordered_map<Vertex*, AdjacencyListVertex*> ALVMap;

   // mapping labels to Vertex* objects provides efficient label lookup
   std::unordered_map<std::string, Vertex*> vertexLabelMap;

   // pairs of <Vertex*, vector<Vertex*>> represents inverse adjacency
   // incoming edges stored as <adjacentVertex, vector<edgeOriginVertex>>
   std::unordered_map<Vertex*, std::vector<Vertex*>> inverseAdjacentMap;
   
public:    
   virtual ~AdjacencyListGraph() {
      for (AdjacencyListVertex* vertex : vertices) {
         delete vertex;
      }
   }

   // Creates and adds a new vertex to the graph, provided a vertex with the
   // same label doesn't already exist in the graph. Returns the new vertex on
   // success, nullptr on failure.
   virtual Vertex* AddVertex(std::string newVertexLabel) override {
      if (!vertexLabelMap.contains(newVertexLabel)) {
        Vertex* newVertex = new Vertex(newVertexLabel);
        AdjacencyListVertex* newALVertex = new AdjacencyListVertex(newVertexLabel);

        // push to vector
        vertices.push_back(newALVertex);
        
        // insert pair
        ALVMap[newVertex] = newALVertex;
        
        // insert label and Vertex pair
        vertexLabelMap[newVertexLabel] = newVertex;

        // insert vertex to inverseAdjacentMap, corresponds to a currently empty vector
        inverseAdjacentMap[newVertex] = {};

        return newVertex;
      }

      return nullptr;
   }
    
   // Adds a directed edge from the first to the second vertex. If the edge
   // already exists in the graph, no change is made and false is returned.
   // Otherwise the new edge is added and true is returned.
   virtual bool AddDirectedEdge(Vertex* fromVertex, Vertex* toVertex) override {
      // HasEdge() will check that both vertices exist
      if (HasEdge(fromVertex, toVertex)) {
        return false;
      }

      // append adjacent toVertex to fromVertex->adjacent vector
      ALVMap.at(fromVertex)->adjacent.push_back(toVertex);

      // insert an inverse adjacent fromVertex to toVertex
      inverseAdjacentMap[toVertex].push_back(fromVertex);

      return true;
   }
    
   // Returns a vector of edges with the specified fromVertex.
   // collects the passed vertex's list of adjacent vertices to fill the outgoing edges vector
   virtual std::vector<Edge> GetEdgesFrom(Vertex* fromVertex) override {
      std::vector<Edge> outgoingEdges;
      if (ALVMap.contains(fromVertex)) {
        for (Vertex* adjacentVertex : ALVMap[fromVertex]->adjacent) {
            Edge edge(fromVertex, adjacentVertex);
            outgoingEdges.push_back(edge);
        }
      }

      return outgoingEdges;
   }
    
   // Returns a vector of edges with the specified toVertex.
   // goes through adjacent vertices for all vertices to collect all possible incoming edges to passed vertex
   virtual std::vector<Edge> GetEdgesTo(Vertex* toVertex) override {
      std::vector<Edge> incomingEdges;
      if (inverseAdjacentMap.contains(toVertex)) {
        for (Vertex* inverseAdjacentVertex : inverseAdjacentMap[toVertex]) {
            Edge edge(inverseAdjacentVertex, toVertex);
            incomingEdges.push_back(edge);
        }
      }

      return incomingEdges;
   }
    
   // Returns a vertex with a matching label, or nullptr if no such vertex
   // exists
   virtual Vertex* GetVertex(std::string vertexLabel) override {
      if (vertexLabelMap.contains(vertexLabel)) {
        return vertexLabelMap.at(vertexLabel);
      }
      return nullptr;
   }
    
   // Returns true if this graph has an edge from fromVertex to toVertex
   virtual bool HasEdge(Vertex* fromVertex, Vertex* toVertex) override {
      if (ALVMap.contains(fromVertex) && ALVMap.contains(toVertex)) {
        AdjacencyListVertex* ALVertex = ALVMap.at(fromVertex);
        if (!ALVertex) {
            return false;
        }
        for (Vertex* adjacentVertex : ALVertex->adjacent) {
            if (adjacentVertex && adjacentVertex == toVertex) {
                return true;
            }
        }
      }

      return false;
   }
};

#endif