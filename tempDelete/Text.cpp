#include "Text.h"
#include "BreakDown.h"

void Text::read()
{
	rawFile.push_back(file.read());
}

/*vector<vector<string>> Text::breakText()
{
	vector<char> delimeter;
	delimeter.push_back('(');
	delimeter.push_back(')');
	//delimeter.push_back('\"');
	delimeter.push_back('.');
	delimeter.push_back('?');
	delimeter.push_back('!');

	//vector<vector<string>> tempRaw;
	for (int i = 0; i < rawFile.size(); i++) {
		rawFile.at(i) = breakDownV(rawFile.at(i).at(0), delimeter);
	}

	//rawFile = tempRaw;

	return rawFile;
}*/

void Text::trimEndsWhite()
{
	for (int i = 0; i < rawFile.size(); i++) {
		for (int a = 0; a < rawFile.at(i).size(); a++) {
			if (rawFile.at(i).at(a).length() <= 1) {
				continue;
			}
			while (rawFile.at(i).at(a)[0] == ' ' && rawFile.at(i).at(a).length() > 0) {
				rawFile.at(i).at(a) = rawFile.at(i).at(a).substr(1, string::npos);
			}
			while (rawFile.at(i).at(a)[rawFile.at(i).at(a).length() - 1] == ' ' && rawFile.at(i).at(a).length() > 0) {
				rawFile.at(i).at(a) = rawFile.at(i).at(a).substr(0, rawFile.at(i).at(a).length() - 1);
			}
		}
	}

	for (int i = 0; i < dialogue.size(); i++) {
		if (dialogue.at(i).length() <= 1) {
			continue;
		}
		while (dialogue.at(i)[0] == ' ' && dialogue.at(i).length() > 0) {
			dialogue.at(i)= dialogue.at(i).substr(1, string::npos);
		}
		while (dialogue.at(i)[dialogue.at(i).length() - 1] == ' ' && dialogue.at(i).length() > 0) {
			dialogue.at(i) = dialogue.at(i).substr(0, dialogue.at(i).length() - 1);
		}
	}

	for (int i = 0; i < exposition.size(); i++) {
		if (exposition.at(i).length() <= 1) {
			continue;
		}
		while (exposition.at(i)[0] == ' ' && exposition.at(i).length() > 0) {
			exposition.at(i) = exposition.at(i).substr(1, string::npos);
		}
		if (exposition.at(i).length() <= 1) {
			continue;
		}
		while (exposition.at(i)[exposition.at(i).length() - 1] == ' ' && exposition.at(i).length() > 0) {
			exposition.at(i) = exposition.at(i).substr(0, exposition.at(i).length() - 1);
		}
	}
}

//change so only works on dialogue
void Text::filterDelimeter(char character)
{
	for (int i = 0; i < dialogue.size(); i++) {
		for (int a = 0; a < dialogue.at(i).length(); a++) {
			if (character == dialogue.at(i)[a]) {
				dialogue.at(i).erase(dialogue.at(i).begin() + a);
			}
		}
	}
}

void Text::filiterDataSize(int minSize)
{
	for (int i = 0; i < rawFile.size(); i++) {
		for (int a = 0; a < rawFile.at(i).size(); a++) {
			if (rawFile.at(i).at(a).length() < minSize) {
				rawFile.at(i).erase(rawFile.at(i).begin() + a);
			}
		}
	}
}

