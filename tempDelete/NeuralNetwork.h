#pragma once
//#include "Connection.h"
#include "Neuron.h"
#include <vector>
#include "LinkedList.h"
#include <iostream>
#include<cstdlib>
#include <fstream>
#include<ctime>
#include <string>
using namespace std;

class NeuralNetwork {
public:
	vector<vector<Neuron>> neuralNetwork;
	//vector <Connection> masterConnection;
	vector<vector<Connection>> masterConnection; // might have problem of pointers not working when delete or change stuff because would change key and value
	vector<double> currentInput;
	double networkError;
	char fileExtension;
	bool batch;
	double momentum;
	double learningRate;
	LinkedList<vector<double>, vector<double>> trainingValues;

	NeuralNetwork();

	Neuron * findNeuron(int id);

	void save();
	void read();
	//void optimize(); 
	//
	void setInput(vector<double> input);

	void reset();

	//void adjustLearningRate(double* firstError, double lastError);

	//virtual void process();
	//virtual void process(vector<double> input);

	vector<Connection*> getConnectionsTo(int id);
	vector<Connection*> getConnectionsFrom(int id);
	Connection* findConnection(int from, int to);

	//virtual void fix(vector<double> inputs, vector<double> desired);

	//virtual void fix(vector<double> desired);

	double calcNetInput(int id);
	double calcNetInput(vector<Connection> connection);
	//void  initializeNN(int input, vector<int> hidden, int output, bool momentum);

	double calcError(vector<double> desired);
	//virtual double train(LinkedList<vector<double>, vector<double>> inputs);
	////virtual double train(int maxEpoch);
	//virtual double train(int maxEpoch, string extension);

	//void autoPrune();
	//void autoPrune(int max, string extension);
	//void autoPrune(LinkedList<vector<double>, vector<double>> input, int layerNumber);
	double calcSigmoid(double netInput);
	double caclSigmoidDerivative(double x);
	double calcTanh(double netInput);
	double tanhDerivative(double x);
	double sech(double x);
};