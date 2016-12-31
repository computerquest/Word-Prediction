#pragma once
#include <iostream>
#include <vector>
#include "LinkedList.h"
#include "POS.h"
#include "vectorContains.h"
#include "MLP.h"
#include "NGram.h"
#include "SStructure.h"
#include "BreakDown.h"
#include "Word.h"
#include <fstream>
using namespace std;

class MasterResource;
class PredictionEngine {
public:
	MLP nn;
	LinkedList<vector<double>, vector<double>> training;
	vector<string> examples;
	MLP predictNN;
	MasterResource* master;
	vector<POS> lastPhraseGlobal;

	//PredictionEngine(int inputN, vector<int> hidden, int output, MasterResource* resource);

	vector<POS> multipleMissing(string phrase);
	vector<POS> multipleMissingTraining(string phrase);

	void mergeTesting(string phrase);

	PredictionEngine(int inputN, vector<int> hidden, int output, MasterResource * resource, bool batch);

	void saveTraining();
	void readTraining();

	//void mergeTraining();

	void createTraining();

	//void save();
	//void read();

	//void findType();

	POS findTypeDeployment(vector<POS> wtype, string phrase, int i);

	void filterExamples();

	//void generateData(int amount);
}; //class end 