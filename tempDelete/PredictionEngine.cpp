#pragma once
#include "PredictionEngine.h"
#include "MasterResource.h"

PredictionEngine::PredictionEngine(int inputN, vector<int> hidden, int output, MasterResource* resource) {
	std::cout << "new engine created" << endl;
	master = resource;
	nn.initializeNN(inputN, hidden, output);
	readTraining();
	//read();
}

void PredictionEngine::saveTraining() {
	ofstream stream;
	stream.open("trainingExamples.txt", ios::trunc);

	for (int i = 0; i < examples.size(); i++) {
		stream << examples.at(i) << endl;
	}
}
void PredictionEngine::readTraining() {
	ifstream stream;
	stream.open("trainingExamples.txt");

	string line;
	while (getline(stream, line)) {
		examples.push_back(line);
	}
}

void PredictionEngine::save() {
	ofstream stream;
	stream.open("weights.txt", ios::trunc);

	for (int a = 0; a < nn.connections.size(); a++) {
		stream << nn.connections.at(a)->weight << endl;
	}
	stream.close();
}
void PredictionEngine::read() {
	ifstream stream;
	stream.open("weights.txt");
	string temp;
	for (int i = 0; i < nn.connections.size() && std::getline(stream, temp); i++) {
		nn.connections.at(i)->weight = atof(temp.c_str());
	}
	stream.close();
}

LinkedList<vector<double>, vector<double>> PredictionEngine::createTrain(vector<Neuron> neurons, vector<vector<double>> desiredI) {
	LinkedList<vector<double>, vector<double>> masterPrint;

	for (int i = 0; i < neurons.size(); i++) {
		vector<double> blueprint;
		vector<double> desired = desiredI.at(i);

		for (int a = 0; a < neurons.size(); a++) {
			if (a == i) {
				blueprint.push_back(1);
			}

			blueprint.push_back(0);
		}

		masterPrint.add(blueprint, desired);
	}

	return masterPrint;
}

void PredictionEngine::findType() {
	double lastError = 1000000000;
	double error = 0;
	int strike = 3;
	int generation = 0;
	int smallest = 1000000;
	int total = 0;

	while (true) {
		total = 0;
		generation++;

		for (int i = 0; i < examples.size(); i++) {
			vector<string> wordStrings = breakDownV(examples.at(i));

			for (int a = 0; a < wordStrings.size(); a++) {
				bool badOne = false;

				//did the wierd condition because if it set it to negative one normally it returns false
				for (int secondMissing = -1; secondMissing + 1 < wordStrings.size() + 1; secondMissing++) {
					if (secondMissing == a) {
						continue;
					}

					vector<POS> wtype;
					for (int b = 0; b < wordStrings.size(); b++) {
						POS type = master->findWord(wordStrings.at(b)).type;

						if (b == a || b == secondMissing) {
							if (type != POS::Noun && type != POS::Verb && type != POS::Adjective && b != secondMissing) {
								badOne = true;
							}

							wtype.push_back(POS::Unknown);
							continue;
						}

						wtype.push_back(type);
					}

					if (badOne == true) {
						continue;
					}

					bool aUsed = false;

					for (int findI = 0; findI < 2; findI++) {
						POS newType;
						if (secondMissing == -1 & aUsed == false | aUsed == false & a < secondMissing) {
							newType = findTypeDeployment(wtype, examples.at(i), a);
							aUsed = true;
							wtype.at(a) = newType;
							total++;
						}
						else if (secondMissing > -1 && aUsed == true | secondMissing < a) {
							newType = findTypeDeployment(wtype, examples.at(i), secondMissing);
							wtype.at(secondMissing) = newType;
							total++;
						}

						vector<Neuron> neurons = nn.neuralNetwork.at(0);
						vector<double> desired;

						double nounV = neurons.at(1).value + neurons.at(2).value + neurons.at(3).value + neurons.at(5).value + neurons.at(4).value
							+ neurons.at(6).value + neurons.at(27).value + neurons.at(13).value + neurons.at(14).value + neurons.at(17).value + neurons.at(16).value
							+ neurons.at(15).value;

						double verbV = neurons.at(0).value + neurons.at(1).value + neurons.at(2).value + neurons.at(3).value + neurons.at(5).value +
							neurons.at(4).value + neurons.at(6).value + neurons.at(28).value + (neurons.at(1).value == 0 ? neurons.at(1).value : 0) + neurons.at(12).value
							+ neurons.at(14).value + neurons.at(17).value + neurons.at(16).value + neurons.at(15).value;

						double adjectiveV = neurons.at(0).value + neurons.at(1).value + neurons.at(2).value + neurons.at(3).value + neurons.at(5).value
							+ neurons.at(4).value + neurons.at(6).value + neurons.at(29).value + neurons.at(13).value + neurons.at(12).value + neurons.at(14).value
							+ neurons.at(17).value + neurons.at(16).value + neurons.at(15).value;

						double total = nounV + verbV + adjectiveV > 0 ? nounV + verbV + adjectiveV : -1;

						if (master->findWord(wordStrings.at(a)).type == POS::Noun) {
							desired.push_back(1);
							desired.push_back((verbV / total) - 1);
							desired.push_back((adjectiveV / total) - 1);
						}
						else if (master->findWord(wordStrings.at(a)).type == POS::Verb) {
							desired.push_back((nounV / total) - 1);
							desired.push_back(1);
							desired.push_back((adjectiveV / total) - 1);
						}
						else if (master->findWord(wordStrings.at(a)).type == POS::Adjective) {
							desired.push_back((nounV / total) - 1);
							desired.push_back((verbV / total) - 1);
							desired.push_back(1);
						}

						training.add(nn.currentInput, desired);
						nn.fix(desired);
						error += nn.networkError;
					}
				}
			}
		}

		std::cout << endl;
		std::cout << "Generation: " << generation << " Error: " << error << " Change: " << error - lastError;

		if (total > 0) {
			std::cout << " Average: " << (error / 3 / total) << endl;
		}
		if (error - lastError > -.00001) {
			if (strike > 0) {
				strike--;
			}
			else {
				break;
			}
		}
		else if (error - lastError < -.0001) {
			strike = 3;
		}

		if (error < smallest) {
			smallest = error;
		}
		lastError = error;
	}

	if (total > 0) {
		std::cout << "Smallest: " << smallest << " Average: " << (smallest / 3 / total) << " Total: " << total << endl;
	}
} //can call find type deployment from method and add loop

