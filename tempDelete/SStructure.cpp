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

void SStructure::addItems(vector<POS> input) {
	component = input;
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