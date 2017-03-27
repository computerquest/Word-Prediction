#pragma once
#include <string>
using namespace std;

enum POS{
	Noun, Verb, Adjective, Conjuction, Preposition, Article, Unknown
};

POS toType(string type);

string POStoString(POS typeInput);

int POStoInt(POS p);

POS intToPOS(int i);