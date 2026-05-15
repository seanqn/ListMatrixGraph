#ifndef GRAPHCOMMANDS_H
#define GRAPHCOMMANDS_H

#include <iostream>
#include <string>
#include <vector>
#include "DirectedGraph.h"

class DirectedGraphTestCommand {
public:
   virtual ~DirectedGraphTestCommand() {
   }

   // Returns true if the test passes, false if the test fails
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph) = 0;
   
   // Utility function that checks if a vector of edges has a particular edge
   bool HasEdge(const std::vector<Edge>& edges, Vertex* fromVertex,
      Vertex* toVertex) const {
      // Iterate through the vector's edges
      for (const Edge& edge : edges) {
         if (edge.fromVertex == fromVertex && edge.toVertex == toVertex) {
            return true;
         }
      }
      return false;
   }
   
   virtual void PrintEdges(std::ostream& output,
      const std::vector<Edge>& edges, std::string separator = ", ",
      std::string prefix = "", std::string suffix = "") {
      // Print the prefix string first
      output << prefix;
      
      // Print edges
      if (edges.size()) {
         edges[0].Print(output);
         for (int i = 1; i < (int)edges.size(); i++) {
            output << separator;
            edges[i].Print(output);
         }
      }
      
      // Print suffix string
      output << suffix;
   }
};

// Command that adds a new vertex to the graph and verifies that the addition
// either failed or succeeded
class AddVertexCommand : public DirectedGraphTestCommand {
protected:
   std::string label;
   bool shouldSucceed;

public:
   AddVertexCommand(std::string vertexLabel, bool shouldSucceed) :
      label(vertexLabel) {
      this->shouldSucceed = shouldSucceed;
   }
   
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph) override {
      // Try to add the vertex. If the return value is non-null, then addition
      // is successful, otherwise addition has failed.
      Vertex* newVertex = graph.AddVertex(label);
      
      if (newVertex) {
         if (!shouldSucceed) {
            testFeedback << "FAIL: AddVertex(\"" << label << "\") returned a ";
            testFeedback << "non-null pointer, but should have returned a ";
            testFeedback << "null pointer due to the label already being in ";
            testFeedback << "use" << std::endl;
            return false;
         }
         
         testFeedback << "PASS: AddVertex(\"" << label << "\") returned a ";
         testFeedback << "non-null pointer" << std::endl;
         return true;
      }
      
      if (shouldSucceed) {
         testFeedback << "FAIL: AddVertex(\"" << label << "\") returned a ";
         testFeedback << "null pointer, but should have returned a non-null ";
         testFeedback << "pointer" << std::endl;
         return false;
      }
      
      testFeedback << "PASS: AddVertex(\"" << label << "\") returned nullptr ";
      testFeedback << "because the label is already in use" << std::endl;
      return true;
   }
};

// Command that gets a vertex by label and verifies the result
class GetVertexCommand : public DirectedGraphTestCommand {
protected:
   std::string label;
   bool shouldExist;

public:
   GetVertexCommand(std::string vertexLabel, bool vertexShouldExist) :
      label(vertexLabel) {
      shouldExist = vertexShouldExist;
   }
   
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph)
      override {
      // Get the vertex by calling GetVertex()
      Vertex* vertex = graph.GetVertex(label);
      
      // Check if the returned vertex is non-null
      if (vertex) {
         // If the vertex shouldn't exist, then print a failure message
         if (!shouldExist) {
            testFeedback << "FAIL: GetVertex(\"" << label << "\") returned a ";
            testFeedback << "non-null pointer, but should have returned a ";
            testFeedback << "null pointer" << std::endl;
            return false;
         }
         
         // Verify the vertex's label
         std::string actualLabel = vertex->GetLabel();
         if (label != actualLabel) {
            testFeedback << "FAIL: GetVertex(\"" << label << "\") returned a ";
            testFeedback << "vertex with incorrect label \"" << actualLabel;
            testFeedback << "\"" << std::endl;
            return false;
         }
         
         testFeedback << "PASS: GetVertex(\"" << label << "\") returned a ";
         testFeedback << "non-null vertex with a correct label" << std::endl;
         return true;
      }
         
      // The returned vertex is null, so check if a null vertex is expected
      if (shouldExist) {
         testFeedback << "FAIL: GetVertex(\"" << label << "\") returned a ";
         testFeedback << "null pointer, but should have returned a non-null ";
         testFeedback << "pointer" << std::endl;
         return false;
      }
      
      // PASS
      testFeedback << "PASS: GetVertex(\"" << label << "\") returned a ";
      testFeedback << "null pointer" << std::endl;
      return true;
   }
};

