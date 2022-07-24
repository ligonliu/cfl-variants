#include <stdio.h>
#include <stdlib.h>
// #include <intrin.h>
#include <time.h>
#include <string>
#include <vector>

using namespace std;

#include "randomInterferenceGraph.h"
#include "graph.h"

#include "ParameterOptimize.h"
#include "ColoringSaveLoad.h"

#ifdef _WIN32
#include "win32\dirent.h"
#else
#include <dirent.h>
#endif

/*
Process: 

1. Generate random graph with parameters.
2. Use ParameterOptimize to optimize num_color and num_fragment
3. Use MultiCFL to find a coloring that assign each node with 1.0 fraction channel.
4. Use MultiCFL_optimize to use the rest fractional channels to optimize the sigma-benefit.

*/


vector<string> ListInputFiles()
{
	vector<string> returnvalue;

	DIR *dir;
	struct dirent *ent;
	dir = opendir (".");
	if (dir != NULL) {

	/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			//printf ("%s\n", ent->d_name);
			returnvalue.push_back(ent->d_name);
		}
		closedir (dir);
	} 

	return returnvalue;
}


int main(int argc, char *argv[])
{
	double MAX_INTERFERE_RADIUS=0;
	int MAX_FRAGMENTS=0;
	int MAX_CHANNELS=0;
	int NUM_NODES = 0;

	//srand(__rdtsc());
	
	

	//if there are no graph*.txt, generate some random graphs.
	
	if(argc<3){
		printf("To generate new random interference graphs: %s NUM_NODES MAX_INTERFERE_RADIUS MAX_FRAGMENT MAX_CHANNELS [RANDOM_SEED]\n", argv[0]);
		printf("To use existing random interference graphs: %s MAX_FRAGMENT MAX_CHANNELS\n", argv[0]);
		exit(0);
	}
	else{
		sscanf(argv[1], "%d", &MAX_FRAGMENTS);
		sscanf(argv[2], "%d", &MAX_CHANNELS);
	}

START:
	vector<string> allfiles = ListInputFiles();
	vector<string> graphdatafiles;

	for(int i=0;i<allfiles.size();i++){
		
		string &fname = allfiles[i];
		
		if(fname.substr(0,5) == "graph" && fname.substr(fname.length()-4,4) == ".txt") {
			graphdatafiles.push_back(fname);
		}
	}

	if(graphdatafiles.size()==0){
		//generate random graphs using RandomInterferenceGraph::DefaultRandomInterferenceGraph
		if(argc<5) {
			printf("No graph files detected. graph files should be like graph***.txt\nTo generate new random interference graphs: %s NUM_NODES MAX_INTERFERE_RADIUS MAX_FRAGMENTS MAX_CHANNELS\n", argv[0]);
			exit(0);
		}
		else{
			if(argc==5){
				sscanf(argv[1],"%d", &NUM_NODES);
				sscanf(argv[2],"%lf", &MAX_INTERFERE_RADIUS);
				sscanf(argv[3], "%d", &MAX_FRAGMENTS);
				sscanf(argv[4], "%d", &MAX_CHANNELS);
			}
			else if (argc==6)
			{
				sscanf(argv[1],"%d", &NUM_NODES);
				sscanf(argv[2],"%lf", &MAX_INTERFERE_RADIUS);
				sscanf(argv[3], "%d", &MAX_FRAGMENTS);
				sscanf(argv[4], "%d", &MAX_CHANNELS);
				unsigned int init_seed;
				sscanf(argv[5], "%d", &init_seed);
				srand(init_seed);
			}
		}
		int guid=rand()*rand();

		for(double i_radius=0.1; i_radius<MAX_INTERFERE_RADIUS; i_radius+=MAX_INTERFERE_RADIUS/10){
			for(int k=0;k<10;k++){ //generate 10 i_radius disk graphs

				Graph g=RandomInterferenceGraph::DefaultRandomInterferenceGraph(NUM_NODES,i_radius);
				char filename[256];
				srand(guid);
				guid = rand()*rand();
				sprintf(filename, "graph_%08x_NODES_%d_INTRAD_%g_MAXFRAG_%d_MAXCHAN_%d.txt",(unsigned int)guid, NUM_NODES, i_radius, MAX_FRAGMENTS, MAX_CHANNELS);
				FILE *fp = fopen(filename, "w");
				g.Save(fp);
				fclose(fp);
			}
		}
		goto START;
	
	}
	//real code starts here:
	//read from the graph*.txt files:

	for (int i=0;i<graphdatafiles.size();i++){
		string &filename = graphdatafiles[i];
		bool success;

		printf("Now coloring %s\n", filename.c_str());
		FILE *fp = fopen(filename.c_str(), "r");
		Graph g = Graph::Load(fp);
		fclose(fp);

		//get the current time
		clock_t time_in_clock_ticks1 = clock();
		double begin_in_seconds = (double)time_in_clock_ticks1 / (double)CLOCKS_PER_SEC;
		

		//now g is the graph to color.
		int opt_num_colors = ParameterOptimize::Optimize_numColors(g,MAX_CHANNELS,MAX_FRAGMENTS);

		if(opt_num_colors==ParameterOptimize::OPTI_COLOR_FAIL){
			success = false;
			opt_num_colors = MAX_CHANNELS;
		}

		int opt_num_fragment = ParameterOptimize::Optimize_numFragment(g,(int)(opt_num_colors*1.1),MAX_FRAGMENTS);

		if(opt_num_fragment==ParameterOptimize::OPTI_FRAGMENT_FAIL){
			success = false;
			opt_num_fragment = MAX_FRAGMENTS;
		}

		printf("Final Coloring with NUM_COLORS = %d and NUM_FRAGMENT = %d\n", opt_num_colors, opt_num_fragment);

		MultiCFL last(opt_num_fragment,opt_num_colors,g);
		success = last.RunAlgorithm(10000,0.3,100);

		clock_t time_in_clock_ticks2 = clock();
		double end_in_seconds = (double)time_in_clock_ticks2 / (double)CLOCKS_PER_SEC;

		string filenameresult = filename + ".result.log";
		FILE *fpresult = fopen(filenameresult.c_str(),"w");

		if(success) {
			vector<unordered_set<int>> currentColoring = last.GetCurrentColoring();
			ColoringSaveLoad::save(currentColoring, opt_num_colors, opt_num_fragment , fpresult);
			fprintf(fpresult, "Success to color %s\n", filename.c_str());
			printf("Success to color %s\n", filename.c_str());
		}
		else {
			fprintf(fpresult, "FAIL\n");
			printf("Fail to color %s\n", filename.c_str());
		}

		printf("%.3f seconds elapsed\n", end_in_seconds - begin_in_seconds);

		fclose(fpresult);
	}

	getc(stdin);

	return 0;
}