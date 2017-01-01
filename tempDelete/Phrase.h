#pragma once
#pragma once
#include <iostream>
#include <vector>
#include "POS.h"
#include "Word.h"
#include "breakdown.h"
#include "MasterResource.h"
#include "SStructure.h"
using namespace std;

class MasterResource;
class Phrase {
public:
	SStructure structure;
	string phrase;
	vector<Word> wordsW;
	vector<POS> wordPOS;
	vector<string> wordString;
	MasterResource* resource;

	Phrase();
	Phrase(string phraseInput);

	bool contains(Word word);
	bool contains(string word);

	bool operator==(string rightside);

};