// Command that adds an edge to a graph
class AddEdgeCommand : public DirectedGraphTestCommand {
protected:
   std::string fromLabel;
   std::string toLabel;
   bool shouldSucceed;

public:
   AddEdgeCommand(std::string fromVertexLabel, std::string toVertexLabel,
      bool shouldSucceed) {
      fromLabel = fromVertexLabel;
      toLabel = toVertexLabel;
      this->shouldSucceed = shouldSucceed;
   }
   
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph) override {
      // Find both vertices
      Vertex* fromVertex = graph.GetVertex(fromLabel);
      Vertex* toVertex = graph.GetVertex(toLabel);
      
      // Add the edge
      bool addedEdge = false;
      if (fromVertex && toVertex) {
         addedEdge = graph.AddDirectedEdge(fromVertex, toVertex);
      }
      
      if (addedEdge == shouldSucceed) {
         // PASS
         testFeedback << "PASS: ";
         if (addedEdge) {
            testFeedback << "Added edge from \"" << fromLabel << "\" to \"";
            testFeedback << toLabel << "\"" << std::endl;
         }
         else {
            testFeedback << "Attempt to add edge from \"" << fromLabel;
            testFeedback << "\" to \"" << toLabel << "\" returned false";
            testFeedback << std::endl;
         }
         return true;
      }
      
      // FAIL
      testFeedback << "FAIL: Add edge from \"" << fromLabel << "\" to \"";
      testFeedback << toLabel << "\"" << std::endl;
      return false;
   }
};

// Command that verifies the return value from a call to HasEdge()
class HasEdgeCommand : public DirectedGraphTestCommand {
protected:
   std::string fromLabel;
   std::string toLabel;
   bool expected;

public:
   HasEdgeCommand(std::string fromVertexLabel, std::string toVertexLabel,
      bool expectedReturnValue) {
      fromLabel = fromVertexLabel;
      toLabel = toVertexLabel;
      expected = expectedReturnValue;
   }
   
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph) override {
      // Find both vertices
      Vertex* fromVertex = graph.GetVertex(fromLabel);
      Vertex* toVertex = graph.GetVertex(toLabel);
      
      // Call HasEdge() to get the actual return value
      bool actual = graph.HasEdge(fromVertex, toVertex);
      
      if (actual != expected) {
         testFeedback << "FAIL: HasEdge() should have returned ";
         testFeedback << (expected ? "true" : "false") << " for an edge from ";
         testFeedback << (fromVertex ? fromVertex->GetLabel() : "null");
         testFeedback << " to ";
         testFeedback << (toVertex ? toVertex->GetLabel() : "null");
         testFeedback << ", but instead returned ";
         testFeedback << (actual ? "true" : "false") << std::endl;
         return false;
      }
      
      testFeedback << "PASS: HasEdge() returned ";
      testFeedback << (expected ? "true" : "false") << " for an edge from ";
      testFeedback << (fromVertex ? fromVertex->GetLabel() : "null");
      testFeedback << " to ";
      testFeedback << (toVertex ? toVertex->GetLabel() : "null");
      testFeedback << std::endl;
      return true;
   }
};

