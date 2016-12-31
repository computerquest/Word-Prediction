#include "MLP.h"

MLP::MLP() {}
void MLP::process() {
	setInput(currentInput);

	for (int i = 1; i < neuralNetwork.size(); i++) {
		for (int a = 0; a < neuralNetwork.at(i).size(); a++) {
			//Neuron *currentNeuron = &neuralNetwork.at(i).at(a);
			//vector<Connection*> connecting = getConnections((*currentNeuron));
			neuralNetwork.at(i).at(a).setValue(calcTanh(calcNetInput(neuralNetwork.at(i).at(a).id)));
		}
	}
}
void MLP::process(vector<double> input) {
	currentInput = input;
	setInput(currentInput);

	for (int i = 1; i < neuralNetwork.size(); i++) {
		for (int a = 0; a < neuralNetwork.at(i).size(); a++) {
			//Neuron *currentNeuron = &neuralNetwork.at(i).at(a);
			//vector<Connection*> connecting = getConnections((*currentNeuron));
			//currentNeuron->setValue(calcSigmoid(calcNetInput(*currentNeuron)));
			//double newValue = calcTanh(calcNetInput(*currentNeuron));
			neuralNetwork.at(i).at(a).setValue(calcTanh(calcNetInput(neuralNetwork.at(i).at(a).id)));
		}
	}
}

//THIS IS WHERE I LEFT OFF I CHANGED MASTERCONNECT TO A LINKED LIST AND NOW I AM CHANGING ALL THIS TO LINKED LIST THIS SHOULD BE THE LAST PIECE BUT WENT TO BED
void MLP::fix(vector<double> inputs, vector<double> desired) {
	currentInput = inputs;
	//loop for each output node

	//this sets the beginning values
	setInput(currentInput);

	process();

	/*vector<double> outputError;
	double totalError = 0;
	{
		double target;
		for (int i = 0; i < (neuralNetwork.at(neuralNetwork.size() - 1)).size(); i++) {
			target = desired.at(i);
			double output = (&(neuralNetwork.at(neuralNetwork.size() - 1)).at(i))->value;
			outputError.push_back(.5*pow((output - target), 2));
			totalError += .5*pow((output - target), 2);
		}
	}*/

	//the output and the connections connecting to the output
	{
		//double target;
		for (int i = 0; i < (neuralNetwork.at(neuralNetwork.size() - 1)).size(); i++) {
			//target = desired.at(i);
			Neuron *outputNeuron = &(neuralNetwork.at(neuralNetwork.size() - 1)).at(i);
			//double output = outputNeuron->value;
			//double totalToOut = output - target;
			double outToNet = tanhDerivative(outputNeuron->value);

			outputNeuron->influence = outputNeuron->value - desired.at(i);

			//connecting directly to output
			{
				vector<Connection*> connection = getConnectionsTo((outputNeuron->id));
				for (int a = 1; a < connection.size(); a++) {
					Connection *currentConnection = connection.at(a);
					//double netToWeight = (findNeuron(currentConnection->sendId))->value;
					//double totalToWeight = outToNet * netToWeight * totalToOut;
					//double partOne = (learningRate*totalToWeight);
					currentConnection->nextWeight += outToNet * (findNeuron(currentConnection->sendId))->value * outputNeuron->influence*learningRate;
				}
			}
		}
	}

	//hidden
	for (int z = 2; neuralNetwork.size() - z > 0; z++) {
		//vector<Neuron>* currentStuff = &neuralNetwork.at(neuralNetwork.size() - z); //current layer

		for (int b = 0; b < neuralNetwork.at(neuralNetwork.size() - z).size(); b++) { //hidden node
			Neuron* currentHidden = &neuralNetwork.at(neuralNetwork.size() - z).at(b);
			vector<Connection*> connection = getConnectionsTo((currentHidden->id));
			//double output = currentHidden->value;
			//double derivitive = output * (1 - output);
			double derivitive = tanhDerivative(currentHidden->value);

			double sumInfluence = 0;
			for (int i = 0; i < neuralNetwork.at(neuralNetwork.size() - z + 1).size(); i++) {
				sumInfluence += (neuralNetwork.at(neuralNetwork.size() - z + 1).at(i)).influence*findConnection(currentHidden->id, neuralNetwork.at(neuralNetwork.size() - z + 1).at(i).id)->weight;
			}

			currentHidden->influence = sumInfluence;

			for (int i = 0; i < connection.size(); i++) {
				//Neuron neuronValue = *findNeuron(connection.at(i)->sendId);
				//double partOne = learningRate*(derivitive* (currentHidden->influence) * neuronValue.value);
				//connection.at(i)->nextWeight += partOne;
				connection.at(i)->nextWeight += learningRate*(derivitive* (currentHidden->influence) * findNeuron(connection.at(i)->sendId)->value);
			}
		}
	}
	//process(); //dont need
}
/*void MLP::fix(vector<double> desired) {
	//this sets the beginning values
	for (int i = 0; i < (neuralNetwork.at(0)).size(); i++) {
		neuralNetwork.at(0).at(i).value = currentInput.at(i);
	}

	process();

	for (int i = 0; i < (neuralNetwork.at(neuralNetwork.size() - 1)).size(); i++) {
		Neuron *outputNeuron = &(neuralNetwork.at(neuralNetwork.size() - 1)).at(i);
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
				double partOne = (learningRate*change);
				if (!batch) {
					currentConnection->setWeight(weight - partOne);
				}
				else {
					currentConnection->nextWeight += partOne;
				}
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

	/*
	for(int d= 2; neuralNetwork.size()-d > 0; d++) {
	vector<Neuron*> currentStuff = neuralNetwork.getValue(neuralNetwork.size - d);

	for (int b = 0; b < currentStuff.size(); b++) {


	for (int z = 2; neuralNetwork.size() - z > 0; z++) {
		vector<Neuron>* currentStuff = &neuralNetwork.at(neuralNetwork.size() - z);

		for (int b = 0; b < currentStuff->size(); b++) { //hidden node
			Neuron* currentHidden = &currentStuff->at(b);
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
					for (int i = 0; i < (neuralNetwork.at(neuralNetwork.size() - 1)).size(); i++) {
						Neuron* neuron = &(neuralNetwork.at(neuralNetwork.size() - 1)).at(i);
						double tempTarget = desired.at(i);
						double tempOutput = neuron->value;
						//solution += (-1 * (tempTarget - tempOutput)) * (tempOutput * (1 - tempOutput)) * weight;
						solution += (-1 * (tempTarget - tempOutput)) * (tanhDerivative(tempOutput)) * weight;
					}
				}

				double change = derivitive * solution * input;

				double partOne = (learningRate*change);
				if (!batch) {
					currentConnection->setWeight(weight - partOne);
				}
				else {
					currentConnection->nextWeight += partOne;
				}
			}
		}
	}
	process();
} //not updated*/

