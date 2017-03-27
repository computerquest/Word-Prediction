#include "POS.h"
using namespace std;

POS toType(string type) {
	if (type == "noun") {
		return POS::Noun;
	}
	else if (type == "verb") {
		return POS::Verb;
	}
	else if (type == "unknown") {
		return POS::Unknown;
	}
	else if (type == "adjective") {
		return POS::Adjective;
	}
	else if (type == "preposition") {
		return POS::Preposition;
	}
	else if (type == "conjunction") {
		return POS::Conjuction;
	}
	else if (type == "article") {
		return POS::Article;
	}
}

string POStoString(POS typeInput) {
	if (typeInput == POS::Noun) {
		return "noun";
	}
	else if (typeInput == POS::Verb) {
		return "verb";
	}
	else if (typeInput == POS::Unknown) {
		return "unknown";
	}
	else if (typeInput == POS::Adjective) {
		return "adjective";
	}
	else if (typeInput == POS::Preposition) {
		return "preposition";
	}
	else if (typeInput == POS::Conjuction) {
		return "conjunction";
	}
	else if (typeInput == POS::Article) {
		return "article";
	}
}

int POStoInt(POS typeInput)
{
	if (typeInput == POS::Noun) {
		return 0;
	}
	else if (typeInput == POS::Verb) {
		return 1;
	}
	else if (typeInput == POS::Unknown) {
		return 2;
	}
	else if (typeInput == POS::Adjective) {
		return 3;
	}
	else if (typeInput == POS::Preposition) {
		return 4;
	}
	else if (typeInput == POS::Conjuction) {
		return 5;
	}
	else if (typeInput == POS::Article) {
		return 6;
	}
}

POS intToPOS(int i)
{
	if (i == 0) {
		return Noun;
	}
	else if (i == 1) {
		return Verb;
	}
	else if (i == 2) {
		return Unknown;
	}
	else if (i == 3) {
		return Adjective;
	}
	else if (i == 4) {
		return Preposition;
	}
	else if (i == 5) {
		return Conjuction;
	}
	else if (i == 6) {
		return Article;
	}
}
