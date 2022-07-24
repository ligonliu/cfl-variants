#include "ParameterOptimize.h"

//should fix numFragment = max_numFragment, and optimize num_colors to the minimum first

//then optimize numFragment, satisfy constriant:  num_colors < best_num_colors * 110%.

int ParameterOptimize::Optimize_numColors(Graph interference_graph, int max_numColors, int test_numFragment, int maxCycle_Try)
{
	//use the numFragment = max_numFragment
	//find the best_num_colors, which is minimum keeping the multicoloring solvable.
	int min_numColors = 1;
	int min_success_numColors = OPTI_COLOR_FAIL;

	while(min_numColors!=max_numColors){


		int current_colors = (min_numColors+max_numColors)/2;
		
		#ifdef _DEBUG
		printf("#DEBUG: Optimize_numColors: Trying with %d Colors and %d Fragments\n", current_colors, test_numFragment);
		#endif

		MultiCFL currentCFLobj(test_numFragment, current_colors, interference_graph);

		bool success = currentCFLobj.RunAlgorithm(MAXCYCLE_TRY_FOR_NUMCOLORS, 0.5);

		if(success){
			max_numColors = current_colors;
			min_success_numColors = current_colors;
		}
		else{
			min_numColors = 1 + (min_numColors + current_colors)/2; 
		}
	}

	return min_success_numColors;
}

int ParameterOptimize::Optimize_numFragment(Graph interference_graph, int max_num_colors, int max_numFragment, int maxCycle_Try)
{
	//Perform binary search of the optimal numFragment
	//from 1 to max_numFragment
	int min_numFragment = 1;

	int min_success_Fragment = INT_MAX;

	while(min_numFragment!=max_numFragment){
		int current_numFragment = (min_numFragment + max_numFragment)/2;

		#ifdef _DEBUG
		printf("#DEBUG: Optimize_numFragment: Trying with %d Colors and %d Fragments\n", max_num_colors, current_numFragment);
		#endif

		MultiCFL currentCFLobj(current_numFragment, max_num_colors, interference_graph);

		bool success = currentCFLobj.RunAlgorithm(maxCycle_Try, 0.5);

		if(success){
			max_numFragment = current_numFragment;
			min_success_Fragment = current_numFragment;
		}
		else {
			min_numFragment = 1 + (min_numFragment + current_numFragment)/2;  //keep conservative in determine the min!
			
		}
	}

	return min_success_Fragment;
}