#pragma once
#include "NeuralNetwork.h"
using namespace std;

class MLP : public NeuralNetwork {
public:
	MLP();

	void process();
	void process(vector<double> input);

	void fix(vector<double> inputs, vector<double> desired);
	//void fix(vector<double> desired);

	void initialize(int input, vector<int> hidden, int output, bool batchI);
	double train(LinkedList<vector<double> , vector<double>> inputs);
	double train(int maxEpoch);//
	double train(int maxEpoch, string extension);

	//void autoPrune();
	//void autoPrune(int max, string extension);
	//void autoPrune(LinkedList<vector<double>, vector<double>> input, int layerNumber);
};