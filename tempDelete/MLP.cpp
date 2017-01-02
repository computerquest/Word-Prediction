#include "MLP.h"

MLP::MLP() {}
void MLP::process() {
	//setInput(currentInput);

	for (int i = 1; i < neuralNetwork.size(); i++) {
		for (int a = 0; a < neuralNetwork.at(i).size(); a++) {
			neuralNetwork.at(i).at(a).setValue(calcTanh(calcNetInput(neuralNetwork.at(i).at(a).id)));
		}
	}
}
void MLP::process(vector<double> input) {
	currentInput = input;
	setInput(currentInput);

	for (int i = 1; i < neuralNetwork.size(); i++) {
		for (int a = 0; a < neuralNetwork.at(i).size(); a++) {
			neuralNetwork.at(i).at(a).setValue(calcTanh(calcNetInput(neuralNetwork.at(i).at(a).id)));
		}
	}
}

void MLP::fix(vector<double> inputs, vector<double> desired) {
	currentInput = inputs;
	//loop for each output node

	//this sets the beginning values
	setInput(currentInput);

	process();

	//the output and the connections connecting to the output
	{
		//double target;
		for (int i = 0; i < (neuralNetwork.at(neuralNetwork.size() - 1)).size(); i++) {
			Neuron *outputNeuron = &(neuralNetwork.at(neuralNetwork.size() - 1)).at(i);
			double outToNet = tanhDerivative(outputNeuron->value);

			outputNeuron->influence = outputNeuron->value - desired.at(i);

			//connecting directly to output
			{
				vector<Connection*> connection = getConnectionsTo((outputNeuron->id));
				for (int a = 1; a < connection.size(); a++) {
					Connection *currentConnection = connection.at(a);
					currentConnection->nextWeight += outToNet * (findNeuron(currentConnection->sendId))->value * outputNeuron->influence*learningRate;
				}
			}
		}
	}

	//hidden
	for (int z = 2; neuralNetwork.size() - z > 0; z++) {

		for (int b = 0; b < neuralNetwork.at(neuralNetwork.size() - z).size(); b++) { //hidden node
			Neuron* currentHidden = &neuralNetwork.at(neuralNetwork.size() - z).at(b);
			vector<Connection*> connection = getConnectionsTo((currentHidden->id));
			double derivitive = tanhDerivative(currentHidden->value);

			double sumInfluence = 0;
			for (int i = 0; i < neuralNetwork.at(neuralNetwork.size() - z + 1).size(); i++) {
				sumInfluence += (neuralNetwork.at(neuralNetwork.size() - z + 1).at(i)).influence*findConnection(currentHidden->id, neuralNetwork.at(neuralNetwork.size() - z + 1).at(i).id)->weight;
			}

			currentHidden->influence = sumInfluence;

			for (int i = 0; i < connection.size(); i++) {
				connection.at(i)->nextWeight += learningRate*(derivitive* (currentHidden->influence) * findNeuron(connection.at(i)->sendId)->value);
			}
		}
	}
}

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

double MLP::train(LinkedList<vector<double>, vector<double>> inputs, int end) {
	double lastError = 10000;
	double smallestError = 10000;
	for (int s = 1; s < end; s++) {
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
			}
		}

		cout << "gen: " << s << " error: " << error << " change: " << error - lastError << " average: " << error / inputs.size() / neuralNetwork.at(neuralNetwork.size() - 1).size() << endl;

		if (error < smallestError) {
			save();
			smallestError = error;
		}

		lastError = error;
	}

	return lastError;
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
						masterConnection.at(i).at(b).weight = masterConnection.at(i).at(b).weight - masterConnection.at(i).at(b).nextWeight;
						masterConnection.at(i).at(b).nextWeight = 0;
					}

				}

				process();

				for (int i = 0; i < inputs.getValueI(a).size(); i++) {
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