void MLP::initialize(int input, vector<int> hidden, int output, bool batchI)
{
	batch = batchI;
	neuralNetwork.clear();

	vector <vector<Neuron>> neurons;
	vector<Neuron> inputN;
	vector<Neuron> outputN;
	for (int i = 0; i < input; i++) {
		Neuron tempNeuron(0);
		tempNeuron.activationFunction = false;
		inputN.push_back(tempNeuron);
	}
	neurons.push_back(inputN);

	for (int i = 0; i < hidden.size(); i++) {
		vector<Neuron> hiddenNeurons;
		for (int a = 0; a < hidden.at(i); a++) {
			Neuron tempNeuron(0);
			tempNeuron.activationFunction = true;
			hiddenNeurons.push_back(tempNeuron);
		}

		neurons.push_back(hiddenNeurons);
	}

	for (int i = 0; i < output; i++) {
		Neuron tempNeuron(0);
		tempNeuron.activationFunction = true;
		outputN.push_back(tempNeuron);
	}

	neurons.push_back(outputN);

	neuralNetwork = neurons;

	vector<vector<Connection>> newMasterConnect;

	std::srand(std::time(0));
	for (int i = 0; i < neuralNetwork.size(); i++) {
		if (i + 1 < neuralNetwork.size()) {
			for (int b = 0; b < neuralNetwork.at(i).size(); b++) {
				vector<Connection> connectV;
				for (int c = 0; c < neuralNetwork.at(i + 1).size(); c++) {
					Connection connect(((double)rand() / (RAND_MAX)), (neuralNetwork.at(i + 1)).at(c).id, neuralNetwork.at(i).at(b).id);
					connectV.push_back(connect);
				}
				newMasterConnect.push_back(connectV);
			}
		}
		else {
			break;
		}
	}

	masterConnection = newMasterConnect;
}