void Text::breakText()
{
	vector<char> delim;
	delim.push_back('.');
	delim.push_back('(');
	delim.push_back(')');
	delim.push_back('?');
	delim.push_back('!');
	/*for (int i = 0; i < rawFile.size(); i++) {
		//for (int a = 0; a < rawFile.at(i).size(); a++) {
		vector<string> newV = breakDownV(rawFile.at(i).at(0), delim);
		newV.erase(newV.begin() + newV.size() - 1);

		rawFile.push_back(newV);
		rawFile.erase(rawFile.begin()+i);
		//a--;
		//}
	}*/

	int exposSize = exposition.size();
	for (int i = 0; i < exposSize; i++) {
		vector<string> newV = breakDownV(exposition.at(i), delim);

		for (int a = 0; a < newV.size(); a++) {
			if (newV.at(a) != "" && newV.at(a) != " ") {
				exposition.push_back(newV.at(a));
			}
		}
	}

	int dialogueSize = dialogue.size();
	for (int i = 0; i < dialogueSize; i++) {
		vector<string> newV = breakDownV(dialogue.at(i), delim);

		for (int a = 0; a < newV.size(); a++) {
			if (newV.at(a) != "" && newV.at(a) != "\""  && newV.at(a) != " ") {
				dialogue.push_back(newV.at(a));
			}
		}
	}
}

void Text::joinCharacters()
{
	/*for (int i = 0; i < rawFile.size(); i++) {
		for (int a = 0; a + 1 < rawFile.at(i).size(); a++) {
			char lastChar = rawFile.at(i).at(a)[rawFile.at(i).at(a).length() - 1];
			if (rawFile.at(i).size() > a + 1 && lastChar == '(' || lastChar == '[' || lastChar == '<' || lastChar == '{') { //lastChar == '\"' & rawFile.at(i).at(a+1)[rawFile.at(i).at(a+1).length() - 1] == '\"'
				rawFile.at(i).at(a + 1).insert(rawFile.at(i).at(a + 1).begin(), lastChar);
				rawFile.at(i).at(a) = rawFile.at(i).at(a).substr(0, rawFile.at(i).at(a).length() - 1); //uses index to start and then the next pararmeter is the number of spaces the string will use
			}
		}
	}*/

	for (int i = 0; i + 1 < exposition.size(); i++) {
		char lastChar = exposition.at(i)[exposition.at(i).length() - 1];
		if (exposition.size() > i + 1 && lastChar == '(' || lastChar == '[' || lastChar == '<' || lastChar == '{') { //lastChar == '\"' & rawFile.at(i).at(a+1)[rawFile.at(i).at(a+1).length() - 1] == '\"'
			exposition.at(i + 1).insert(exposition.at(i + 1).begin(), lastChar);
			exposition.at(i) = exposition.at(i).substr(0, exposition.at(i).length() - 1); //uses index to start and then the next pararmeter is the number of spaces the string will use
		}
	}

	for (int i = 0; i + 1 < dialogue.size(); i++) {
		char lastChar = dialogue.at(i)[dialogue.at(i).length() - 1];
		if (dialogue.size() > i + 1 && lastChar == '(' || lastChar == '[' || lastChar == '<' || lastChar == '{') { //lastChar == '\"' & rawFile.at(i).at(a+1)[rawFile.at(i).at(a+1).length() - 1] == '\"'
			dialogue.at(i + 1).insert(dialogue.at(i + 1).begin(), lastChar);
			dialogue.at(i) = dialogue.at(i).substr(0, dialogue.at(i).length() - 1); //uses index to start and then the next pararmeter is the number of spaces the string will use
		}
	}
}

void Text::dialogueFilter()
{
	for (int i = 0; i < rawFile.size(); i++) {

		for (int a = 0; a < rawFile.at(i).size(); a++) {
			string current = rawFile.at(i).at(a);

			if (current.find('\"') != current.npos) {
				vector<string> addTo;
				int beforeSize = exposition.size();
				while (current.find('\"') != string::npos) {
					int qoutePos = current.find('\"');
					int secondQoute = current.find('\"', qoutePos + 1);
					string before = current.substr(0, qoutePos);
					string dialogueS = current.substr(qoutePos, secondQoute + 1 - qoutePos);
					string after = current.substr(secondQoute + 1, string::npos);
					current = after;

					if (dialogueS != "\" \"") {
						dialogue.push_back(dialogueS);

					}

					if (before != "" && before != " ") {
						exposition.push_back(before);
					}
				}

				/*rawFile.at(i).erase(rawFile.at(i).begin() + a);
				for (int b = 0; b < addTo.size(); b++) {
					rawFile.at(i).insert(rawFile.at(i).begin() + a + b, addTo.at(b));
				}

				rawFile.at(i).insert(rawFile.at(i).begin() + a + addTo.size(), current);*/

				if (current != "" && current != " ") {
					exposition.push_back(current);
				}

				a += exposition.size() - beforeSize + 1;
			}

		}
	}
}

