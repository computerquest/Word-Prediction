#pragma once
#include "Neuron.h"
using namespace std;

Neuron::Neuron() {
	id = idValue;
	idValue++;
}

Neuron::Neuron(int value) : value(value) {
	id = idValue;
	idValue++;
}

void Neuron::setValue(double input) {
	value = input;
}
int Neuron::idValue = 0;