double MLP::train(LinkedList<vector<double>, vector<double>> inputs) {
	double lastError = 10000;
	double smallestError = 10000;
	for (int s = 1; s < 1001; s++) {
		double error = 0;
		for (int a = 0; a < inputs.size(); a++) {
			double lineError = 0;
			MLP::fix(inputs.getKeyI(a), inputs.getValueI(a));

			if (batch) {
				for (int i = 0; i < masterConnection.size(); i++) {
					for (int b = 0; b < masterConnection.at(i).size(); b++) {
						double before = masterConnection.at(i).at(b).weight;
						masterConnection.at(i).at(b).weight -= (masterConnection.at(i).at(b).nextWeight / inputs.size()) + momentum *(masterConnection.at(i).at(b).weight - masterConnection.at(i).at(b).epochWeight);
						masterConnection.at(i).at(b).epochWeight = before;
						masterConnection.at(i).at(b).nextWeight = 0;
					}
				}

				for (int b = 0; b < inputs.size(); b++) {
					double lineError = 0;
					process(inputs.getKeyI(b));
					for (int i = 0; i < inputs.getValueI(b).size(); i++) {
						//lineError += .5 * pow(inputs.getValueI(a).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value, 2);
						lineError += abs(inputs.getValueI(b).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value);
					}

					error += lineError;
				}
			}
		}
		networkError = error;

		if (batch) {
			for (int i = 0; i < masterConnection.size(); i++) {
				for (int b = 0; b < masterConnection.at(i).size(); b++) {
					masterConnection.at(i).at(b).weight = masterConnection.at(i).at(b).nextWeight / inputs.size();
				}
				//masterConnection.at(i).weight = masterConnection.at(i).nextWeight / inputs.size();
			}
		}

		cout << "gen: " << s << " error: " << error << " change: " << error - lastError << " average: " << error / inputs.size() / neuralNetwork.at(neuralNetwork.size() - 1).size() << endl;

		if (s > 1) {
			//adjustLearningRate(error, lastError);
		}

		if (error < smallestError) {
			save();
			smallestError = error;
		}

		lastError = error;
	}

	return lastError;
}
double MLP::train(int end) {
	return train(trainingValues);
}
double MLP::train(int end, string extension) {
	ofstream stream("history" + extension + ".txt", ios::trunc);

	double lastError = 10000;
	double smallestError = 10000;
	LinkedList<vector<double>, vector<double>> inputs = trainingValues;

	for (int s = 1; s < end; s++) {
		double error = 0;
		for (int a = 0; a < inputs.size(); a++) {
			double lineError = 0;
			MLP::fix(inputs.getKeyI(a), inputs.getValueI(a));

			if (!batch) {
				for (int i = 0; i < masterConnection.size(); i++) {
					for (int b = 0; b < masterConnection.at(i).size(); b++) {
						//int sendId = masterConnection.at(i).at(b).sendId;
						//int connectId = masterConnection.at(i).at(b).connectToID;
						masterConnection.at(i).at(b).weight = masterConnection.at(i).at(b).weight - masterConnection.at(i).at(b).nextWeight;
						masterConnection.at(i).at(b).nextWeight = 0;
					}

				}

				process();

				for (int i = 0; i < inputs.getValueI(a).size(); i++) {
					//lineError += .5 * pow(inputs.getValueI(a).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value, 2);
					lineError += abs(inputs.getValueI(a).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value);
				}
				error += lineError;
			}
		}
		if (batch) {
			for (int i = 0; i < masterConnection.size(); i++) {
				for (int b = 0; b < masterConnection.at(i).size(); b++) {
					double before = masterConnection.at(i).at(b).weight;
					masterConnection.at(i).at(b).weight -= (masterConnection.at(i).at(b).nextWeight / inputs.size()) + momentum *(masterConnection.at(i).at(b).weight - masterConnection.at(i).at(b).epochWeight);
					masterConnection.at(i).at(b).epochWeight = before;
					masterConnection.at(i).at(b).nextWeight = 0;
				}
			}

			for (int b = 0; b < inputs.size(); b++) {
				double lineError = 0;
				process(inputs.getKeyI(b));
				for (int i = 0; i < inputs.getValueI(b).size(); i++) {
					//lineError += .5 * pow(inputs.getValueI(a).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value, 2);
					lineError += abs(inputs.getValueI(b).at(i) - neuralNetwork.at(neuralNetwork.size() - 1).at(i).value);
				}

				error += lineError;
			}
		}

		networkError = error;

		stream << to_string(s) << " " << to_string(error) << endl;

		cout << "gen: " << s << " error: " << error << " change: " << error - lastError << " average: " << error / inputs.size() / neuralNetwork.at(neuralNetwork.size() - 1).size() << endl;

		if (error < smallestError) {
			save();
			smallestError = error;
		}

		lastError = error;
	}

	stream.close();

	return lastError;
}