void Text::addWhiteSpace()
{
	vector<char> delimeter;
	delimeter.push_back(',');
	delimeter.push_back(';');
	delimeter.push_back(':');
	delimeter.push_back('.');
	delimeter.push_back('!');
	delimeter.push_back('?');
	/*for (int i = 0; i < rawFile.size(); i++) {
		for (int a = 0; a < rawFile.at(i).size(); a++) {
			string& currentString = rawFile.at(i).at(a);
			//int length = currentString.length();
			for (int b = 0; b < currentString.length(); b++) {
				for (int c = 0; c < delimeter.size(); c++) {
					if (currentString[b] == delimeter[c]) {
						currentString.insert(b, " ");
						b += 2;
						//currentString.insert()
					}
				}
			}
		}
	}*/

	for (int i = 0; i < exposition.size(); i++) {
		string& currentString = exposition.at(i);
		//int length = currentString.length();
		for (int b = 0; b < currentString.length(); b++) {
			for (int c = 0; c < delimeter.size(); c++) {
				if (currentString[b] == delimeter[c]) {
					currentString.insert(b, " ");
					b += 2;
					//currentString.insert()
				}
			}
		}
	}

	for (int i = 0; i < dialogue.size(); i++) {
		string& currentString = dialogue.at(i);
		//int length = currentString.length();
		for (int b = 0; b < currentString.length(); b++) {
			for (int c = 0; c < delimeter.size(); c++) {
				if (currentString[b] == delimeter[c]) {
					currentString.insert(b, " ");
					b += 2;
					//currentString.insert()
				}
			}
		}
	}
}

void Text::joinDialogue() {
	for (int i = 0; i < rawFile.size(); i++) {
		for (int a = 0; a < rawFile.at(i).size(); a++) {
			string current = rawFile.at(i).at(a);

			int count = 0;
			for (int b = 0; b < current.length(); b++) {
				if (current[b] == '\"') {
					count++;
				}
			}

			if (count % 2 != 0 && count != 0) {
				string addition = rawFile.at(i + 1).at(0);
				addition.erase(addition.find('\"'), 1);

				rawFile.at(i).at(a) = current + addition;

				rawFile.erase(rawFile.begin() + i + 1);

				a--;
			}
		}
	}
}

void Text::textParagraph()
{
	for (int i = 0; i < rawFile.size(); i++) {
		for (int a = 0; a < rawFile.at(i).size(); a++) {
			string s = rawFile.at(i).at(a);
			if (s.length() > 0 && s[s.length() - 1] == ':' && i + 1 < rawFile.size()) {
				string newP = rawFile.at(i + 1).at(0);
				int iMod = 2;
				while (i + iMod < rawFile.size() && rawFile.at(i + iMod).size() == 1 && rawFile.at(i + iMod).at(0).length() < 75) {
					newP += rawFile.at(i + iMod).at(a);
					rawFile.erase(rawFile.begin() + i + iMod);
					//iMod++;
				}

				rawFile.at(i + 1).at(0) = newP;
			}
		}
	}
}

void Text::createParagraph() {
	//vector<string> paragraphs;
	int startSize = rawFile.size();
	for (int i = 0; i < startSize; i++) {
		for (int a = 0; a < rawFile.at(i).size(); a++) {
			if (rawFile.at(i).at(a) != "") {
				string p = "";
				do {
					p += " " + rawFile.at(i).at(a);
					a++;
				} while (a < rawFile.at(i).size() && rawFile.at(i).at(a) != "");

				vector<string> temp;
				temp.push_back(p);
				rawFile.push_back(temp);
				//paragraphs.push_back(p);
				a--;
			}
		}
	}

	rawFile.erase(rawFile.begin());
}