POS PredictionEngine::findTypeDeployment(vector<POS> wtype, string phrase, int targetWordIndex) {
	vector<string> wordStrings = breakDownV(phrase);
	LinkedList<POS, int> typeBlocks;
	POS lastPOS = POS::Unknown;

	//creates POS blocks
	for (int i = 0; i < wtype.size(); i++) {
		//check if it is a definitive word
		if (wtype.at(i) == POS::Verb | wtype.at(i) == POS::Noun | wtype.at(i) == POS::Preposition | wtype.at(i) == POS::Conjuction) {
			if (lastPOS == POS::Preposition | lastPOS == POS::Verb | lastPOS == POS::Conjuction) {
				typeBlocks.add(wtype.at(i), i);
				typeBlocks.add(lastPOS != POS::Unknown ? lastPOS : wtype.at(i), i - 1);
				continue;
			}

			typeBlocks.add(lastPOS != POS::Unknown ? lastPOS : wtype.at(i), i - 1);
			lastPOS = wtype.at(i);
		}
	}

	//does it fall within noun block
	double nounBlock = 0;

	//what block is in front
	double frontBlockNoun = 0;
	double frontBlockVerb = 0;
	double frontBlockConjunction = 0;
	double frontBlockPreposition = 0;

	//what block is in back
	double backBlockNoun = 0;
	double backBlockVerb = 0;
	double backBlockConjunction = 0;
	double backBlockPreposition = 0;

	//what blocks there are
	double hasNounBlock = 0;
	double hasVerbBlock = 0;

	//ALL BLOCK STUFF
	{
		int wordBlock = -1;

		//find which block is yours
		for (int i = 0; i < typeBlocks.size() && i > 0 ? (typeBlocks.getValueI(i - 1) < targetWordIndex) : true; i++) {
			int currentLast = typeBlocks.getValueI(i);
			int currentFirst = 0;
			if (i > 0) {
				int currentFirst = typeBlocks.getValueI(i - 1);
			}

			if (i <= currentLast && i >= currentFirst) {
				wordBlock = i;

				if (typeBlocks.getKeyI(i) == POS::Noun) {
					nounBlock = 1;
				}

				break;
			}
		}

		//front blocks
		if (wordBlock > 0) {
			POS type = typeBlocks.getKeyI(wordBlock - 1);

			if (type == POS::Noun) {
				frontBlockNoun = 1;
			}
			else if (type == POS::Verb) {
				frontBlockVerb = 1;
			}
			else if (type == POS::Conjuction) {
				frontBlockConjunction = 1;
			}
			else if (type == POS::Preposition) {
				frontBlockPreposition = 1;
			}
		}

		//back blocks
		if (wordBlock < typeBlocks.size() - 1) {
			POS type = typeBlocks.getKeyI(wordBlock + 1);

			if (type == POS::Noun) {
				backBlockNoun = 1;
			}
			else if (type == POS::Verb) {
				backBlockVerb = 1;
			}
			else if (type == POS::Conjuction) {
				backBlockConjunction = 1;
			}
			else if (type == POS::Preposition) {
				backBlockPreposition = 1;
			}
		}

		//has blocks
		hasNounBlock = vectorContains(typeBlocks.key, POS::Noun);
		hasNounBlock = vectorContains(typeBlocks.key, POS::Verb);
	}

	vector<SStructure> possibleStructure = master->matchPossibleSS(wtype);

	//LAST WORD
	//the type of the word before
	double lastWTypeNoun = 0;
	double lastWTypeAdjective = 0;
	double lastWTypeVerb = 0;
	double lastWTypePreposition = 0;
	double lastWTypeConjunction = 0;
	double lastWTypeArticle = 0;

	//the ngrams
	double indicatesNoun = 0;
	double indicatesVerb = 0;
	double indicatesAdjective = 0;
	if (targetWordIndex > 0) {
		POS type = wtype.at(targetWordIndex - 1);

		if (type == POS::Adjective) {
			lastWTypeAdjective = 1;
		}
		else if (type == POS::Noun) {
			lastWTypeNoun = 1;
		}
		else if (type == POS::Verb) {
			lastWTypeVerb = 1;
		}
		else if (type == POS::Conjuction) {
			lastWTypeConjunction = 1;
		}
		else if (type == POS::Preposition) {
			lastWTypePreposition = 1;
		}
		else if (type == POS::Article) {
			lastWTypeArticle = 1;
		}

		NGram<Word> lastWordNGram = master->findNGram(master->findWord(wordStrings.at(targetWordIndex - 1)));
		LinkedList<int, Word> usedWords = lastWordNGram.content;

		double usedNouns = 0;
		double usedVerbs = 0;
		double usedAdjectives = 0;
		double total = 0;
		for (int i = 0; i < usedWords.size(); i++) {
			POS type = usedWords.getValueI(i).type;
			if (type == POS::Noun) {
				usedNouns++;
			}
			else if (type == POS::Verb) {
				usedVerbs++;
			}
			else if (type == POS::Adjective) {
				usedAdjectives++;
			}
			else {
				total++;
			}
		}

		double total = usedNouns + usedVerbs + usedAdjectives + total;

		indicatesNoun = usedNouns / total;
		indicatesVerb = usedVerbs / total;
		indicatesAdjective = usedAdjectives / total;
	}

	//THE WORD AFTER
	//the type of the next word
	double afterNoun = 0;
	double afterVerb = 0;
	double afterAdjective = 0;
	double afterConjunction = 0;
	double afterPreposition = 0;
	double afterArticle = 0;

	//the type of word that generally calls the after word
	double afterCallerNoun = 0;
	double afterCallerVerb = 0;
	double afterCallerAdjective = 0;
	double afterCallerConjunction = 0;
	double afterCallerPreposition = 0;
	double afterCallerArticle = 0;
	if (targetWordIndex < wordStrings.size() - 1) {
		POS type = wtype.at(targetWordIndex + 1);

		if (type == POS::Adjective) {
			afterAdjective = 1;
		}
		else if (type == POS::Noun) {
			afterNoun = 1;
		}
		else if (type == POS::Verb) {
			afterVerb = 1;
		}
		else if (type == POS::Conjuction) {
			afterConjunction = 1;
		}
		else if (type == POS::Preposition) {
			afterPreposition = 1;
		}
		else if (type == POS::Article) {
			afterArticle = 1;
		}

		//see which types of words use this word
		Word afterWord = master->findWord(wordStrings.at(targetWordIndex + 1));
		LinkedList<double, POS> wordCall;
		for (int i = 0; i < master->ngramML.size(); i++) {
			NGram<Word> currentNGram = master->ngramML.at(i);

			if (currentNGram.content.containsV(afterWord)) {
				wordCall.add(currentNGram.content.getKeyV(afterWord), currentNGram.subject.type);
			}
		}

		double nounUsed = 0;
		double verbUsed = 0;
		double adjectiveUsed = 0;
		double conjunctionUsed = 0;
		double prepositionUsed = 0;
		double articleUsed = 0;
		double total = 0;
		for (int i = 0; i < wordCall.size(); i++) {
			POS callerType = wordCall.getValueI(i);

			if (callerType == POS::Noun) {
				nounUsed += wordCall.getKeyI(i);
			}
			else if (callerType == POS::Verb) {
				verbUsed += wordCall.getKeyI(i);
			}
			else if (callerType == POS::Adjective) {
				adjectiveUsed += wordCall.getKeyI(i);
			}
			else if (callerType == POS::Conjuction) {
				conjunctionUsed += wordCall.getKeyI(i);
			}
			else if (callerType == POS::Preposition) {
				prepositionUsed += wordCall.getKeyI(i);
			}
			else if (callerType == POS::Article) {
				articleUsed += wordCall.getKeyI(i);
			}
		}
		total = nounUsed + verbUsed + adjectiveUsed + conjunctionUsed + prepositionUsed + articleUsed;

		//creates the ratio
		afterCallerNoun = nounUsed / total;
		afterCallerVerb = verbUsed / total;
		afterCallerAdjective = adjectiveUsed / total;
	}

	//POSSIBLE STRUCTURE
	//ratio of what is at target
	double nounPossible = 0;
	double verbPossible = 0;
	double adjectivePossible = 0;

	if (possibleStructure.size() > 0) {
		double nounOccerence = 0;
		double verbOccerence = 0;
		double adjectiveOccerence = 0;

		for (int i = 0; i < possibleStructure.size(); i++) {
			POS type = possibleStructure.at(i).component.at(targetWordIndex);

			if (type == POS::Noun) {
				nounOccerence++;
			}
			else if (type == POS::Verb) {
				verbOccerence++;
			}
			else if (type == POS::Adjective) {
				adjectiveOccerence++;
			}
		}
		double total = nounOccerence + verbOccerence + adjectiveOccerence;
		nounPossible = nounOccerence / total;
		verbPossible = verbOccerence / total;
		adjectivePossible = adjectiveOccerence / total;
	}
}

