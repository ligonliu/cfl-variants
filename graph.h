#pragma once

#include <vector>
#include <unordered_set>
#include <cstdio>

struct Graph
{
	std::vector<std::unordered_set<int>> edges;
	int Count_of_Vertices() const;
	int AddVertex(); //Add a node and return the node#
	void AddEdge(int v1, int v2);

	int MaxDegree() const;
	Graph(int _count_of_vertices);
    Graph()=default;

	void Save(FILE* writeTo) const;
	static Graph Load(FILE* readFrom);

};
