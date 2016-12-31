#pragma once
#include <string>
#include "POS.h"
#include "BreakDown.h"
using namespace std;

class Word {
public:
	string name;
	POS type;

	bool operator==(Word leftside);
	//
	bool operator==(string leftside);

	Word();

	Word(string word);
	Word(string word, POS typeInput);

	void setWord(string word);

	void setType(POS input);

	string getWord();

	POS getType();

	int wordValue();
};


