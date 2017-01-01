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

	void setInput(vector<double> input);

	void reset();

	vector<Connection*> getConnectionsTo(int id);
	vector<Connection*> getConnectionsFrom(int id);
	Connection* findConnection(int from, int to);

	double calcNetInput(int id);
	double calcNetInput(vector<Connection> connection);

	double calcError(vector<double> desired);

	double calcSigmoid(double netInput);
	double caclSigmoidDerivative(double x);
	double calcTanh(double netInput);
	double tanhDerivative(double x);
	double sech(double x);
};