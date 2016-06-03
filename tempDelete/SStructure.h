#pragma once
#include <vector>
#include "POS.h"
using namespace std;

class SStructure {
public:
	vector<POS> component;

	SStructure();

	SStructure(vector<POS> input);

	bool operator==(SStructure rightside);

	void addItem(POS type);

	void addItems(vector<POS> input);

	vector<string> structToString();

	POS getType(int i);
};


