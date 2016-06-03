#pragma once
#include <vector>
#include "Connection.h"
#include <iostream>
using namespace std;

class Neuron {
public:
	double value;
	double id;
	static double idValue;
	vector<Connection> connection;

	Neuron();

	Neuron(double value);

	void setValue(double input);

	void addConnection(double weight, Neuron neuron);

	void addConnection(vector<double> weights, vector<Neuron> layer);
};
