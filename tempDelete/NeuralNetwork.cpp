#pragma once
#include "NeuralNetwork.h"
using namespace std;

Neuron* NeuralNetwork::findNeuron(double id) {
	for (int i = 0; i < neuralNetwork.size(); i++) {
		for (int a = 0; a < neuralNetwork.at(i).size(); a++) {
			if (id == neuralNetwork.at(i).at(a).id) {
				return &neuralNetwork.at(i).at(a);
			}
		}
	}
}

void NeuralNetwork::reset() {
	for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
		(neuralNetwork.at(0).at(i)).value = 0;
	}
}

void NeuralNetwork::process() {
	for (int i = 0; i < neuralNetwork.size(); i++) {
		neuralNetwork.at(0).at(i).setValue(currentInput.at(i));
	}

	for (int i = 0; i < neuralNetwork.size(); i++) {
		if (i == 0) {
			continue;
		}

		else {
			vector<Neuron*> neuron;

			for (int b = 0; b < neuralNetwork.at(i).size(); b++) {
				neuron.push_back(&neuralNetwork.at(i).at(b));
			}
			for (int a = 0; a < neuron.size(); a++) {
				Neuron *currentNeuron = neuron.at(a);
				vector<Connection*> connecting = getConnections((*currentNeuron));
				currentNeuron->setValue(calcTanh(calcNetInput(*currentNeuron)));
			}
		}
	}
}
void NeuralNetwork::process(vector<double> input) {
	currentInput = input;
	for (int i = 0; i < neuralNetwork.size(); i++) {
		neuralNetwork.at(0).at(i).setValue(input.at(i));
	}

	for (int i = 0; i < neuralNetwork.size(); i++) {
		if (i == 0) {
			continue;
		}

		else {
			vector<Neuron*> neuron;

			for (int b = 0; b < neuralNetwork.at(i).size(); b++) {
				neuron.push_back(&neuralNetwork.at(i).at(b));
			}
			for (int a = 0; a < neuron.size(); a++) {
				Neuron *currentNeuron = neuron.at(a);
				vector<Connection*> connecting = getConnections((*currentNeuron));
				//currentNeuron->setValue(calcSigmoid(calcNetInput(*currentNeuron)));
				currentNeuron->setValue(calcTanh(calcNetInput(*currentNeuron)));
			}
		}
	}
}

vector<Connection*> NeuralNetwork::getConnections(Neuron neuron) {
	vector<Connection*> answer;

	for (int i = 0; i < connections.size(); i++) {
		if (neuron.id == connections.at(i)->connectToID) {
			answer.push_back(connections.at(i));
		}
	}

	return answer;
}

