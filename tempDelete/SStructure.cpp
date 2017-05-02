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

void SStructure::setComponent(map<int, POS> input)
{
	for (auto s : input) {
		component.push_back(s.second);
	}
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
	for(int i = 0; i < component.size(); i++) {
		answer.push_back(POStoString((component[i])));
	}

	return answer;
}

POS SStructure::getType(int i) {
	return component.at(i);
}

vector<POS> SStructure::getBlocks()
{
	return component;
}