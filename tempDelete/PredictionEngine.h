#pragma once
#include <iostream>
#include <vector>
#include "LinkedList.h"
#include "POS.h"
#include "vectorContains.h"
#include "MLP.h"
#include "NGram.h"
#include "SStructure.h"
#include "MasterResource.h"
#include "BreakDown.h"
#include "Word.h"
#include <fstream>
using namespace std;

class PredictionEngine {
public:
	MLP nn;
	LinkedList<vector<double>, vector<double>> training;
	vector<string> examples;
	MLP predictNN;
	MasterResource master;
	vector<POS> lastPhraseGlobal;

	vector<POS> multipleMissing(string phrase);
	vector<POS> multipleMissingTraining(string phrase);

	void mergeTesting(string phrase);

	PredictionEngine(int inputN, vector<int> hidden, int output, bool read, bool batch);

	void saveTraining();
	void readTraining();

	void createTraining();

	POS findTypeDeployment(vector<POS> wtype, string phrase, int i);

	void filterExamples();
}; //class end 