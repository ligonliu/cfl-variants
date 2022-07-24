#pragma once

#include "graph.h"

/*
RandomInterferenceGraph: In the unit circle(circle with radius 1.0, generate random nodes and see if they interfere)
*/

class RandomInterferenceGraph
{
public:
	static Graph DefaultRandomInterferenceGraph(int num_nodes, double interfere_radius);
};

