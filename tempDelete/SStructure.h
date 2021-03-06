#pragma once
#include <vector>
#include "LinkedList.h"
#include "Punctuation.h"
#include "POS.h"
#include <map>
using namespace std;

//this has been simplified to using basic blocks
class SStructure {
public:
	vector<POS> component;
	LinkedList<int, Punctuation> ending; //this is like ngram for the possible endings
	LinkedList<int, Punctuation> punctuation; //this is the punctuation throughout int is position to insert it 
	SStructure();

	SStructure(vector<POS> input);

	bool operator==(SStructure rightside);
	
	void addItem(POS type);

	void setComponent(vector<POS> input);
	void setComponent(map<int, POS> input);

	void addPunctuation(Punctuation p);

	void addEnding(Punctuation p);

	vector<string> structToString();

	POS getType(int i);

	vector<POS> getBlocks();
};


