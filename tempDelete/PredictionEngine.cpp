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
				for (int secondMissing = -1; secondMissing+1 < wordStrings.size()+1; secondMissing++) {
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

POS PredictionEngine::findTypeDeployment(vector<POS> wtype, string phrase, int i) {
	vector<Word> word;
	vector<double> masterPrint;
	vector<string> wordS = breakDownV(phrase);
	int unknown = 0;
	SStructure lastPhraseStruct = master->findStructurePercision(lastPhraseGlobal);

	Neuron nounI(0);
	Neuron verbI(0);
	Neuron adjectiveI(0);
	Neuron conjunctionI(0); // have list
	Neuron articleI(0);//have list
	Neuron prepositionI(0); //have list

	Neuron predictN(0);
	Neuron predictV(0);
	Neuron predictA(0);

	Neuron endA(0);
	Neuron endVerb(0);
	Neuron endN(0);

	Neuron nextNoun(0);
	Neuron nextVerb(0);
	Neuron nextAdjective(0);
	Neuron nextArticle(0);
	Neuron nextPreposition(0);
	Neuron nextConjunction(0);

	Neuron prepositionNG(0);
	Neuron nounNG(0);
	Neuron verbNG(0);
	Neuron adjectiveNG(0);
	Neuron conjunctionNG(0);
	Neuron articleNG(0);

	Neuron bestNounN(0);
	Neuron bestVerbN(0);
	Neuron bestAdjectiveN(0);

	Neuron bestNounWn(0);
	Neuron bestVerbWn(0);
	Neuron bestAdjectiveWn(0);

	Neuron wordLength(0);
	Neuron vowelLength(0);
	Neuron constanantLength(0);

	Neuron nounN(0);
	Neuron verbN(0);
	Neuron adjectiveN(0);
	Neuron conjunctionN(0);
	Neuron prepositionN(0);
	Neuron articleN(0);

	vector<double> blueprint;
	SStructure structure;

	structure = master->findPartailStructure(wtype);

	vector<SStructure> possibilities = master->matchPossibleSS(wtype);
	//inputs nounI
	if (i > 0 && wtype.size() > 1) {
		POS type = wtype.at(i - 1);

		if (type == POS::Noun) {
			nounI.setValue(1);
		}
		else if (type == POS::Verb) {
			verbI.setValue(1);
		}
		else if (type == POS::Adjective) {
			adjectiveI.setValue(1);
		}
		else if (type == POS::Preposition) {
			prepositionI.setValue(1);
		}
		else if (type == POS::Conjuction) {
			conjunctionI.setValue(1);
		}
		else if (type == POS::Article) {
			articleI.setValue(1);
		}
	}

	//predict with ss predictN
	if (structure.component.size() > 1) {
		POS type = structure.component.at(i);

		if (type == POS::Noun) {
			predictN.setValue(1);
		}
		else if (type == POS::Verb) {
			predictV.setValue(1);
		}
		else if (type == POS::Adjective) {
			predictA.setValue(1);
		}
	}

	//next nextNoun
	if (i < wtype.size() - 1 && (structure.component.size() > 1)) {
		POS type = structure.component.at(i + 1);

		if (type == POS::Noun) {
			nextNoun.setValue(1);
		}
		else if (type == POS::Verb) {
			nextVerb.setValue(1);
		}
		else if (type == POS::Adjective) {
			nextAdjective.setValue(1);
		}
		else if (type == POS::Article) {
			nextArticle.setValue(1);
		}
		else if (type == POS::Preposition) {
			nextPreposition.setValue(1);
		}
		else if (type == POS::Conjuction) {
			nextConjunction.setValue(1);
		}
	}

	//ending endN
	{
		if ((wordS.at(i)).length() > 3) {
			string end = wordS.at(i).substr(wordS.at(i).length() - 1, 1);
			string end2 = "";
			string end3 = "";
			string end4 = "";

			if (wordS.at(i).length() > 4) {
				end2 = wordS.at(i).substr(wordS.at(i).length() - 2, 2);
				if (wordS.at(i).length() > 5) {
					end3 = wordS.at(i).substr(wordS.at(i).length() - 3, 3);

					if (wordS.at(i).length() > 6) {
						end4 = wordS.at(i).substr(wordS.at(i).length() - 4, 4);
					}
				}
			}

			if (end == "s" || end3 == "ion" || end3 == "acy" || end4 == "ence" || end4 == "ance" || end4 == "hood" || end2 == "ar" || end2 == "or"
				|| end3 == "ism" || end3 == "ist" || end4 == "ment" || end4 == "ness" || end == "y" || end3 == "ity") {
				endN.setValue(1);
			}

			if (end2 == "en" || end3 == "ify" || end3 == "ate" || end3 == "ize") {
				endVerb.setValue(1);
			}

			if (end2 == "al" || end3 == "ful" || end2 == "ly" || end2 == "ic" || end3 == "ish" || end4 == "like" || end3 == "ous" || end == "y"
				|| end3 == "ate" || end3 == "able" || end3 == "ible") {
				endA.setValue(1);
			}
		}
	}

	//most often type in value nounNG RATIO
	if (i > 0) {
		NGram<Word> wordNGram = master->findNGram(master->findWord(wordS.at(i - 1)));

		double pTotal = 0;
		double nTotal = 0;
		double vTotal = 0;
		double aTotal = 0;
		double cTotal = 0;
		double articleTotal = 0;
		double total = 0;
		for (int a = 0; a < wordNGram.content.size(); a++) {
			POS type = wordNGram.content.getValueI(a).type;
			if (type == POS::Noun) {
				nTotal += wordNGram.content.getKeyI(a);
			}
			else if (type == POS::Verb) {
				vTotal += wordNGram.content.getKeyI(a);
			}
			else if (type == POS::Adjective) {
				aTotal += wordNGram.content.getKeyI(a);
			}
			else if (type == POS::Conjuction) {
				cTotal += wordNGram.content.getKeyI(a);
			}
			else if (type == POS::Preposition) {
				pTotal += wordNGram.content.getKeyI(a);
			}
			else if (type == POS::Article) {
				articleTotal += wordNGram.content.getKeyI(a);
			}
		}

		total = nTotal + vTotal + aTotal + cTotal + pTotal + articleTotal;

		if (total > 0) {
			nounNG.setValue(nTotal / total);
			verbNG.setValue(vTotal / total);
			adjectiveNG.setValue(aTotal / total);
		}
	}

	//last structure stuff bestNounN
	{
		if (lastPhraseStruct.component.size() != 0) {
			NGram<SStructure> lastStruct = master->findNGramS(lastPhraseStruct.component);
			int index = -1;

			for (int a = 0; a < lastStruct.content.size(); a++) {
				vector<POS> currentPOS = lastStruct.content.getValueI(a).component;

				int greatest = 0;

				for (int b = 0; b < possibilities.size(); b++) {
					if (currentPOS == possibilities.at(b).component) {
						if (greatest < lastStruct.content.getKeyI(a)) {
							index = a;
							greatest = lastStruct.content.getKeyI(a);
						}
					}
				}
			}

			if (index != -1) {
				SStructure seconStruct = lastStruct.content.getValueI(index);

				if (seconStruct.component.size() > i + 1) {
					POS type = seconStruct.component.at(i);

					if (type == POS::Noun) {
						bestNounN.setValue(1);
					}
					else if (type == POS::Verb) {
						bestVerbN.setValue(1);
					}
					else if (type == POS::Adjective) {
						bestAdjectiveN.setValue(1);
					}
				}
			}
		}
	}

	//find the type of the most often occuring word bestWn
	if (i > 0) {
		NGram<Word> wordNGram = master->findNGram(master->findWord(wordS.at(i - 1)));
		int typeI = wordNGram.findGreatest();

		if (typeI != -1) {
			POS type = wordNGram.content.getValueI(typeI).type;

			if (type == POS::Noun) {
				bestNounWn.setValue(1);
			}
			else if (type == POS::Verb) {
				bestVerbWn.setValue(1);
			}
			else if (type == POS::Adjective) {
				bestAdjectiveWn.setValue(1);
			}
		}
	}

	//goes through possibility sstructure and finds the amount supported for each type nounN RATIO
	{
		double nTotal = 0;
		double vTotal = 0;
		double aTotal = 0;
		double cTotal = 0;
		double pTotal = 0;
		double articleTotal = 0;
		double total = 0;
		for (int a = 0; a < possibilities.size(); a++) {
			if (possibilities.at(a).component.size() > i) {
				POS type = possibilities.at(a).component.at(i);

				if (type == POS::Noun) {
					nTotal += 1;
				}
				else if (type == POS::Verb) {
					vTotal += 1;
				}
				else if (type == POS::Adjective) {
					aTotal += 1;
				}
				else if (type == POS::Conjuction) {
					cTotal += 1;
				}
				else if (type == POS::Preposition) {
					pTotal += 1;
				}
				else if (type == POS::Article) {
					articleTotal += 1;
				}
			}
		}

		total = nTotal + aTotal + vTotal + cTotal + pTotal + articleTotal;
		if (total > 0) {
			nounN.setValue(nTotal / total);
			verbN.setValue(vTotal / total);
			adjectiveN.setValue(aTotal / total);
		}
	}

	//type input from last word
	blueprint.push_back(nounI.value);
	blueprint.push_back(verbI.value);
	blueprint.push_back(adjectiveI.value);
	blueprint.push_back(conjunctionI.value);
	blueprint.push_back(prepositionI.value);
	blueprint.push_back(articleI.value);

	blueprint.push_back(predictN.value);
	blueprint.push_back(predictV.value);
	blueprint.push_back(predictA.value);

	//what the possiblities say could happen next in a ratio
	blueprint.push_back(nounN.value);
	blueprint.push_back(verbN.value);
	blueprint.push_back(adjectiveN.value);

	blueprint.push_back(nextNoun.value);
	blueprint.push_back(nextVerb.value);
	blueprint.push_back(nextAdjective.value);
	blueprint.push_back(nextConjunction.value);
	blueprint.push_back(nextPreposition.value);
	blueprint.push_back(nextArticle.value);

	//ratio of key and total size for each type of word 
	blueprint.push_back(nounNG.value);
	blueprint.push_back(verbNG.value);
	blueprint.push_back(adjectiveNG.value);

	//find anything possibility matching the last ngrams content NOT WORKING?
	blueprint.push_back(bestNounN.value);
	blueprint.push_back(bestVerbN.value);
	blueprint.push_back(bestAdjectiveN.value);

	//highest score word in ngram 
	blueprint.push_back(bestNounWn.value);
	blueprint.push_back(bestVerbWn.value);
	blueprint.push_back(bestAdjectiveWn.value);

	blueprint.push_back(endN.value);
	blueprint.push_back(endVerb.value);
	blueprint.push_back(endA.value);

	nn.process(blueprint);

	std::cout << "Find Type Values " << nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(0).value << " " << nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(1).value << " "
		<< nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(2).value << endl;

	vector<POS> anotherOne;
	for (int s = 0; s < wtype.size(); s++) {
		double noun = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(0).value;
		double verb = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(1).value;
		double adjective = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(2).value;
		if (wtype.at(s) == POS::Unknown) {
			if (noun > adjective && noun > verb) {
				anotherOne.push_back(POS::Noun);
			}
			else if (adjective > noun && adjective > verb) {
				anotherOne.push_back(POS::Adjective);
			}
			else {
				anotherOne.push_back(POS::Verb);
			}
		}
	}
	lastPhraseGlobal = anotherOne;

	double one = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(0).value;
	double two = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(1).value;
	double three = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(2).value;

	if (one > two && one > three) {
		return POS::Noun;
	}
	else if (two > one && two > three) {
		return POS::Verb;
	}
	else if (three > one && three > two) {
		return POS::Adjective;
	}

	return POS::Unknown;
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