#include "ColoringSaveLoad.h"

#include <algorithm>
#include "fileio0.h"
#include "StringTokenizer.h"

void ColoringSaveLoad::save(const vector<unordered_set<int>> &p, int numColor, int numFragment, FILE *fp)
{
	fprintf(fp, "# numColor = %d, numFragment = %d\n", numColor ,numFragment);

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

vector<unordered_set<int>> ColoringSaveLoad::load(FILE *fp)
{
	vector<unordered_set<int>> returnvalue;
	char buffer[65535];
	fgets(buffer, 65535, fp);

	while(!feof(fp)){
		unordered_set<int> temp;
		
		fgets(buffer, 65535, fp);
		StringTokenizer stok(buffer," ");

		while (stok.hasMoreTokens()){
			temp.insert(stok.nextIntToken());
		}
		returnvalue.push_back(temp);
	}
	return returnvalue;
}