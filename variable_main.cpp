#include <stdio.h>
#include <string>
#include <assert.h>
#include <string.h>
#include <algorithm>
#include <numeric>

#include "fileio0.h"

#include "VariableCFL.h"

#include "IntColoringSaveLoad.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc<3){
		fprintf(stderr, "Usage: %s adjList.txt numChansReq.txt [numTotalChan=?] [randomSeed=?]\n", argv[0]);
		return 0;
	}
	
	// read the graph:
	fprintf(stderr, "Input adjList file: %s\n", argv[1]);
	FILE *fp = fopen(argv[1], "r");
	Graph g = Graph::Load(fp);
	fclose(fp);
	
	vector<int> numChanReq;
	
	fp = fopen(argv[2],"r");
	
	char buffer[1024];
	
	while(!feof(fp)){
		buffer[0]='\0';
		fgetline(buffer,1024,fp);
		if (strlen(buffer)==0) continue;
		numChanReq.push_back(atoi(buffer));
	}
	
	assert(numChanReq.size()==g.Count_of_Vertices());
	
	int numTotalChan_lower_incl = *max_element(numChanReq.begin(),numChanReq.end());
	int numTotalChan_upper_excl = 1 + accumulate(numChanReq.begin(),numChanReq.end(),0);
	
	int numTotalChan;
	
	unsigned int randomSeed = 0;
	
	if(argc>3){
		
		vector<string> tokens;
		
		for(int i=3;i<argc;i++){
			tokenize(argv[i],tokens,"=");
			if(tokens.size()!=2) continue;
			
			if(tokens[0]=="numTotalChan"){
				numTotalChan = atoi(tokens[1].c_str());
				
				if (numTotalChan<numTotalChan_lower_incl){
					fprintf(stderr, "numTotalChan too small.\n");
					return 0;
				}
				
				else {
					numTotalChan_lower_incl = numTotalChan;
					numTotalChan_upper_excl = numTotalChan +1;
				}
			}
			
			else if(tokens[0]=="randomSeed"){
				randomSeed = atoi(tokens[1].c_str());
			}
			
		}
	}
	
	srand(randomSeed);
	
	for(numTotalChan=numTotalChan_upper_excl-1;numTotalChan>=numTotalChan_lower_incl;numTotalChan--)
	{
		fprintf(stderr, "Now trying numTotalChan=%d\n", numTotalChan);
		
		char outputFileName[2048];
		sprintf(outputFileName,"%s_numTotalChan%d.out",argv[1],numTotalChan);
		
		VariableCFL problem(numChanReq,numTotalChan,g);
		
		int success = problem.RunAlgorithm(10000,0.3,10000);
		
		FILE *fpresult = fopen(outputFileName,"w");
		
		if(success) {
			vector<unordered_set<int>> currentColoring = problem.GetCurrentColoring();
			IntColoringSaveLoad::Save(currentColoring, numTotalChan , fpresult);
			fprintf(fpresult, "# Success to color %s with %d colors in %d cycles\n", argv[1], numTotalChan, success);
			fprintf(stderr,"Success to color %s with %d colors in %d cycles\n", argv[1], numTotalChan,success);
		}
		else {
			fprintf(fpresult, "# FAIL\n");
			fprintf(stderr, "Fail to color %s\n", argv[1]);
		}
		
		fclose(fpresult);
	}
	
	return 0;
}
