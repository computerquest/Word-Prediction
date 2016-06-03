#pragma once
#include "Neuron.h"
using namespace std;

Neuron::Neuron() {
	id = idValue;
	idValue += .0001;
}

Neuron::Neuron(double value) : value(value) {
	id = idValue;
	idValue += .0001;
}

void Neuron::setValue(double input) {
	value = input;
}

void Neuron::addConnection(double weight, Neuron neuron) {
	Connection newConnection(weight, neuron.id, id);
	connection.push_back(newConnection);
}

void Neuron::addConnection(vector<double> weights, vector<Neuron> layer) {
	for (int i = 0; i < layer.size(); i++) {
		addConnection(weights.at(i), layer.at(i));
	}
}
double Neuron::idValue = 0;

