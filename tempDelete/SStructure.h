#pragma once
#include <vector>
#include "LinkedList.h"
#include "Punctuation.h"
#include "POS.h"
using namespace std;

class SStructure {
public:
	vector<POS> component;
	LinkedList<int, Punctuation> ending; //this is like ngram for the possible endings
	LinkedList<int, Punctuation> punctuation; //this is the punctuation throughout int is position to insert it 
	SStructure();

	SStructure(vector<POS> input);

	bool operator==(SStructure rightside);
	
	void addItem(POS type);

	void addItems(vector<POS> input);

	void addPunctuation(Punctuation p);

	void addEnding(Punctuation p);

	vector<string> structToString();

	POS getType(int i);
};


