#include "randomInterferenceGraph.h"

#include <cstdlib>
#include <ctime>

#include <cmath>
using namespace std;


Graph RandomInterferenceGraph::DefaultRandomInterferenceGraph(int num_nodes, double interfere_radius)
{
	vector<double> xs,ys;

	Graph returnvalue(num_nodes);

	srand((unsigned)time(NULL));
	
	int vertices_added = 0;

	while(vertices_added<num_nodes){	
		double x=((double)rand()/(double)RAND_MAX);
		double y=((double)rand()/(double)RAND_MAX);

		if(x*x+y*y>1) continue;

		xs.push_back(x);
		ys.push_back(y);

		vertices_added++;
	}

	for(int i=0;i<num_nodes;i++){
		for(int j=0;j<i;j++){
			double distance = sqrt((xs[i]-xs[j])*(xs[i]-xs[j]) + (ys[i]-ys[j])*(ys[i]-ys[j]));
			if(distance<interfere_radius){
				returnvalue.AddEdge(i,j);
			}
		}
	}

	return returnvalue;
};