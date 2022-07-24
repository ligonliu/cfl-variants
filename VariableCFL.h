#pragma once


#include "graph.h"
#include <vector>

class VariableCFL
{
	//This class try to find a integer coloring that 
	//assign total numColorsRequested[i] channel(s) to node i.

protected:
	Graph interfereGraph;
	std::vector<int> numColorsRequested;
	int numColors; //Maximum number of integer channels
	int num_total_conflicts;

private:
	std::vector<std::unordered_set<int> > currentColoring;
	std::vector<std::vector<double> > currentProbability;

public:
	int NumNodes() const;
	std::vector<std::unordered_set<int> > GetCurrentColoring() const;
	std::vector<std::vector<double> > GetCurrentProbability() const;

	VariableCFL(std::vector<int> _numColorsRequested, int _numColors, Graph _interfereGraph);
	int RunAlgorithm(int Max_Cycle,double beta,int window = 10);

	//friend class numFragmentOptimize;

private:
	void PickupRandomColoring();
	bool ReconfigureProbability(double beta);

protected:
	static double NormalizeVector(std::vector<double> &obj, double sumTo);
	static bool IsDecreasing(std::vector<int> series, int window = 10);

};