## List/Matrix Graph
This repo serves as a dual graph structure which provides both list (vector) and matrix (2D vector) implementations. Both classes inherit from the pure virtual
DirectedGraph class, which outlines the core graph functionality.

### Adjacency List Graph
The list graph structure uses AdjacencyListVertex objects as vertices instead of Vertex objects as would typically be expected from DirectedGraph derived classes.
AdjacencyListVertex defines an additional vector member that stores all adjacent vertices to that vertex. Since inherited methods define Vector objects as
arguments, a resolution map is used to store corresponding AdjacencyListVertex, Vertex pairs to reduce type cast clutter. Another resolution map exists for
quickly viewing vertex labels. The inverseAdjacenct map is used specifically for mapping a vertex to vertices that have an outgoing edge to that vertex. Both
adjacent and inverse adjacent vertices for a vertex are determined when a directed edge is added.

### Adjacency Matrix Graph
The matrix graph is a 2D vector that stores a boolean value indicating if a vertex is adjacent to another. After the matrix is populated with default
values, values are assigned to an index in the matrix when a directed edge is added. A resolution map stores vertices to their corresponding index in the
matrix, which is equivalent to the one dimensional size of the matrix after that vertex is added. Resolving vertices to their matrix indices avoids O(N^2)
traversals and simplifies retrieving incoming edges from a vertex.