/*void MLP::autoPrune() {
	cout << "pruning" << endl;
	vector<int> hiddenLayers;
	vector<double> bestWeights;

	for (int i = 0; i < masterConnection.size(); i++) {
		bestWeights.push_back(masterConnection.at(i).weight);
	}

	for (int i = 1; i < neuralNetwork.size() - 1; i++) {
		hiddenLayers.push_back(neuralNetwork.at(i).size());
	}

	double smallestError = train(500);

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

			initialize(neuralNetwork.at(0).size(), newHiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size(), batch);
			double currentError = train(500);

			if (currentError < smallestError) {
				smallestError = currentError;

				cout << "dimesions: ";
				for (int i = 0; i < newHiddenLayers.size(); i++) {
					cout << newHiddenLayers.at(i) << " ";
				}

				for (int i = 0; i < masterConnection.size(); i++) {
					bestWeights.push_back(masterConnection.at(i).weight);
				}

				cout << "smallest: " << smallestError << endl;

				cout << "continue" << endl;
				hiddenLayers = newHiddenLayers;
			}
			else {
				cout << "dimesions: ";
				for (int i = 0; i < hiddenLayers.size(); i++) {
					cout << hiddenLayers.at(i) << " ";
				}
				cout << "smallest: " << smallestError << endl;
				break;
			}
		}
	}

	cout << "dimesions: ";
	for (int i = 0; i < hiddenLayers.size(); i++) {
		cout << hiddenLayers.at(i) << " ";
	}
	cout << "smallest: " << smallestError << endl;

	initialize(neuralNetwork.at(0).size(), hiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size(), batch);

	for (int i = 0; i < masterConnection.size(); i++) {
		masterConnection.at(i).weight = bestWeights.at(i);
	}
}
void MLP::autoPrune(int max, string extension) {
	cout << "pruning" << endl;
	vector<int> hiddenLayers;
	vector<double> bestWeights;

	for (int i = 0; i < masterConnection.size(); i++) {
		bestWeights.push_back(masterConnection.at(i).weight);
	}

	for (int i = 1; i < neuralNetwork.size() - 1; i++) {
		hiddenLayers.push_back(neuralNetwork.at(i).size());
	}

	string newExtension = extension;
	for (int i = 0; i < hiddenLayers.size(); i++) {
		newExtension = newExtension + " " + to_string(hiddenLayers.at(i));
	}
	double smallestError = train(max, newExtension);

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

			initialize(neuralNetwork.at(0).size(), newHiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size(), batch);
			string newExtensiona = extension;
			for (int i = 0; i < hiddenLayers.size(); i++) {
				newExtensiona = newExtensiona + " " + to_string(newHiddenLayers.at(i));
			}
			double currentError = train(max, newExtensiona);

			if (currentError < smallestError) {
				smallestError = currentError;

				cout << "dimesions: ";
				for (int i = 0; i < newHiddenLayers.size(); i++) {
					cout << newHiddenLayers.at(i) << " ";
				}

				for (int i = 0; i < masterConnection.size(); i++) {
					bestWeights.push_back(masterConnection.at(i).weight);
				}

				cout << "smallest: " << smallestError << endl;

				cout << "continue" << endl;
				hiddenLayers = newHiddenLayers;
			}
			else {
				cout << "dimesions: ";
				for (int i = 0; i < hiddenLayers.size(); i++) {
					cout << hiddenLayers.at(i) << " ";
				}
				cout << "smallest: " << smallestError << endl;
				break;
			}
		}
	}

	cout << "dimesions: ";
	for (int i = 0; i < hiddenLayers.size(); i++) {
		cout << hiddenLayers.at(i) << " ";
	}
	cout << "smallest: " << smallestError << endl;

	initialize(neuralNetwork.at(0).size(), hiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size(), batch);

	for (int i = 0; i < masterConnection.size(); i++) {
		masterConnection.at(i).weight = bestWeights.at(i);
	}
}

void MLP::autoPrune(LinkedList<vector<double>, vector<double>> input, int layerNumber) {
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

		initialize(neuralNetwork.at(0).size() - 1, newHiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size(), batch);
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

	initialize(neuralNetwork.at(0).size() - 1, hiddenLayers, neuralNetwork.at(neuralNetwork.size() - 1).size(), batch);
}*/