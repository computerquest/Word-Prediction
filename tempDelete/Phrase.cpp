#pragma once
#include "Phrase.h"
using namespace std;

Phrase::Phrase() {
	cout << "phrase created" << endl;
}
Phrase::Phrase(string phraseInput) {
	phrase = phraseInput;

	wordString = breakDownV(phraseInput);

	for (int i = 0; i < wordString.size(); i++) {
		wordsW.push_back(resource->findWord(wordString.at(i)));
		wordPOS.push_back(wordsW.at(wordsW.size() - 1).type);
	}

	structure = resource->findStructure(wordPOS);
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