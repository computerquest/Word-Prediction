#pragma once
#include "NeuralNetwork.h"
using namespace std;

class MLP : public NeuralNetwork {
public:
	MLP();

	void process();
	void process(vector<double> input);

	void fix(vector<double> inputs, vector<double> desired);

	void initialize(int input, vector<int> hidden, int output, bool batchI);
	double train(LinkedList<vector<double>, vector<double>> inputs, int end);
	double train(int maxEpoch, string extension);
};