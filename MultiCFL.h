#pragma once

#include "graph.h"

using namespace std;

class MultiCFL
{
	//This class try to find a fractional coloring that assign total 1.00 channel to each node.

protected:
	Graph interfereGraph;
	int numFragment; //this is the M in the doc
	int numColors; //Maximum number of integer channels, so there is numFragment*numColors fractional channels.

	int num_total_conflicts;

private:
	vector<unordered_set<int>> currentColoring;
	vector<vector<double>> currentProbability;

public:
	int NumNodes() const;
	vector<unordered_set<int>> GetCurrentColoring() const;
	vector<vector<double>> GetCurrentProbability() const;

	MultiCFL(int _numFragment, int _numColors, Graph _interfereGraph);
	bool RunAlgorithm(int Max_Cycle,double beta,int window = 10);

	//friend class numFragmentOptimize;

private:
	void PickupRandomColoring();
	bool ReconfigureProbability(double beta);

protected:
	static double NormalizeVector(vector<double> &obj, double sumTo);
	static bool IsDecreasing(vector<int> series, int window = 10);

};