void NeuralNetwork::fix(vector<double> inputs, vector<double> desired) {
	currentInput = inputs;
	trainingValues.add(inputs, desired);
	//loop for each output node
	vector<vector<Neuron*>> nodes;

	for (int i = 0; i < this->neuralNetwork.size(); i++) {
		vector<Neuron*> tempNeurons;

		//this might be redundant it makes pointers from neuralNetwork and puts them in tempNeurons
		for (int a = 0; a < this->neuralNetwork.at(i).size(); a++) {
			tempNeurons.push_back(&neuralNetwork.at(i).at(a));
		}
		nodes.push_back(tempNeurons);
	}

	//this sets the beginning values
	for (int i = 0; i < (nodes.at(0)).size(); i++) {
		nodes.at(0).at(i)->value = inputs.at(i);
	}

	process();

	for (int i = 0; i < (nodes.at(nodes.size() - 1)).size(); i++) {
		Neuron *outputNeuron = (nodes.at(nodes.size() - 1)).at(i);
		double target = desired.at(i);
		double output = outputNeuron->value;
		double difference = -1 * (target - output);
		//double derivitive = output * (1 - output);
		double derivitive = tanhDerivative(output);

		//connecting directly to output
		{
			vector<Connection*> connection = getConnections((*outputNeuron));
			for (int a = 1; a < connection.size(); a++) {
				Connection *currentConnection = connection.at(a);
				double weight = currentConnection->weight;
				double input = (findNeuron(currentConnection->sendId))->value;
				double change = derivitive * input * difference;
				currentConnection->setWeight(weight - (learningRate*change) + (momentum * weight));
			}
		}
	}

	//hidden
	/*
	3
	3-2 = 1
	4 - 2 = 2
	2> 1
	num = 3;
	4-3 = 1
	all ways has to be -1 to make it 0 based
	*/

	/*
	for(int d= 2; neuralNetwork.size()-d > 0; d++) {
	vector<Neuron*> currentStuff = neuralNetwork.getValue(neuralNetwork.size - d);

	for (int b = 0; b < currentStuff.size(); b++) {
	*/

	for (int z = 2; nodes.size() - z > 0; z++) {
		vector<Neuron*> currentStuff = nodes.at(nodes.size() - z);

		for (int b = 0; b < currentStuff.size(); b++) { //hidden node
			Neuron* currentHidden = currentStuff.at(b);
			vector<Connection*> connection = getConnections((*currentHidden));
			double output = currentHidden->value;
			//double derivitive = output * (1 - output);
			double derivitive = tanhDerivative(output);

			for (int c = 0; c < connection.size(); c++) { //input to node
				Connection* currentConnection = connection.at(c);
				double weight = currentConnection->weight;
				double solution = 0;
				double input = (findNeuron(currentConnection->sendId))->value;

				{
					for (int i = 0; i < (nodes.at(nodes.size() - 1)).size(); i++) {
						Neuron* neuron = (nodes.at(nodes.size() - 1)).at(i);
						double tempTarget = desired.at(i);
						double tempOutput = neuron->value;
						//solution += (-1 * (tempTarget - tempOutput)) * (tempOutput * (1 - tempOutput)) * weight;
						solution += (-1 * (tempTarget - tempOutput)) * (tanhDerivative(tempOutput)) * weight;
					}
				}

				double change = derivitive * solution * input;

				currentConnection->setWeight(weight - learningRate * change + (momentum * weight));
			}
		}
	}
	process();
	networkError = calcError(desired);
}
void NeuralNetwork::fix(vector<double> desired) {
	trainingValues.add(currentInput, desired);

	//loop for each output node
	vector<vector<Neuron*>> nodes;

	for (int i = 0; i < this->neuralNetwork.size(); i++) {
		vector<Neuron*> tempNeurons;

		//this might be redundant it makes pointers from neuralNetwork and puts them in tempNeurons
		for (int a = 0; a < this->neuralNetwork.at(i).size(); a++) {
			tempNeurons.push_back(&neuralNetwork.at(i).at(a));
		}
		nodes.push_back(tempNeurons);
	}

	//this sets the beginning values
	for (int i = 0; i < (nodes.at(0)).size(); i++) {
		nodes.at(0).at(i)->value = currentInput.at(i);
	}

	process();

	for (int i = 0; i < (nodes.at(nodes.size() - 1)).size(); i++) {
		Neuron *outputNeuron = (nodes.at(nodes.size() - 1)).at(i);
		double target = desired.at(i);
		double output = outputNeuron->value;
		double difference = -1 * (target - output);
		//double derivitive = output * (1 - output);
		double derivitive = tanhDerivative(output);

		//connecting directly to output
		{
			vector<Connection*> connection = getConnections((*outputNeuron));
			for (int a = 1; a < connection.size(); a++) {
				Connection *currentConnection = connection.at(a);
				double weight = currentConnection->weight;
				double input = (findNeuron(currentConnection->sendId))->value;
				double change = derivitive * input * difference;
				currentConnection->setWeight(weight - (learningRate*change) + (momentum * weight));
			}
		}
	}

	//hidden
	/*
	3
	3-2 = 1
	4 - 2 = 2
	2> 1
	num = 3;
	4-3 = 1
	all ways has to be -1 to make it 0 based
	*/

	/*
	for(int d= 2; neuralNetwork.size()-d > 0; d++) {
	vector<Neuron*> currentStuff = neuralNetwork.getValue(neuralNetwork.size - d);

	for (int b = 0; b < currentStuff.size(); b++) {
	*/

	for (int z = 2; nodes.size() - z > 0; z++) {
		vector<Neuron*> currentStuff = nodes.at(nodes.size() - z);

		for (int b = 0; b < currentStuff.size(); b++) { //hidden node
			Neuron* currentHidden = currentStuff.at(b);
			vector<Connection*> connection = getConnections((*currentHidden));
			double output = currentHidden->value;
			//double derivitive = output * (1 - output);
			double derivitive = tanhDerivative(output);

			for (int c = 0; c < connection.size(); c++) { //input to node
				Connection* currentConnection = connection.at(c);
				double weight = currentConnection->weight;
				double solution = 0;
				double input = (findNeuron(currentConnection->sendId))->value;

				{
					for (int i = 0; i < (nodes.at(nodes.size() - 1)).size(); i++) {
						Neuron* neuron = (nodes.at(nodes.size() - 1)).at(i);
						double tempTarget = desired.at(i);
						double tempOutput = neuron->value;
						//solution += (-1 * (tempTarget - tempOutput)) * (tempOutput * (1 - tempOutput)) * weight;
						solution += (-1 * (tempTarget - tempOutput)) * (tanhDerivative(tempOutput)) * weight;
					}
				}

				double change = derivitive * solution * input;

				currentConnection->setWeight(weight - learningRate * change + (momentum * weight));
			}
		}
	}
	process();

	networkError = calcError(desired);
}

