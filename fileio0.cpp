#include "fileio0.h"
#include <sstream>
#include <fstream>
#include <assert.h>
#include <string.h>

using namespace std;


char *fgetline(char *s,int size,FILE *stream)
{
	int len;
	if(fgets(s,size,stream)==NULL) return NULL;
	else{
		len=strlen(s);
		s[len-1]= '\0';
		return s;
	}
}

vector<string> fReadAllLines(FILE *fp)
{
	char temp[65535];
	vector<string> returnvalue;
	while(!feof(fp)){
		if(NULL==fgetline(temp,65535,fp)) continue;
		returnvalue.push_back(temp);
	}
	return returnvalue;
}


void tokenize(const std::string& str, vector<string> &out_tokens, const std::string& delimiters, const bool trimEmpty)
{
   std::string::size_type pos, lastPos = 0;

	out_tokens.clear();

   while(true)
   {
      pos = str.find_first_of(delimiters, lastPos);
      if(pos == std::string::npos)
      {
         pos = str.length();

         if(pos != lastPos || !trimEmpty)
		 {
			 string temp1 = str.substr(lastPos,pos-lastPos);
			 assert(pos>lastPos);
			 assert(temp1.length()>0);
			 out_tokens.push_back(temp1);
		 }

         break;
      }
      else
      {
         if(pos != lastPos || !trimEmpty)
	     {
			 string temp1 = str.substr(lastPos,pos-lastPos);
			 assert(pos>lastPos);
			 assert(temp1.length()>0);	
			 
			 out_tokens.push_back(temp1.c_str());
		 }
      }

      lastPos = pos + 1;
   }
};

vector<int> LineToArray(const string &line)
{
	vector<string> split;
	tokenize(line,split," ,");
	vector<int> returnvalue;
	
	for(int i=0;i<split.size();i++){
		returnvalue.push_back(atoi(split[i].c_str()));
	}
	return returnvalue;
}

vector<char> LineToByteArray(const string &line)
{
	vector<string> split;
	tokenize(line,split," ,",true);
	vector<char> returnvalue;
	
	for(int i=0;i<split.size();i++){
		returnvalue.push_back(atoi(split[i].c_str()));
	}
	return returnvalue;
}

vector<vector<int> >  LinesToArrays(const vector<string> &lines)
{
	vector<vector<int> > returnvalue;
	for(int i=0;i<lines.size();i++){
		returnvalue.push_back(LineToArray(lines[i]));
	}
	return returnvalue;
}

vector<vector<char> > LinesToByteArrays(const vector<string> &lines)
{
	vector<vector<char> > returnvalue;
	for(int i=0;i<lines.size();i++){
		returnvalue.push_back(LineToByteArray(lines[i]));
	}
	return returnvalue;
}

string ArrayToLine(const vector<int> &Array)
{
	string returnvalue;
	for(int i=0;i<Array.size();i++){
		stringstream istr;
		istr<<Array[i];
		returnvalue+=istr.str();
		returnvalue += ' ';
	}
	returnvalue.resize(returnvalue.size()-1);
	return returnvalue;
}

string ArrayToLine(const vector<char> &Array)
{
	string returnvalue;
	for(int i=0;i<Array.size();i++){
		stringstream istr;
		istr<<((int)Array[i]);
		returnvalue += istr.str();
		returnvalue += ' ';
	}
	returnvalue.resize(returnvalue.size()-1);
	return returnvalue;
}


int ReadMatrix(string fileName, vector<vector<char> > &outMatrix)
{
	char line[100000];

	try{
		
		ifstream fs(fileName);
		FreeMemory(outMatrix);	
		
		while(!fs.eof()){
			
			fs.getline(line, 100000);
			
			if(strlen(line)==0){
				continue;
			}
			outMatrix.push_back(LineToByteArray((string)line));
			
		}
		
		fs.close();
		return 0;
	}
	catch(...){
		return 1;
	}
}


int WriteMatrix(string FileName, const vector<vector<char> > &Matrix)
{
	try{
		ofstream fs(FileName);
		
		for(int i=0;i<Matrix.size();i++)
		{
			fs<<ArrayToLine(Matrix[i])<<"\n";
		}
		fs.close();
		
		return 0;
	}
	catch(...){
		return 1;
	}
}

int ReadRelations(string fileName, relationDict &outRelDict)
{
	char line[100000];
	
	try{
		ifstream fs(fileName);
		
		outRelDict.clear();
		//FreeMemory(outRelDict);
		
		bool endofParagraph=true;
		
		vector<int> indexSet;
		
		vector<vector<char> > *ptupleSet;
		
		while(!fs.eof()){
			
			fs.getline(line, 100000);
			
			if(strlen(line)==0){
				endofParagraph = true;
				continue;
			}
			
			if(endofParagraph){
				indexSet = LineToArray((string)line);
				ptupleSet = &outRelDict[indexSet];
				endofParagraph = false;
			}
			else{
				ptupleSet->push_back(LineToByteArray((string)line));
			}
		}
		
		fs.close();
		return 0;
	}
	catch(...){
		return 1;
	}
}


int WriteRelations(string fileName, const relationDict &relDict)
{
	try{
		ofstream fs(fileName);
		
		for(auto it=relDict.begin();it!=relDict.end();it++){
			const vector<int> &indexSet = it->first;
			const vector<vector<char> > &tupleSet = it->second;
			
			fs << ArrayToLine(indexSet)<<"\n";
			
			for(int i=0;i<tupleSet.size();i++){
				fs<<ArrayToLine(tupleSet[i])<<"\n";
			}
			
			fs<<"\n";
		}
		
		fs.close();
		
		return 0;
	}
	catch(...){
		return 1;
	}
}

