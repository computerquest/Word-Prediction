#pragma once
#include "SStructure.h"
using namespace std;

SStructure::SStructure() {}

SStructure::SStructure(vector<POS> input) {
	component = input;
}

bool SStructure::operator==(SStructure rightside) {
	if (rightside.component.size() == component.size()) {
		for (int i = 0; i < rightside.component.size() && i < component.size(); i++) {
			if (rightside.component.at(i) != component.at(i)) {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void SStructure::addItem(POS type) {
	component.push_back(type);
}

void SStructure::setComponent(vector<POS> input) {
	component = input;
}

void SStructure::addPunctuation(Punctuation p) {
	if (punctuation.containsV(p)) {
		int pIndex = punctuation.getPostionV(p);
		punctuation.changeKeyI(pIndex, punctuation.getKeyI(pIndex) + 1);
	}
	else {
		punctuation.add(1, p);
	}
}

void SStructure::addEnding(Punctuation p) {
	if (ending.containsV(p)) {
		int pIndex = ending.getPostionV(p);
		ending.changeKeyI(pIndex, ending.getKeyI(pIndex) + 1);
	}
	else {
		ending.add(1, p);
	}
}
vector<string> SStructure::structToString() {
	vector<string> answer;
	for (int i = 0; i < component.size(); i++) {
		answer.push_back(POStoString((component.at(i))));
	}

	return answer;
}

POS SStructure::getType(int i) {
	return component.at(i);
}

vector<POS> SStructure::getBlocks()
{
	return vector<POS>();
}

vector<POS> SStructure::parseBlocks(vector<POS> blocks)
{
	LinkedList<POS, int> typeBlocks;
	int startIndex = 0; //the next available thing to add it is 0 based
	bool articleIndex = false;

	//builds the major blocks by deleting small ones
	//major noun blocks
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

	//create verb blocks
	for (int i = blocks.size() - 1; i - 1 >= 0; i--) {
		if (blocks[i] == Verb && blocks[i - 1] == Adjective) {
			blocks.erase(blocks.begin() + i - 1);
		}
	}

	//run fat cat | run quickly jared
	//this puts the blocks into typeblocks with indexes
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

	//this is just to add the last type
	if (lastSignificantPOS != POS::Unknown) {
		typeBlocks.add(lastSignificantPOS, blocks.size() - 1);
	}

	//remove doubles (adjective)
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

	//this checks to see if it goes with the verb or the noun block
	for (int i = 0; i < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		if (type == POS::Adjective) {
			if (i > 0) {
				int beginning = typeBlocks.getValueI(i - 1);
				int end = typeBlocks.getValueI(i);

				bool changed = false;

				POS newType = POS::Noun;
				for (int b = beginning; b <= end; b++) {
					if (wordStrings.at(b).find("ly") == std::string::npos) { //ly indicates adverb
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

	return typeBlocks;
}
