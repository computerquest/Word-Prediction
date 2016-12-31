#pragma once
#include <vector>
#include "Connection.h"
#include <iostream>
using namespace std;

class Neuron {
public:
	double value;
	int id;
	static int idValue;
	bool activationFunction;
	double influence;
	//
	Neuron();

	Neuron(int value);

	void setValue(double input);
};////
