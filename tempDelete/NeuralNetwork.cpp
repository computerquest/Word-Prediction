#pragma once
#include "NeuralNetwork.h"
using namespace std;

NeuralNetwork::NeuralNetwork() {
	learningRate = .099;
	momentum = .1;
	networkError = -1;
	fileExtension = 'a';
}

Neuron* NeuralNetwork::findNeuron(int id) {
	int sumSize = 0;
	int lastSize = 0;
	for (int i = 0; i < neuralNetwork.size(); i++) {
		sumSize += neuralNetwork.at(i).size();
		if (id < sumSize) {
			return &neuralNetwork.at(i).at(id-lastSize);
		}

		lastSize = sumSize;
	}
}

void NeuralNetwork::setInput(vector<double> input)
{
	for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
		neuralNetwork.at(0).at(i).setValue(input.at(i));
	}
}

void NeuralNetwork::reset() {
	for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
		(neuralNetwork.at(0).at(i)).value = 0;
	}
}

//the connections it is recieving
vector<Connection*> NeuralNetwork::getConnectionsTo(int id) {
	vector<Connection*> answer;

	int layer = 0;
	for (int i = 0; i < neuralNetwork.size(); i++) {
		if (id > neuralNetwork.at(i).at(neuralNetwork.at(i).size() - 1).id) {
			layer++;
		}
	}

	int posNum = id - neuralNetwork.at(layer).at(0).id;
	for (int i = 0; i < neuralNetwork.at(layer - 1).size(); i++) {
		answer.push_back((getConnectionsFrom(neuralNetwork.at(layer - 1).at(i).id).at(posNum)));
	}

	return answer;
}

double NeuralNetwork::calcNetInput(int id) {
	double answer = 0;
	vector<Connection*> connections = getConnectionsTo(id);

	for (int i = 0; i < connections.size(); i++) {
		answer += connections.at(i)->weight * findNeuron(connections.at(i)->sendId)->value;
	}

	return answer;
}
double NeuralNetwork::calcNetInput(vector<Connection> connect) {
	double answer = 0;

	for (int i = 0; i < connect.size(); i++) {
		answer += connect.at(i).weight * findNeuron(connect.at(i).sendId)->value;
	}

	return answer;
}

double NeuralNetwork::tanhDerivative(double x) {
	double answer = 1 - (pow(calcTanh(x), 2));
	if (answer != answer) {
		return 0;
	}
	return answer;
}
double NeuralNetwork::calcTanh(double netInput)
{
	double answer = (pow(2.718, netInput) - pow(2.718, (-1 * netInput))) / (pow(2.718, netInput) + pow(2.718, (-1 * netInput)));
	if (answer != answer) {
		return 0;
	}
	return answer;
}

double NeuralNetwork::sech(double x) {
	double answer = 2 / (pow(2.72, x) + pow(2.72, (-1 * x)));
	return answer;
}

double NeuralNetwork::calcSigmoid(double netInput) {
	double e = 1 / pow(2.71, netInput);
	return 1 / (1 + e);
}
double NeuralNetwork::caclSigmoidDerivative(double x) {
	return calcSigmoid(x)*(1 - calcSigmoid(x));
}

double NeuralNetwork::calcError(vector<double> desired) {
	double error = 0;
	for (int i = 0; i < desired.size(); i++) {
		error += .5 * pow(desired.at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value, 2);
	}

	return error;
}

vector<Connection*> NeuralNetwork::getConnectionsFrom(int id) {
	vector<Connection*> answer;

	for (int i = 0; i < masterConnection.at(id).size(); i++) {
		answer.push_back(&masterConnection.at(id).at(i));
	}

	return answer;
}

Connection* NeuralNetwork::findConnection(int from, int to)
{
	vector<Connection*> fromConnect = getConnectionsFrom(from);
	return fromConnect.at(to-fromConnect.at(0)->connectToID);
}

void NeuralNetwork::save() {
	ofstream stream;
	stream.open("weights.txt", ios::trunc);

	for (int a = 0; a < masterConnection.size(); a++) {
		for (int i = 0; i < masterConnection.at(a).size(); i++) {
			stream << masterConnection.at(a).at(i).weight << endl;
		}
	}
	stream.close();
}
void NeuralNetwork::read() {
	ifstream stream;
	stream.open("weights.txt");
	string temp;
	for (int i = 0; i < masterConnection.size(); i++) {
		for (int b = 0; b < masterConnection.at(i).size() && std::getline(stream, temp); b++) {
			masterConnection.at(i).at(b).weight = atof(temp.c_str());
		}
	}
	stream.close();
}