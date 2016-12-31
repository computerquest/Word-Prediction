#include "RBM.h"

/*RBM::RBM() {
	layerFirst = true;
	learningRate = .01;
	momentum = .9;
}

void RBM::initializeNetwork(int input, int output) {
	neuralNetwork.clear();
	connections.clear();

	vector <vector<Neuron>> neurons;
	vector<Neuron> inputN;
	vector<Neuron> outputN;
	for (int i = 0; i < input; i++) {
		Neuron tempNeuron(0);
		tempNeuron.activationFunction = false;
		inputN.push_back(tempNeuron);
	}

	neurons.push_back(inputN);

	for (int i = 0; i < output; i++) {
		Neuron tempNeuron(0);
		tempNeuron.activationFunction = true;
		outputN.push_back(tempNeuron);
	}

	neurons.push_back(outputN);

	neuralNetwork = neurons;//

	std::srand(std::time(0));

	for (int i = 0; i < neuralNetwork.size(); i++) {
		if (i + 1 < neuralNetwork.size()) {
			for (int b = 0; b < neuralNetwork.at(i).size(); b++) {
				for (int c = 0; c < neuralNetwork.at(i + 1).size(); c++) {
					neuralNetwork.at(i).at(b).addConnection(((double)rand() / (RAND_MAX)) - ((double)rand() / (RAND_MAX)), (neuralNetwork.at(i + 1)).at(c));
				}
			}
		}
		else {
			break;
		}
	}

	addAllConnections();
}

//working on
void RBM::backPropagation(vector<double> input) {
	//loop for each output node

	//this sets the beginning values
	for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
		neuralNetwork.at(0).at(i).value = input.at(i);
	}

	layerFirst = true;
	process();

	vector<double> desired;
	for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
		desired.push_back(neuralNetwork.at(0).at(i).value);
	}

	layerFirst = false;
	process();

	while (true) {
		//the output and the connections connecting to the output
		for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
			Neuron *outputNeuron = &(neuralNetwork.at(0)).at(i);
			double target = desired.at(i);
			double output = outputNeuron->value;
			double difference = -1 * (target - output);
			//double derivitive = tanhDerivative(output);

			//connecting directly to output
			{
				vector<Connection*> connection = getConnectionsFrom((*outputNeuron));
				for (int a = 0; a < connection.size(); a++) {
					Connection *currentConnection = connection.at(a);
					double weight = currentConnection->weight;
					double input = (findNeuron(currentConnection->connectToID))->value;
					double change = input*difference;
					double partOne = (learningRate*change);
					currentConnection->setWeight(weight - partOne);
				}
			}
		}

		layerFirst = false;
		process();

		double errorAvg = 0;
		for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
			errorAvg += abs(desired.at(i) - neuralNetwork.at(0).at(i).value);
			cout << neuralNetwork.at(0).at(i).value << " ";
		}

		errorAvg /= neuralNetwork.at(0).size();

		cout << errorAvg << endl;

		if (errorAvg < .01) {
			break;
		}
	}*/
	/*
	set data
(pos hidden activitions) get net input
(pos hidden prob) logistic function
(pos hidden states) pos hidden rob > random number
(pos associations) multiply data with the pos hidden probs
(neg visible activations) multiple pos hidden states with weights
(neg visible prob) lostistic neg visible activations
(neg hidden activations) neg visible probs multiple weights
(neg hidden probs) logistic neg hidden activations
(neg associations) neg visible probs multiple neg hidden probs

	for (int i = 0; i < neuralNetwork.at(0).size(); i++) {
		neuralNetwork.at(0).at(i) = input.at(i);
	}

	vector<double> posProbV;
	for (int i = 0; i < neuralNetwork.at(1).size(); i++) {
		double posProb = calcSigmoid(calcNetInput(getConnections(neuralNetwork.at(1).at(i))));
		neuralNetwork.at(1).at(i).value = posProb > .5 ? 1 : 0;
		posProbV.push_back(posProb);
	}

	double posAssociation = 0;
	for (int i = 0; i < posProbV.size(); i++) {
		for (int a = 0; a < input.size(); a++) {
			posAssociation += posProbV.at(i)*input.at(a);
		}
	}

	vector<double> negVisProbV;
	for (int i = 0; i < neuralNetwork.at(1).size(); i++) {
		double negVisProb = calcSigmoid(neuralNetwork.at(1).at(i).value);
		negVisProbV.push_back(negVisProb);
	}


}

void RBM::process() {
	for (int i = 0; i < currentInput.size(); i++) {
		neuralNetwork.at(0).at(i).value = currentInput.at(i);
	}

	for (int i = 0; i < neuralNetwork.at(1).size(); i++) {
		double newValue = 0;
		double netInput = calcNetInput(getConnections(neuralNetwork.at(1).at(i)));
		newValue = calcSigmoid(netInput);

		neuralNetwork.at(1).at(i).value = newValue < .5 ? 0 : 1;
	}
}*/