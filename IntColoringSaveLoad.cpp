#include "IntColoringSaveLoad.h"

#include <string>
#include "fileio0.h"


void IntColoringSaveLoad::Save(const vector<unordered_set<int>> &p, int numColor, FILE *fp)
{
	fprintf(fp, "# numColor = %d\n", numColor);

	for(int i=0;i<p.size();i++){
		vector<int> pi;
		for(auto it_color=p[i].begin();it_color!=p[i].end();it_color++){
			pi.push_back(*it_color);
		}
		sort(pi.begin(),pi.end());
		for(int j=0;j<pi.size();j++){
			fprintf(fp,"%d ",pi[j]);
		}
		fprintf(fp, "\n"); 
	}
}

vector<unordered_set<int>> IntColoringSaveLoad::Load(FILE *fp)
{
	vector<unordered_set<int>> returnvalue;
	//char buffer[65535];
	//fgets(buffer, 65535, fp);
	
	vector<string> lines = fReadAllLines(fp);
	
	for(int i=0;i<lines.size();i++){
		string &line = lines[i];
		if (line[0]=='#') continue;
		
		auto lineInts = LineToArray(line);
		
		unordered_set<int> temp(lineInts.begin(),lineInts.end());
		returnvalue.push_back(temp);
	}

	return returnvalue;
}

