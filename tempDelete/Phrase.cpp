#pragma once
#include "Phrase.h"
using namespace std;

Phrase::Phrase() {
	cout << "phrase created" << endl;
}
Phrase::Phrase(string phrase, vector<string> wordString, vector<POS> wordPOS, vector<Word> wordsW,SStructure structure) {
	this->phrase = phrase;
	this->wordString = wordString;
	this->wordPOS = wordPOS;
	this->wordsW = wordsW;
	this->structure = structure;
}

bool Phrase::contains(Word word) {
	for (int i = 0; i < wordsW.size(); i++) {
		if (wordsW.at(i) == word) {
			return true;
		}
	}

	return false;
}
bool Phrase::contains(string word) {
	for (int i = 0; i < wordString.size(); i++) {
		if (wordString.at(i) == word) {
			return true;
		}
	}

	return false;
}

bool Phrase::operator==(string rightside) {
	if (rightside == phrase) {
		return true;
	}

	return false;
}
