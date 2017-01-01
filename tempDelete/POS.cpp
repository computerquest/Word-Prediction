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