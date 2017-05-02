#pragma once
#include "Word.h"
using namespace std;


bool Word::operator==(Word leftside) {
	if (leftside.name == name && type == leftside.type) {
		return true;
	}
	else {
		return false;
	}
}

bool  Word::operator==(string leftside) {
	if (leftside == name) {
		return true;
	}
	else {
		return false;
	}
}

Word::Word() {}

Word::Word(string word) : name(word) {}
Word::Word(string word, POS typeInput) : name(word), type(typeInput) {}

void  Word::setWord(string word) {
	name = word;
}

void  Word::setType(POS input) {
	type = input;
}

string  Word::getWord() {
	return name;
}

POS  Word::getType() {
	return type;
}

int Word::wordValue()
{
	return stringToInt(name);
}