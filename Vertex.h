#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <string>

class Vertex {
protected:
   std::string label;

public:
   Vertex(const std::string vertexLabel) {
      label = vertexLabel;
   }
   
   virtual ~Vertex() {
   }
   
   virtual std::string GetLabel() {
      return label;
   }
   
   // Prints this vertex's label
   virtual void Print(std::ostream& output) {
      output << label;
   }
   
   virtual void SetLabel(std::string newLabel) {
      label = newLabel;
   }
};

#endif