double NeuralNetwork::calcNetInput(Neuron input) {
	double answer = 0;
	vector<Connection*> connections = getConnections(input);

	for (int i = 0; i < connections.size(); i++) {
		answer += connections.at(i)->weight * findNeuron(connections.at(i)->sendId)->value;
	}

	return answer;
}

void  NeuralNetwork::initializeNN(int input, vector<int> hidden, int output) {
	neuralNetwork.clear();
	connections.clear();

	vector <vector<Neuron>> neurons;
	vector<Neuron> inputN;
	vector<Neuron> outputN;
	for (int i = 0; i < input; i++) {
		Neuron tempNeuron(0);
		inputN.push_back(tempNeuron);
	}
	neurons.push_back(inputN);

	for (int i = 0; i < hidden.size(); i++) {
		vector<Neuron> hiddenNeurons;
		for (int a = 0; a < hidden.at(i); a++) {
			Neuron tempNeuron(0);
			hiddenNeurons.push_back(tempNeuron);
		}

		neurons.push_back(hiddenNeurons);
	}

	for (int i = 0; i < output; i++) {
		Neuron tempNeuron(0);
		outputN.push_back(tempNeuron);
	}

	neurons.push_back(outputN);

	neuralNetwork = neurons;

	std::srand(std::time(0));

	for (int i = 0; i < neuralNetwork.size(); i++) {
		if (i + 1 < neuralNetwork.size()) {
			for (int b = 0; b < neuralNetwork.at(i).size(); b++) {
				for (int c = 0; c < neuralNetwork.at(i + 1).size(); c++) {
					neuralNetwork.at(i).at(b).addConnection(((double)rand() / (RAND_MAX)), (neuralNetwork.at(i + 1)).at(c));
				}
			}
		}
		else {
			break;
		}
	}
	addAllConnections();
}

void NeuralNetwork::addAllConnections() {
	for (int i = 0; i < neuralNetwork.size(); i++) {
		for (int c = 0; c < neuralNetwork.at(i).size(); c++) {
			for (int b = 0; b < neuralNetwork.at(i).at(c).connection.size(); b++) {
				connections.push_back(&neuralNetwork.at(i).at(c).connection.at(b));
			}
		}
	}
}

double NeuralNetwork::tanhDerivative(double x) {
	double answer = 1-(pow(calcTanh(x), 2));
	return answer;
}

double NeuralNetwork::sech(double x) {
	double answer = 2 / (pow(2.72, x) + pow(2.72, (-1 * x)));
	return answer;
}