void PredictionEngine::filterExamples() {
	LinkedList<vector<POS>, string> newList;
	for (int i = 0; i < examples.size(); i++) {
		vector<POS> currentExample = master->findAllWordType(examples.at(i));

		newList.add(currentExample, examples.at(i));
	}

	newList.filterDuplicatesK();

	examples = newList.value;
	saveTraining();
}

void PredictionEngine::generateData(int amount) {
	srand(time(0));
	for (int i = 0; i < amount; i++) {
		int current = (std::rand() % (master->ngramML.size() - 1));
		NGram<Word>* currentWord = &master->ngramML.at(current);

		while (true) {
			int x = (std::rand() % (master->ngramML.size() - 1));
			Word currentWordProp = master->ngramML.at(x).subject;

			if (currentWord->subject.type != currentWordProp.type && currentWord->subject.name != currentWordProp.name) {
				currentWord->addItem(currentWordProp);
				currentWord->updateItem(currentWordProp, (int)(rand() % 15) + 1);
				break;
			}
		}
	}

	master->saveNGram();
}

vector<POS> PredictionEngine::multipleMissing(string phrase) {
	vector<string> wordStrings = breakDownV(phrase);
	vector<POS> wordTypes;
	vector<int> flagged;
	vector<int> flaggedUnknown;

	for (int i = 0; i < wordStrings.size(); i++) {
		if (master->isDouble(wordStrings.at(i))) {
			flagged.push_back(i);
			wordTypes.push_back(POS::Unknown);
		}
		else if (master->wordExist(wordStrings.at(i))) {
			wordTypes.push_back(master->findWord(wordStrings.at(i)).type);
		}
		else {
			wordTypes.push_back(POS::Unknown);
			flaggedUnknown.push_back(i);
		}
	}

	for (int i = 0; i < flagged.size(); i++) {
		vector<POS> possible = master->doublePossibilities(wordStrings.at(flagged.at(i)));

		double one = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(0).value;
		double two = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(1).value;
		double three = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(2).value;

		if (one > two && one > three && vectorContains(possible, POS::Noun)) {
			wordTypes[flagged.at(i)] = POS::Noun;
		}
		else if (two > one && two > three&& vectorContains(possible, POS::Verb)) {
			wordTypes[flagged.at(i)] = POS::Verb;
		}
		else if (three > one && three > two&& vectorContains(possible, POS::Adjective)) {
			wordTypes[flagged.at(i)] = POS::Adjective;
		}

		wordTypes[flagged.at(i)] = findTypeDeployment(wordTypes, phrase, flagged.at(i));
	}

	vector<POS> newPOS;
	for (int i = 0; i < flaggedUnknown.size(); i++) {
		POS newType = findTypeDeployment(wordTypes, phrase, flaggedUnknown.at(i));
		wordTypes[flaggedUnknown.at(i)] = newType;
		newPOS.push_back(newType);
		std::cout << wordStrings.at(flaggedUnknown.at(i)) << " New Type: " << POStoString(newType) << endl;
	}

	return newPOS;
}
vector<POS> PredictionEngine::multipleMissingTraining(string phrase) {
	vector<string> wordStrings = breakDownV(phrase);
	vector<POS> wordTypes;
	vector<int> flagged;
	vector<int> flaggedUnknown;

	for (int i = 0; i < wordStrings.size(); i++) {
		if (master->isDouble(wordStrings.at(i))) {
			flagged.push_back(i);
			wordTypes.push_back(POS::Unknown);
		}
		else if (master->wordExist(wordStrings.at(i))) {
			wordTypes.push_back(master->findWord(wordStrings.at(i)).type);
		}
		else {
			wordTypes.push_back(POS::Unknown);
			flaggedUnknown.push_back(i);
		}
	}

	for (int i = 0; i < flagged.size(); i++) {
		vector<POS> possible = master->doublePossibilities(wordStrings.at(flagged.at(i)));

		double one = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(0).value;
		double two = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(1).value;
		double three = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(2).value;

		if (one > two && one > three && vectorContains(possible, POS::Noun)) {
			wordTypes[flagged.at(i)] = POS::Noun;
		}
		else if (two > one && two > three&& vectorContains(possible, POS::Verb)) {
			wordTypes[flagged.at(i)] = POS::Verb;
		}
		else if (three > one && three > two&& vectorContains(possible, POS::Adjective)) {
			wordTypes[flagged.at(i)] = POS::Adjective;
		}

		wordTypes[flagged.at(i)] = findTypeDeployment(wordTypes, phrase, flagged.at(i));
	}

	vector<POS> newPOS;
	for (int i = 0; i < flaggedUnknown.size(); i++) {
		POS newType = findTypeDeployment(wordTypes, phrase, flaggedUnknown.at(i));
		wordTypes[flaggedUnknown.at(i)] = newType;
		newPOS.push_back(newType);
		std::cout << wordStrings.at(flaggedUnknown.at(i)) << " New Type: " << POStoString(newType) << endl;
	}

	return newPOS;
}

void PredictionEngine::mergeTraining() {
	for (int i = 0; i < training.size(); i++) {
		vector<double> currentInput = training.getKeyI(i);

		for (int a = 0; a < training.size(); a++) {
			if (i == a) {
				continue;
			}
			vector<double> secondaryInput = training.getKeyI(a);
			bool similar = true;

			for (int c = 0; c < secondaryInput.size(); c++) {
				if (secondaryInput.at(c) == 0 & currentInput.at(c) == 1 || secondaryInput.at(c) == 1 & currentInput.at(c) == 1) {
					similar = false;
					break;
				}
			}

			if (similar == false) {
				continue;
			}

			vector<double> currentDesired = training.getValueI(i);
			vector<double> secondaryDesired = training.getValueI(a);
			for (int c = 0; c < currentDesired.size(); c++) {
				currentDesired.at(c) = currentDesired.at(c) >= secondaryDesired.at(c) ? currentDesired.at(c) : secondaryDesired.at(c);
			}

			training.deleteIndex(a);
		}
	}
}