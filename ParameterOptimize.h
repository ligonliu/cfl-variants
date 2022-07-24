#pragma once

#include "MultiCFL.h"

#include <limits.h>

class ParameterOptimize
{
public:
	const static int MAXCYCLE_TRY = 100;
	const static int MAXCYCLE_TRY_FOR_NUMCOLORS = MAXCYCLE_TRY;
	const static int MAX_NUM_FRAGMENT = 100; //It's a deep fragmentation.... 100 should be enough.

	static int Optimize_numColors(Graph interference_graph, int max_num_colors,int test_numFragment=MAX_NUM_FRAGMENT, int maxCycle_Try=MAXCYCLE_TRY);
	static int Optimize_numFragment(Graph graph, int max_num_colors, int max_numFragment, int maxCycle_Try=MAXCYCLE_TRY);

	const static int OPTI_COLOR_FAIL = INT_MAX;
	const static int OPTI_FRAGMENT_FAIL = INT_MAX;
};

