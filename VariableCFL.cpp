#include "VariableCFL.h"

#include <cstdlib>

#include <algorithm>
#include <unordered_set>

using namespace std;

int VariableCFL::NumNodes() const
{
	return interfereGraph.Count_of_Vertices();
}

vector<unordered_set<int> > VariableCFL::GetCurrentColoring() const
{
	return currentColoring;
}

vector<vector<double> > VariableCFL::GetCurrentProbability() const
{
	return currentProbability;
}

VariableCFL::VariableCFL(vector<int> _numColorsRequested, int _numColors, Graph _interfereGraph):numColorsRequested(_numColorsRequested),interfereGraph(_interfereGraph),numColors(_numColors),num_total_conflicts(0)
{
	//vector<double> uniform;
	//uniform.resize(_numFragment*_numColors, 1.0/_numColors); //start with uniform distribution

	//currentProbability.resize(_interfereGraph.Count_of_Vertices(),uniform);
	currentProbability.resize(_interfereGraph.Count_of_Vertices());
	
	for(int i=0;i<_interfereGraph.Count_of_Vertices();i++){
		currentProbability[i].resize(_numColors, 1.0/_numColors);
	}
	
	currentColoring.resize(_interfereGraph.Count_of_Vertices());
}


int VariableCFL::RunAlgorithm(int Max_Cycle, double beta, int window)
{
	//attempt assign numFragment channels to each node
	//started by initialization of currentProbability to be uniform
	bool success = false;
	int cycle = 0;

	vector<int> conflict_history;

	conflict_history.reserve(Max_Cycle);

	while((!success) && cycle<Max_Cycle &&IsDecreasing(conflict_history, window)){
		PickupRandomColoring();
		success = ReconfigureProbability(beta);
		conflict_history.push_back(this->num_total_conflicts);
		cycle++;
		
		fprintf(stderr,"Current Cycle: %d\n", cycle);
		
	}
	if (success){
		return cycle;
	}
	else {
		return 0;
	}
}

bool VariableCFL::IsDecreasing(vector<int> series,int window)
{
	//check the series' last "window" elements have more than significant (5%) decrease than past 3
	double control_sum=0, control_size=0;
	double sample_sum=0, sample_size=0;

	for(int i=0; i<window && ((int)series.size()-i>0) ;i++){
		sample_sum += series[series.size()-i-1];
		sample_size ++;
	}

	for(int j= window; j<window*2 && ((int)series.size()-j>0) ;j++){
		control_sum += series[series.size()-j-1];
		control_size ++;
	}

	if(sample_size==0 || control_size==0) return true;
	else {
		double sample_average = sample_sum/sample_size;
		double control_average = control_sum/control_size;
		return (sample_average/control_average)<0.95;
	} 

}

void VariableCFL::PickupRandomColoring()
{
	//randomly pickup numColorsRequested[i] choices from all colors
	//If there is a probability of 1.00, that color is a "must choice"
	//For all that is lower than 1.00, divide their chance proportionallly by probability value.

	int num_nodes = NumNodes();

	for(int i=0;i<num_nodes;i++){

		currentColoring[i].clear();

		//first, find in currentProbability[i] which one(s) have 1.00 probability
		unordered_set<int> colors_less_than_prob_1;
		for(int j=0;j<currentProbability[i].size();j++){
			if(currentProbability[i][j]<1.00) colors_less_than_prob_1.insert(j);
			else currentColoring[i].insert(j);
		}

		//randomly choose from colors_less_than_prob_1 and insert it into currentColoring[i]

		unordered_set<int>::const_iterator it_lessthan1 = colors_less_than_prob_1.begin();

		while(currentColoring[i].size() < numColorsRequested[i]){
			int color = *it_lessthan1;
			double prob = currentProbability[i][color];
			double random = (double)rand()/(double)RAND_MAX;

			if(random<prob){
				currentColoring[i].insert(color);
			}

			it_lessthan1++;
			if(it_lessthan1 == colors_less_than_prob_1.end()) it_lessthan1=colors_less_than_prob_1.begin();
		}
	}
}

bool VariableCFL::ReconfigureProbability(double beta)  //Return true if there are no conflict (coloring found), false otherwise.
{
	int out_num_total_conflicts=0;

	//three sets: (in the current M coloring: conflict_set, nconflict_set, not in the current M coloring: others_set)
	//nconflict_set has prob = 1.00
	int num_nodes = NumNodes();

	bool no_conflict_globally = true;

	for(int i=0;i<num_nodes;i++){

		unordered_set<int> conflict_set, nconflict_set;
		unordered_set<int>& neighborNodes =  this->interfereGraph.edges[i];

		//here we get a set of neighborNodes' colors
		unordered_set<int> neighbor_colors;

		for (auto it=neighborNodes.begin();it!=neighborNodes.end();it++){
			
			int neighbor = *it;
			
			for (auto it=currentColoring[neighbor].begin(); it!=currentColoring[neighbor].end();it++) {
				int neighborColor = * it;
				neighbor_colors.insert(neighborColor);
			}
		}

		for(auto it=currentColoring[i].begin();it!=currentColoring[i].end();it++){
			//check neighbor_colors
			
			int color = *it;
			
			if(neighbor_colors.find(color)!=neighbor_colors.end()){
				conflict_set.insert(color);
			}
			else{
				nconflict_set.insert(color);
			}
		}

		if(conflict_set.size()>0) {
			no_conflict_globally=false;
		}

		out_num_total_conflicts+=conflict_set.size();

		vector<double> &prob = this->currentProbability[i];
		int N_NOCONF = nconflict_set.size();

		for(int j=0; j<prob.size();j++){
			if(nconflict_set.find(j)!=nconflict_set.end()){
				prob[j] = 0;
			}
		}

		NormalizeVector(prob, (1-beta)*((double)numColorsRequested[i]-N_NOCONF));

		for(int j=0; j<prob.size();j++){
			if(currentColoring[i].find(j)==currentColoring[i].end()) {
				prob[j] += beta * ((double)numColorsRequested[i]-N_NOCONF) / (numColors-numColorsRequested[i]); //(M-N_NOCONF)/(NCOLOR_FRAC-M)
			}
			else {
				if(nconflict_set.find(j)!=nconflict_set.end()){
					prob[j] = 1.0;
				}
			}
		}
	}
	#ifdef _DEBUG
		printf("#DEBUG: total conflict: %d\n",out_num_total_conflicts) ;
	#endif

	this->num_total_conflicts = out_num_total_conflicts;

	return no_conflict_globally;
}


double VariableCFL::NormalizeVector(vector<double> &obj, double sumTo)
{
	double sum = 0;

	for(int i=0; i<obj.size();i++){
		sum += obj[i];
	}

	if(sum==0.0) return 0;

	for(int i=0; i<obj.size();i++){
		obj[i]*=sumTo/sum;
	}

	return sum;
}