double NeuralNetwork::train(LinkedList<vector<double>, vector<double>> inputs) {
	double lastError = 10000;
	int strikeA = 3;
	double greatest = 1000000000;
	double hundredthError = 100000000;

	for (int s = 1; true; s++) {
		double error = 0;
		cout << "gen: " << s << endl;
		for (int a = 0; a < inputs.size(); a++) {
			double lineError = 0;
			fix(inputs.getKeyI(a), inputs.getValueI(a));

			for (int i = 0; i < inputs.getValueI(a).size(); i++) {
				lineError += .5 * pow(inputs.getValueI(a).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value, 2);
			}
			error += lineError;

			cout << inputs.getValueI(a).at(0);
			for (int i = 1; i < inputs.getValueI(a).size(); i++) {
				cout << " " << inputs.getValueI(a).at(i);
			}
			cout << ": " << neuralNetwork.at(neuralNetwork.size() - 1).at(0).value << " ";
			for (int i = 1; i < neuralNetwork.at(neuralNetwork.size() - 1).size(); i++) {
				cout << neuralNetwork.at(neuralNetwork.size() - 1).at(i).value << " ";
			}
			cout << "lineError: " << lineError << endl;
		}
		networkError = error;
		{
			if (strikeA > 1 && error - lastError >= -.00001) {
				strikeA--;
			}
			else if (strikeA <= 1) {
				break;
			}
			else if (error - lastError <= -.0001) {
				strikeA = 3;
			}
		}

		if (hundredthError - error > -.0001 && s % 100 == 0) {
			break;
		}

		if (error < greatest) {
			greatest = error;
		}

		cout << "error: " << error << " change: " << error-lastError << " smallest: " << greatest << endl;

		lastError = error;
		cout << endl;
	}
	networkError = greatest;

	return greatest;
}
double NeuralNetwork::train(int maxEpoch) {
	double lastError = 10000;
	LinkedList<vector<double>, vector<double>> inputs = trainingValues;
	double greatest = 10000;
	for (int s = 1; s < maxEpoch+1; s++) {
		double error = 0;
		cout << "gen: " << s << endl;
		for (int a = 0; a < inputs.size(); a++) {
			double lineError = 0;
			fix(inputs.getKeyI(a), inputs.getValueI(a));

			for (int i = 0; i < inputs.getValueI(a).size(); i++) {
				lineError += .5 * pow(inputs.getValueI(a).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value, 2);
			}
			error += lineError;
		}
		networkError = error;

		if (s > 1) {
			adjustLearningRate(error, lastError);
		}

		if (error < greatest) {
			greatest = error;
		}

		cout << "error: " << error << " change: " << error - lastError << " smallest: " << greatest << endl;

		lastError = error;
		cout << endl;
	}

	networkError = greatest;
	return greatest;
}

void NeuralNetwork::adjustLearningRate(double firstError, double lastError) {
	double change = firstError - lastError;

	if (change < 0) {
		learningRate *= .05;
	}
	else {
		learningRate / 2;
	}
}

