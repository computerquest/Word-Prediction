#pragma once
#include <iostream>
#include <vector>
#include "LinkedList.h"
#include "POS.h"
#include "vectorContains.h"
#include "NeuralNetwork.h"
#include "NGram.h"
#include "SStructure.h"
#include "BreakDown.h"
#include "Word.h"
#include <fstream>
using namespace std;

class MasterResource;
class PredictionEngine {
public:
	NeuralNetwork nn;
	LinkedList<vector<double>, vector<double>> training;
	vector<string> examples;
	NeuralNetwork predictNN;
	MasterResource* master;
	vector<POS> lastPhraseGlobal;

	PredictionEngine(int inputN, vector<int> hidden, int output, MasterResource* resource);

	vector<POS> multipleMissing(string phrase);

	void saveTraining();
	void readTraining();

	LinkedList<vector<double>, vector<double>> createTrain(vector<Neuron> neurons, vector<vector<double>> desired);

	void save();
	void read();

	void findType();

	POS findTypeDeployment(vector<POS> wtype, string phrase, int i);

	void filterExamples();

	void generateData(int amount);
}; //class end 