#include <algorithm>
#include <iostream>
#include <string>
#include "DirectedGraph.h"
#include "AdjacencyListVertex.h"
#include "AdjacencyListGraph.h"
#include "AdjacencyMatrixGraph.h"
#include "GraphCommands.h"
using namespace std;

bool TestGraph(ostream& testFeedback, DirectedGraph& graph);

int main() {
   // Test AdjacencyListGraph first
   AdjacencyListGraph graph1;
   cout << "AdjacencyListGraph:   " << endl;
   bool adjPass = TestGraph(cout, graph1);
   
   // Test AdjacencyMatrixGraph second
   AdjacencyMatrixGraph graph2;
   cout << endl << "AdjacencyMatrixGraph: " << endl;
   bool matPass = TestGraph(cout, graph2);
   
   // Print test results
   cout << endl << "Summary:" << endl;
   cout << "  AdjacencyListGraph:   " << (adjPass ? "PASS" : "FAIL") << endl;
   cout << "  AdjacencyMatrixGraph: " << (matPass ? "PASS" : "FAIL") << endl;

   return 0;
}

bool TestGraph(ostream& testFeedback, DirectedGraph& graph) {
   vector<DirectedGraphTestCommand*> commands = {
      new AddVertexCommand("A", true),
      new AddVertexCommand("B", true),
      
      // Verify that vertices A and B exist, but not C, D, or E
      new GetVertexCommand("C", false),
      new GetVertexCommand("A", true),
      new GetVertexCommand("B", true),
      new GetVertexCommand("E", false),
      new GetVertexCommand("D", false),
      
      // Add remaining vertices
      new AddVertexCommand("C", true),
      new AddVertexCommand("D", true),
      new AddVertexCommand("E", true),
      
      // Add edges
      new AddEdgeCommand("B", "C", true),
      new AddEdgeCommand("C", "A", true),
      new AddEdgeCommand("C", "D", true),
      new AddEdgeCommand("C", "E", true),
      new AddEdgeCommand("D", "C", true),
      new AddEdgeCommand("E", "A", true),
      new AddEdgeCommand("E", "D", true),
      
      // Attempts to add a duplicate edge should fail
      new AddEdgeCommand("C", "E", false),
      new AddEdgeCommand("D", "C", false),
      
      new VerifyEdgesFromCommand("A", {}),
      new VerifyEdgesFromCommand("B", { "C" }),
      new VerifyEdgesFromCommand("C", { "A", "D", "E" }),
      new VerifyEdgesFromCommand("D", { "C" }),
      new VerifyEdgesFromCommand("E", { "A", "D" }),
      
      new VerifyEdgesToCommand("A", { "C", "E" }),
      new VerifyEdgesToCommand("B", {}),
      new VerifyEdgesToCommand("C", { "B", "D" }),
      new VerifyEdgesToCommand("D", { "C", "E" }),
      new VerifyEdgesToCommand("E", { "C" }),
      
      // Verify some edges
      new HasEdgeCommand("A", "C", false),
      new HasEdgeCommand("A", "E", false),
      new HasEdgeCommand("B", "C", true),
      new HasEdgeCommand("C", "A", true),
      new HasEdgeCommand("C", "B", false),
      new HasEdgeCommand("C", "D", true),
      new HasEdgeCommand("C", "E", true),
      new HasEdgeCommand("D", "C", true),
      new HasEdgeCommand("E", "A", true),
      new HasEdgeCommand("E", "C", false),
      new HasEdgeCommand("E", "D", true)
   };

   // Execute each test command, stopping if any command fails
   for (DirectedGraphTestCommand* command : commands) {
      bool pass = command->Execute(testFeedback, graph);
      delete command;
      if (!pass) {
         return false;
      }
   }

   return true;
}