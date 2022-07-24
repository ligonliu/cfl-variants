#include "graph.h"
#include "fileio0.h"
#include <cassert>


Graph::Graph(int _count_of_vertices)
{
	edges.resize(_count_of_vertices);
}

int Graph::Count_of_Vertices() const
{
	return edges.size();
}

int Graph::AddVertex() 
{
	unordered_set<int> temp;
	edges.push_back(temp);
	return edges.size();
}

void Graph::AddEdge(int v1, int v2)
{
	assert(v1<edges.size() && v2<edges.size());

	edges[v1].insert(v2);
	edges[v2].insert(v1);
}

int Graph::MaxDegree() const
{

	int max=0;
	for(int i=0;i<edges.size();i++){
		const unordered_set<int> &es = edges[i];
		if (max<es.size()) max=es.size();
	}
	return max;
}

void Graph::Save(FILE *writeTo) const {
    for(int i=0;i<edges.size();i++){
        vector<int> edgei(edges[i].begin(),edges[i].end());
        /*
        for each (int neigbor in edges[i]){
            edgei.push_back(neigbor);
        }
         */
        sort(edgei.begin(),edgei.end());

        for(int j=0;j<edgei.size();j++){
            fprintf(writeTo, "%d ",edgei[j]);
        }
        fprintf(writeTo, "\n");
    }
}

Graph Graph::Load(FILE* readFrom)
{
	Graph returnvalue;
	
	/*while(!feof(readFrom)){
	 * 
	 * 
		char buffer[65535];
		  
		unordered_set<int> vertex;
		
		fgetline(buffer, 65535, readFrom);

		string buffer_str(buffer);

		StringTokenizer  tokenizer(buffer_str," ");
		
		while(tokenizer.hasMoreTokens()){
			vertex.insert(tokenizer.nextIntToken());
		}
		returnvalue.edges.push_back(vertex);
		
	}
	 */
	
	//fpos64_t begin_pos;
	//fgetpos(readFrom, &begin_pos);
	
	vector<string> lines = fReadAllLines(readFrom);
	
	int currentVertex = 0;
	
	map<int, vector<int> > tempmap;
	
	for(int i=0;i<lines.size();i++){
		string &line = lines[i];
		if (line[0]=='#') continue;
	
		auto lineInts = LineToArray(line);
		
		// unordered_set<int> temp(lineInts.begin(),lineInts.end());
		
		tempmap[i] = lineInts;
		
		//returnvalue.push_back(temp);
	}
	
	for(int i=0;i<tempmap.size();i++){
		returnvalue.AddVertex();
	}
	
	for(auto it=tempmap.begin();it!=tempmap.end();it++){
		int i=it->first;
		const vector<int> &adjListi = it->second;
		for(int j=0;j<adjListi.size();j++){
			returnvalue.AddEdge(i,adjListi[j]);
		}
	}

	return returnvalue;
}