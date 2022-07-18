# cfl-variants
My variants of Leith&amp;Clifford's CFL randomized approximation algorithm for graph fractional coloring and multicoloring

MultiCFL: Each vertex is colored by multiple fractional colors, their fractions sum up to 1.0

VariableCFL: Each vertex is colored by one or multiple colors, specified by their 

# Comments from 2022
Performance enhancement of embedded processors and ubiquitous cellular connectivity have eliminated the need of decentralized channel contention, because the optimal channel assignments can be computed on a single head node or a cloud server, based on the topological information aggregated from each node's sensing.

# Usage
  ./VariableCFL adjList.txt numColorsReq.txt [numTotalColors]
  
  adjList.txt: "(E,V)" adjacent list of the graph, the i<sup>th</sup> line is the adjacent list of the i<sup>th</sup> vertex.
  
  numColorsReq.txt: "D", the i<sup>th</sup> line the number of colors needed by the i<sup>th</sup> vertex.
  
  numTotalColors: optional parameter "N<sub>c</sub>" number of total available colors(or channels). If not specified, the program will try each number from sum(D) down to max(D)