class VerifyEdgesFromCommand : public DirectedGraphTestCommand {
protected:
   std::string fromLabel;
   std::vector<std::string> toLabels;
   
public:
   VerifyEdgesFromCommand(std::string fromVertexLabel,
      const std::vector<std::string>& toVertexLabels) :
      fromLabel(fromVertexLabel), toLabels(toVertexLabels) {
   }
   
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph) override {
      // Find fromVertex
      Vertex* fromVertex = graph.GetVertex(fromLabel);
      if (!fromVertex) {
         testFeedback << "FAIL: GetVertex(\"" << fromLabel << "\") returned ";
         testFeedback << "nullptr for a vertex that should exist" << std::endl;
         return false;
      }
      
      // Ask the graph for edges from fromVertex
      std::vector<Edge> actual = graph.GetEdgesFrom(fromVertex);
      
      bool pass = true;
      if (actual.size() == toLabels.size()) {
         for (const std::string& toLabel : toLabels) {
            // Get the expected to-vertex
            Vertex* expectedTo = graph.GetVertex(toLabel);
            
            // If the actual vector of vertices does not have the edge then the
            // test fails
            if (!HasEdge(actual, fromVertex, expectedTo)) {
               pass = false;
               break;
            }
         }
      }
      else {
         pass = false;
      }
      
      // Print pass or fail message along with actual and expected collections
      testFeedback << (pass ? "PASS" : "FAIL");
      testFeedback << ": Get edges from \"" << fromLabel << "\"" << std::endl;
      PrintEdges(testFeedback, actual, ", ", "  Actual:   {", "}\n");
      testFeedback << "  Expected: {";
      if (toLabels.size()) {
         testFeedback << fromLabel << " to " << toLabels[0];
         for (int i = 1; i < (int) toLabels.size(); i++) {
            testFeedback << ", " << fromLabel << " to " << toLabels[i];
         }
      }
      testFeedback << "}" << std::endl;
      
      return pass;
   }
};

class VerifyEdgesToCommand : public DirectedGraphTestCommand {
protected:
   std::string toLabel;
   std::vector<std::string> fromLabels;
   
public:
   VerifyEdgesToCommand(std::string toVertexLabel,
      const std::vector<std::string>& fromVertexLabels) :
      toLabel(toVertexLabel), fromLabels(fromVertexLabels) {
   }
   
   virtual bool Execute(std::ostream& testFeedback, DirectedGraph& graph) override {
      // Find toVertex
      Vertex* toVertex = graph.GetVertex(toLabel);
      if (!toVertex) {
         testFeedback << "FAIL: GetVertex(\"" << toLabel << "\") returned ";
         testFeedback << "nullptr for a vertex that should exist" << std::endl;
         return false;
      }
      
      // Ask the graph for edges to toVertex
      std::vector<Edge> actual = graph.GetEdgesTo(toVertex);
      
      bool pass = true;
      if (actual.size() == fromLabels.size()) {
         for (const std::string& fromLabel : fromLabels) {
            // Get the expected from-vertex
            Vertex* expectedFrom = graph.GetVertex(fromLabel);
            
            // If the actual vector of vertices does not have the edge then the
            // test fails
            if (!HasEdge(actual, expectedFrom, toVertex)) {
               pass = false;
               break;
            }
         }
      }
      else {
         pass = false;
      }
      
      // Print pass or fail message along with actual and expected collections
      testFeedback << (pass ? "PASS" : "FAIL");
      testFeedback << ": Get edges to \"" << toLabel << "\"" << std::endl;
      PrintEdges(testFeedback, actual, ", ", "  Actual:   {", "}\n");
      testFeedback << "  Expected: {";
      if (fromLabels.size()) {
         testFeedback << fromLabels[0] << " to " << toLabel;
         for (int i = 1; i < (int) fromLabels.size(); i++) {
            testFeedback << ", " << fromLabels[i] << " to " << toLabel;
         }
      }
      testFeedback << "}" << std::endl;
      
      return pass;
   }
};

#endif