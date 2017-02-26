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

vector<string> breakDownV(string phrase, vector<char> delimeter) {
	vector<string> answers;
	string stuff;
	int beginInt = 0;
	int closestPos = -1;

	for (int i = 0; i < delimeter.size(); i++) {
		if (closestPos > phrase.find(delimeter.at(i))) {
			closestPos = phrase.find(delimeter.at(i));
		}
	}

	if (closestPos == -1) {
		answers.push_back(phrase);
		return answers;
	}

	do {
		answers.push_back(phrase.substr(0, closestPos+1));
		beginInt = closestPos + 1;
		phrase = phrase.substr(beginInt, phrase.size());

		closestPos = 1000;
		for (int i = 0; i < delimeter.size(); i++) {
			int delimeterPos = phrase.find(delimeter.at(i));
			if (closestPos > delimeterPos && delimeterPos != -1) {
				closestPos = delimeterPos;
			}
		}
	} while (closestPos != -1 && closestPos != 1000);

	answers.push_back(phrase);

	return answers;
}

int stringToInt(string s) {
	int value = 0;
	for (int i = 0; i < s.length(); i++) {
		value += s[i];
	}

	return value;
}

string trimWhite(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ') {
			s = s.substr(0, i) + s.substr(i + 1, string::npos);
		}
	}

	return s;
}