void NeuralNetwork::autoPrune() {
	vector<int> hiddenLayers;

	for (int i = 1; i < neuralNetwork.size() - 1; i++) {
		hiddenLayers.push_back(neuralNetwork.at(i).size());
	}

	double smallestError = train(1000);

	for (int i = 1; i < neuralNetwork.size() - 1; i++) {
		cout << i << endl;
		while (true) {
			vector<int> newHiddenLayers;
			for (int a = 0; a < hiddenLayers.size(); a++) {
				if (a == i - 1) {
					newHiddenLayers.push_back(hiddenLayers.at(a) + 1);
					continue;
				}
				newHiddenLayers.push_back(hiddenLayers.at(a));
			}

			initializeNN(neuralNetwork.at(0).size(), newHiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size());
			double currentError = train(1000);

			if (currentError < smallestError) {
				cout << "dimesions: ";
				for (int i = 0; i < hiddenLayers.size(); i++) {
					cout << hiddenLayers.at(i) << " ";
				}
				cout << "smallest: " << smallestError << endl;

				cout << "continue" << endl;
				smallestError = currentError;
				hiddenLayers = newHiddenLayers;
			}
			else {
				cout << "end: " << i << endl;
				break;
			}
		}
	}

	cout << "dimesions: ";
	for (int i = 0; i < hiddenLayers.size(); i++) {
		cout << hiddenLayers.at(i) << " ";
	}
	cout << "smallest: " << smallestError << endl;

	initializeNN(neuralNetwork.at(0).size(), hiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size());
}
void NeuralNetwork::autoPrune(LinkedList<vector<double>, vector<double>> input, int layerNumber) {
	vector<int> hiddenLayers;

	for (int i = 1; i < neuralNetwork.size() - 1; i++) {
		hiddenLayers.push_back(neuralNetwork.at(i).size());
	}

	double smallestError = train(input);

	while (true) {
		vector<int> newHiddenLayers;
		for (int a = 0; a < hiddenLayers.size(); a++) {
			if (a == layerNumber - 1) {
				newHiddenLayers.push_back(hiddenLayers.at(a) + 1);
				continue;
			}
			newHiddenLayers.push_back(hiddenLayers.at(a));
		}

		initializeNN(neuralNetwork.at(0).size() - 1, newHiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size());
		double currentError = train(input);

		if (currentError < smallestError) {
			cout << "continue" << endl;
			smallestError = currentError;
			hiddenLayers = newHiddenLayers;
		}
		else {
			break;
		}
	}

	cout << "dimesions: ";
	for (int i = 0; i < hiddenLayers.size(); i++) {
		cout << hiddenLayers.at(i) << " ";
	}
	cout << "smallest: " << smallestError << endl;

	initializeNN(neuralNetwork.at(0).size() - 1, hiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size());
}

double NeuralNetwork::calcSigmoid(double netInput) {
	double e = 1 / pow(2.71, netInput);
	return 1 / (1 + e);
}

double NeuralNetwork::calcTanh(double netInput)
{
	double answer= (pow(2.71, netInput) - pow(2.71, (-1* netInput))) / (pow(2.71, netInput) + pow(2.71, (-1* netInput)));
	return answer;
}

double NeuralNetwork::calcError(vector<double> desired) {
	double error = 0;
	for (int i = 0; i < desired.size(); i++) {
		error += .5 * pow(desired.at(i) - neuralNetwork.at(neuralNetwork.size()-1).at(i).value, 2);
	}

	return error;
}

vector<Connection*> NeuralNetwork::getConnectionsFrom(Neuron neuron) {
	double id = neuron.id;
	vector<Connection*> solution;
	for (int i = 0; i < connections.size(); i++) {
		if (connections.at(i)->sendId == id) {
			solution.push_back(connections.at(i));
		}
	}

	return solution;
}

void NeuralNetwork::optimize() {
	
	train(1000);

	vector<int> hiddenSize;
	for (int i = neuralNetwork.size()-2; i > 0; i--) {
		hiddenSize.push_back(neuralNetwork.at(i).size());
	}

	double lastError = networkError;
	for (int i = 0; i < hiddenSize.size(); i++) {
		int direction = -1;
		for (int gen = 0; true; gen++) {
			train(1000);

			if (lastError >= networkError) {
				hiddenSize.at(i) += direction;

				if (hiddenSize.at(i) == 0) {
					hiddenSize.erase(hiddenSize.begin() + i);
				}
			}
			else {
				if (gen == 0) {
					direction *= -1;
					lastError = networkError;
					continue;
				}

				if (gen == 1 && direction == 1) {
					hiddenSize.at(i) -= 1;

					if (hiddenSize.at(i) == 0) {
						hiddenSize.erase(hiddenSize.begin() + i);
					}
				}

				lastError = networkError;
				break;
			}

			lastError = networkError;
			initializeNN(neuralNetwork.at(0).size(), hiddenSize, 3);
		}
	}

	cout << "Dimensions (L->S): ";
	for (int i = 0; i < hiddenSize.size(); i++) {
		cout << hiddenSize.at(i);
	}
}