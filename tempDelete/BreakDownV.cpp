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

//blocks is the typers of the words
//no placeholders for punctuation
//it all about finding where parts of the sentence fit
//todo testing
LinkedList<POS, int> posToBlocks(vector<string> wordStrings, vector<POS> blocks)
{
	LinkedList<POS, int> typeBlocks;
	/*int startIndex = 0; //the next available thing to add it is 0 based
	bool articleIndex = false;
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i] == POS::Article) {
			startIndex = i;
		}
		else if (!articleIndex && blocks[i] == Adjective || blocks[i] == Verb || blocks[i] == Conjuction || blocks[i] == Preposition) {
			startIndex = i + 1;
		}
		else if (blocks[i] == Noun) {
			for (int a = startIndex; a < i; a++) {
				blocks.erase(blocks.begin() + a);
			}
		}
	}

	cout << "good stuff";

	//create verb blocks
	for (int i = blocks.size() - 1; i - 1 >= 0; i--) {
		if (blocks[i] == Verb && blocks[i - 1] == Adjective) {
			blocks.erase(blocks.begin() + i - 1);
		}
	}
	cout << "good stuff";
	//run fat cat | run quickly jared
	POS lastSignificantPOS = POS::Unknown;
	for (int i = 0; i < blocks.size(); i++) {
		POS type = blocks.at(i);

		if (type == POS::Conjuction | type == POS::Preposition | type == POS::Adjective) {
			if (lastSignificantPOS != POS::Unknown) {
				typeBlocks.add(lastSignificantPOS, i - 1);
			}

			typeBlocks.add(type, i);

			lastSignificantPOS = POS::Unknown;
		}
		else if (type == POS::Verb | type == POS::Noun | type == POS::Article) {
			if (type == POS::Noun & lastSignificantPOS == POS::Noun) {
				continue;
			}

			if (lastSignificantPOS != POS::Unknown) {
				typeBlocks.add(lastSignificantPOS, i - 1);
			}

			lastSignificantPOS = type == POS::Article ? POS::Noun : type;
		}
	}

	if (lastSignificantPOS != POS::Unknown) {
		typeBlocks.add(lastSignificantPOS, blocks.size() - 1);
	}

	for (int i = 0; i < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		if (i > 0 && type == POS::Adjective) {
			POS secondary = typeBlocks.getKeyI(i - 1);

			if (secondary == POS::Adjective) {
				typeBlocks.changeValueI(i, typeBlocks.getValueI(i - 1));
			}
		}
	}

	//find prep phrases
	for (int i = 0; i + 1 < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		POS secondary = typeBlocks.getKeyI(i + 1);
		if (type == POS::Preposition && secondary == POS::Noun) {
			typeBlocks.changeValueI(i, typeBlocks.getValueI(i + 1));
			typeBlocks.deleteIndex(i + 1);
		}
	}

	for (int i = 0; i < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		if (type == POS::Adjective) {
			if (i > 0) {
				int beginning = typeBlocks.getValueI(i - 1);
				int end = typeBlocks.getValueI(i);

				bool changed = false;

				POS newType = POS::Noun;
				for (int b = beginning; b <= end; b++) {
					if (wordStrings.at(b).find("ly") == std::string::npos) {
						newType = POS::Verb;

						if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Verb) {
							typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
							typeBlocks.deleteIndex(i);
							changed = true;
						}
						else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Verb) {
							typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
							typeBlocks.deleteIndex(i);
							changed = true;
						}

						break;
					}
				}

				if (changed == false) {
					if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Noun) {
						typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Noun) {
						typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
				}

				int iterations = 0;
				while (changed == false && iterations < 3) {
					if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Noun) {
						typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Noun) {
						typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Verb) {
						typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Verb) {
						typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}

					iterations++;
				}
			}
		}
		else if (type == POS::Preposition) {
			if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Verb) {
				typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
				typeBlocks.deleteIndex(i);
			}
			else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Verb) {
				typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
				typeBlocks.deleteIndex(i);
			}
		}
	}
	*/
	//this creates the typeblocks
	
	for (int i = 0; i < blocks.size(); i++) {
		typeBlocks.add(blocks[i], i);
	}

	//deletes joining words by looking for conjunctions without punctuation and deleting them from the blocks
	for (int i = 0; i < wordStrings.size(); i++) {
		if (stringToPunctuation(wordStrings[i]) != Punctuation::UNDEFINED) {
			continue;
		}

		if (typeBlocks.getKeyI(i) == Conjuction) {
			if (stringToPunctuation(wordStrings[i - 1]) != Punctuation::UNDEFINED) {
				typeBlocks.deleteIndex(typeBlocks.getPostionV(i));
				continue;
			}
		}
	}

	//joins adverbs with either verbs (1st priority) or adjectives
	for (int i = 0; i < typeBlocks.size(); i++) {
		if (typeBlocks.getKeyI(i) == Adverb) {
			if (i > 0 && typeBlocks.getKeyI(i - 1) == Verb) {
				typeBlocks.changeKeyI(i, Verb);
				typeBlocks.deleteIndex(i - 1);
			}
			else if (i < typeBlocks.size()-1 && typeBlocks.getKeyI(i + 1) == Verb) {
				typeBlocks.deleteIndex(i);
			}
			else if (i < typeBlocks.size() - 1 && typeBlocks.getKeyI(i + 1) == Adjective) {
				typeBlocks.deleteIndex(i);
			}
			else if (i > 0 && typeBlocks.getKeyI(i - 1) == Adjective) {
				typeBlocks.changeKeyI(i, Adjective);
				typeBlocks.deleteIndex(i - 1);
			}
		}
	}



	//this removes all the doubles
	for (int i = 0; i < typeBlocks.size() - 1; i++) {
		if (typeBlocks.getKeyI(i) == typeBlocks.getKeyI(i+1)) {
			typeBlocks.deleteIndex(i+1);
		}
	}

	//create noun blocks
	int startPos = -1;
	for (int i = 0; i < typeBlocks.size(); i++) {
		if (typeBlocks.getKeyI(i) == Article) {
			startPos = i;
		}
		else if (startPos == -1 && typeBlocks.getKeyI(i) == Noun) { //check for something like: his fat cat
			for (int a = i + 1; a < typeBlocks.size(); a++) {
				if (typeBlocks.getKeyI(a) != Noun & typeBlocks.getKeyI(a) != Adjective) {
					break;
				}
				else if (typeBlocks.getKeyI(a) == Noun) {
					for (int b = i; b< a; b++) {
						typeBlocks.deleteIndex(i);
					}
				}
			}
		}
		else if (startPos != -1 & typeBlocks.getKeyI(i) == Noun) { //checks for: the fat cat
			for (int a = startPos; a < i; a++) {
				typeBlocks.deleteIndex(startPos);
			}

			startPos = -1;
		}
	}
	
	//verb blocks
	for (int i = 0; i < typeBlocks.size(); i++) {
		if (typeBlocks.getKeyI(i) == Verb) {
			if (i > 0 && typeBlocks.getKeyI(i) == Adjective) {
				typeBlocks.deleteIndex(i-1);
			}
			else if (i < typeBlocks.size() - 1 && typeBlocks.getKeyI(i + 1) == Adjective) {
				typeBlocks.changeKeyI(i+1, Verb);
				typeBlocks.deleteIndex(i);
			}
		}
	}

	return typeBlocks;
}

//searches for integer existence
bool binaryExist(vector<int> input, int target) {
	if (input.size() == 0) {
		return 0;
	}
	int middle = (input.size())/2;

	if (input[middle] == target) {
		return true;
	}
	else if (input.size() == 1 && input[0] != target) {
		return false;
	}
	else if (input[middle] > target) {
		for (int i = middle; i < input.size(); i++) {
			input.erase(input.begin()+middle);
		}

		return binaryExist(input, target);
	}
	else if (input[middle] < target) {
		for (int i = 0; i <= middle; i++) {
			input.erase(input.begin());
		}

		return binaryExist(input, target);
	}
}