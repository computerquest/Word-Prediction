#include "BreakDown.h"
using namespace std;

vector<string> breakDownV(string phrase) {
	vector<string> answers;
	string stuff;
	int beginInt = 0;
	do {
		answers.push_back(phrase.substr(0, phrase.find(" ")));
		beginInt = phrase.find(" ") + 1;
		phrase = phrase.substr(beginInt, phrase.size());
	} while (phrase.find(" ") != -1);

	answers.push_back(phrase);

	return answers;
}