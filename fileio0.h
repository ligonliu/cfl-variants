#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

char *fgetline(char *s,int size,FILE *stream);

vector<string> fReadAllLines(FILE *fp);

void tokenize(const std::string& str, vector<string> &out_tokens, const std::string &delimiters = " ", const bool trimEmpty = true);

vector<int> LineToArray(const string &line);

vector<char> LineToByteArray(const string &line);

vector<vector<int> > LinesToArrays(const vector<string> &lines);

vector<vector<char> > LinesToByteArrays(const vector<string> &lines);

int ReadMatrix(string FileName, vector<vector<char> > &outMatrix);

int WriteMatrix(string FileName, const vector<vector<char> > &Matrix);



string ArrayToLine(const vector<int> &Array);

string ArrayToLine(const vector<char> &Array);


typedef map<vector<int>, vector<vector<char> > > relationDict;

int ReadRelations(string fileName, relationDict &outRelDict);

int WriteRelations(string fileName, const relationDict &relDict);


template<class T>
void FreeMemory(T &t)
{
    T tmp(0);
    t.swap(tmp);
}

