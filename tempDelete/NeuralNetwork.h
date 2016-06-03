#pragma once
//#include "Connection.h"
#include "Neuron.h"
#include <vector>
#include "LinkedList.h"
#include <iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

class NeuralNetwork {
public:
	vector<vector<Neuron>> neuralNetwork;
	vector <Connection*> connections;

	Neuron * findNeuron(double id);

	void reset();

	void process();
	void process(vector<double> input);

	vector<Connection*> getConnections(Neuron neuron);

	void fix(vector<double> inputs, vector<double> desired);

	double calcNetInput(Neuron input);

	void  initializeNN(int input, vector<int> hidden, int output);

	void addAllConnections();
	LinkedList<vector<double>, vector<double>> limit(LinkedList<vector<double>, vector<double>> input, int amount);
	double calcDifference(vector<double> one, vector<double> two);
	LinkedList<vector<double>, vector<double>> staggerDesired(LinkedList<vector<double>, vector<double>> input);
	LinkedList<vector<double>, vector<double>> tweakDesired(LinkedList<vector<double>, vector<double>> input);
	double train(LinkedList<vector<double>, vector<double>> inputs);

	void autoPrune(LinkedList<vector<double>, vector<double>> input, bool);
	void autoPrune(LinkedList<vector<double>, vector<double>> input, int layerNumber, bool);
	double calcSigmoid(double netInput);
	double calcTanh(double netInput);
	double tanhDerivative(double x);
	double sech(double x);
};