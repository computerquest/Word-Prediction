#pragma once
#include "PredictionEngine.h"
#include "MasterResource.h"

PredictionEngine::PredictionEngine(int inputN, vector<int> hidden, int output, MasterResource* resource, bool batch) {
	std::cout << "new engine created" << endl;
	master = resource;
	nn.initialize(inputN, hidden, output, batch);
	readTraining();
	createTraining();
	//nn.read();
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

void PredictionEngine::createTraining() {
	for (int i = 0; i < examples.size(); i++) {
		vector<string> wordStrings = breakDownV(examples.at(i));
		vector<POS> wtype;

		for (int a = 0; a < wordStrings.size(); a++) {
			cout << wordStrings.at(a) << endl;
			wtype.push_back(master->findWord(wordStrings.at(a)).type);
		}

		for (int b = 0; b < wordStrings.size(); b++) {
			if (wtype.at(b) != POS::Noun && wtype.at(b) != POS::Verb && wtype.at(b) != POS::Adjective) {
				continue;
			}

			vector<POS> newWType = wtype;
			newWType[b] = POS::Unknown;

			vector<double> desired;

			if (wtype.at(b) == POS::Noun) {
				desired.push_back(1);
				desired.push_back(-1);
				desired.push_back(-1);
			}
			else if (wtype.at(b) == POS::Verb) {
				desired.push_back(-1);
				desired.push_back(1);
				desired.push_back(-1);
			}
			else if (wtype.at(b) == POS::Adjective) {
				desired.push_back(-1);
				desired.push_back(-1);
				desired.push_back(1);
			}

			findTypeDeployment(newWType, examples.at(i), b);
			training.add(nn.currentInput, desired);
		}
	}

	nn.trainingValues = training;
}

//31 32 33
POS PredictionEngine::findTypeDeployment(vector<POS> wtype, string phrase, int targetWordIndex) {
	vector<string> wordStrings = breakDownV(phrase);
	LinkedList<POS, int> typeBlocks;
	POS lastPOS = POS::Unknown;
	vector<double> neuralNetworkInput; //thought about making pointer but would have to modify nn class

									   //creates POS blocks

									   //run fat cat | run quickly jared
	POS lastSignificantPOS = POS::Unknown;
	for (int i = 0; i < wtype.size(); i++) {
		POS type = wtype.at(i);

		if (type == POS::Conjuction | type == POS::Preposition | type == POS::Adjective) {
			if (lastSignificantPOS != POS::Unknown) {
				typeBlocks.add(lastSignificantPOS, i - 1);
			}

			typeBlocks.add(type, i);

			lastSignificantPOS = POS::Unknown;
		}
		else if (type == POS::Verb | type == POS::Noun | type == POS::Article) {
			if (type == POS::Noun & lastSignificantPOS == POS::Noun) {
				continue;
			}

			if (lastSignificantPOS != POS::Unknown) {
				typeBlocks.add(lastSignificantPOS, i - 1);
			}

			lastSignificantPOS = type == POS::Article ? POS::Noun : type;
		}
	}

	if (lastSignificantPOS != POS::Unknown) {
		typeBlocks.add(lastSignificantPOS, wtype.size() - 1);
	}

	for (int i = 0; i < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		if (i > 0 && type == POS::Adjective) {
			POS secondary = typeBlocks.getKeyI(i - 1);

			if (secondary == POS::Adjective) {
				typeBlocks.changeValueI(i, typeBlocks.getValueI(i - 1));
			}
		}
	}

	//find prep phrases
	for (int i = 0; i + 1 < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		POS secondary = typeBlocks.getKeyI(i + 1);
		if (type == POS::Preposition && secondary == POS::Noun) {
			typeBlocks.changeValueI(i, typeBlocks.getValueI(i + 1));
			typeBlocks.deleteIndex(i + 1);
		}
	}

	for (int i = 0; i < typeBlocks.size(); i++) {
		POS type = typeBlocks.getKeyI(i);
		if (type == POS::Adjective) {
			if (i > 0) {
				int beginning = typeBlocks.getValueI(i - 1);
				int end = typeBlocks.getValueI(i);

				bool changed = false;

				POS newType = POS::Noun;
				for (int b = beginning; b <= end; b++) {
					if (wordStrings.at(b).find("ly") == std::string::npos) {
						newType = POS::Verb;

						if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Verb) {
							typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
							typeBlocks.deleteIndex(i);
							changed = true;
						}
						else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Verb) {
							typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
							typeBlocks.deleteIndex(i);
							changed = true;
						}

						break;
					}
				}

				if (changed == false) {
					if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Noun) {
						typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Noun) {
						typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
				}

				int iterations = 0;
				while (changed == false && iterations < 3) {
					if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Noun) {
						typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Noun) {
						typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Verb) {
						typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Verb) {
						typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
						typeBlocks.deleteIndex(i);
						changed = true;
					}
					
					iterations++;
				}
			}
		}
		else if (type == POS::Preposition) {
			if (i > 0 && typeBlocks.getKeyI(i - 1) == POS::Verb) {
				typeBlocks.changeValueI(i - 1, typeBlocks.getValueI(i));
				typeBlocks.deleteIndex(i);
			}
			else if (i + 1 < typeBlocks.size() && typeBlocks.getKeyI(i + 1) == POS::Verb) {
				typeBlocks.changeValueI(i + 1, typeBlocks.getValueI(i));
				typeBlocks.deleteIndex(i);
			}
		}
	}
	/*
	only if there is a verb before a noun and there is no article is a adjective part of a verb block (becomes adverb but don't want to add)
	*/
	//does it fall within noun block
	double nounBlock = 0;
	double verbBlock = 0;
	double prepositionBlock = 0;
	double adjectiveBlock = 0;

	//what block is in front
	double frontBlockNoun = 0;
	double frontBlockVerb = 0;
	double frontBlockConjunction = 0;
	double frontBlockPreposition = 0;
	double frontBlockAdjective = 0;

	//what block is in back
	double backBlockNoun = 0;
	double backBlockVerb = 0;
	double backBlockConjunction = 0;
	double backBlockPreposition = 0;
	double backBlockAdjective = 0;

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
				else if (typeBlocks.getKeyI(i) == POS::Verb) {
					verbBlock = 1;
				}
				else if (typeBlocks.getKeyI(i) == POS::Adjective) {
					adjectiveBlock = 1;
				}
				else if (typeBlocks.getKeyI(i) == POS::Preposition) {
					prepositionBlock = 1;
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
			else if (type == POS::Adjective) {
				frontBlockAdjective = 1;
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
			else if (type == POS::Adjective) {
				backBlockAdjective = 1;
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

		//CHECK FIND NGRAM
		Word w = master->findWord(wordStrings.at(targetWordIndex - 1));
		NGram<Word> lastWordNGram = master->findNGram(w);//
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

		total = usedNouns + usedVerbs + usedAdjectives + total;

		indicatesNoun = usedNouns / (total == 0? 1: total);
		indicatesVerb = usedVerbs / (total == 0 ? 1 : total);
		indicatesAdjective = usedAdjectives / (total == 0 ? 1 : total);
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
			for (int a = 0; a < master->ngramML.getValueI(i).size(); a++) {
				NGram<Word> currentNGram = master->ngramML.getValueI(i).at(a);

				if (currentNGram.content.containsV(afterWord)) {
					wordCall.add(currentNGram.content.getKeyV(afterWord), currentNGram.subject.type);
				}
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
		afterCallerNoun = nounUsed / (total == 0 ? 1 : total);
		afterCallerVerb = verbUsed / (total == 0 ? 1 : total);
		afterCallerAdjective = adjectiveUsed / (total == 0 ? 1 : total);
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

		if (total == 0) {
			nounPossible = 0;
			verbPossible = 0;
			adjectivePossible = 0;
		}
		else {
			nounPossible = nounOccerence / total;
			verbPossible = verbOccerence / total;
			adjectivePossible = adjectiveOccerence / total;
		}
	}

	//ENDING
	//the types of endings
	double nounEnding = 0;
	double adjectiveEnding = 0;
	double verbEnding = 0;

	{
		if ((wordStrings.at(targetWordIndex)).length() > 3) {
			string end = wordStrings.at(targetWordIndex).substr(wordStrings.at(targetWordIndex).length() - 1, 1);
			string end2 = "";
			string end3 = "";
			string end4 = "";

			if (wordStrings.at(targetWordIndex).length() > 4) {
				end2 = wordStrings.at(targetWordIndex).substr(wordStrings.at(targetWordIndex).length() - 2, 2);
				if (wordStrings.at(targetWordIndex).length() > 5) {
					end3 = wordStrings.at(targetWordIndex).substr(wordStrings.at(targetWordIndex).length() - 3, 3);

					if (wordStrings.at(targetWordIndex).length() > 6) {
						end4 = wordStrings.at(targetWordIndex).substr(wordStrings.at(targetWordIndex).length() - 4, 4);
					}
				}
			}

			if (end == "s" || end3 == "ion" || end3 == "acy" || end4 == "ence" || end4 == "ance" || end4 == "hood" || end2 == "ar" || end2 == "or"
				|| end3 == "ism" || end3 == "ist" || end4 == "ment" || end4 == "ness" || end == "y" || end3 == "ity") {
				nounEnding = 1;
			}

			if (end2 == "en" || end3 == "ify" || end3 == "ate" || end3 == "ize") {
				verbEnding = 1;
			}

			if (end2 == "al" || end3 == "ful" || end2 == "ly" || end2 == "ic" || end3 == "ish" || end4 == "like" || end3 == "ous" || end == "y"
				|| end3 == "ate" || end3 == "able" || end3 == "ible") {
				adjectiveEnding = 1;
			}
		}
	}

	//38 of them
	neuralNetworkInput.push_back(nounBlock);		//0
	neuralNetworkInput.push_back(verbBlock);
	neuralNetworkInput.push_back(adjectiveBlock);
	neuralNetworkInput.push_back(prepositionBlock);

	neuralNetworkInput.push_back(frontBlockNoun);
	neuralNetworkInput.push_back(frontBlockVerb);
	neuralNetworkInput.push_back(frontBlockAdjective);		//1
	neuralNetworkInput.push_back(frontBlockConjunction);
	neuralNetworkInput.push_back(frontBlockPreposition);

	neuralNetworkInput.push_back(backBlockNoun);
	neuralNetworkInput.push_back(backBlockVerb);
	neuralNetworkInput.push_back(backBlockAdjective);
	neuralNetworkInput.push_back(backBlockConjunction);		//1
	neuralNetworkInput.push_back(backBlockPreposition);

	neuralNetworkInput.push_back(hasNounBlock);		//0
	neuralNetworkInput.push_back(hasVerbBlock);

	neuralNetworkInput.push_back(lastWTypeNoun);
	neuralNetworkInput.push_back(lastWTypeVerb);
	neuralNetworkInput.push_back(lastWTypeAdjective);
	neuralNetworkInput.push_back(lastWTypeConjunction);		//1
	neuralNetworkInput.push_back(lastWTypePreposition);
	neuralNetworkInput.push_back(lastWTypeArticle);

	neuralNetworkInput.push_back(indicatesNoun);
	neuralNetworkInput.push_back(indicatesVerb);		//3
	neuralNetworkInput.push_back(indicatesAdjective);

	neuralNetworkInput.push_back(afterNoun);
	neuralNetworkInput.push_back(afterVerb);
	neuralNetworkInput.push_back(afterAdjective);		//1
	neuralNetworkInput.push_back(afterConjunction);
	neuralNetworkInput.push_back(afterPreposition);
	neuralNetworkInput.push_back(afterArticle);

	neuralNetworkInput.push_back(afterCallerNoun);
	neuralNetworkInput.push_back(afterCallerVerb);
	neuralNetworkInput.push_back(afterCallerAdjective);
	neuralNetworkInput.push_back(afterCallerConjunction);		//1
	neuralNetworkInput.push_back(afterCallerPreposition);
	neuralNetworkInput.push_back(afterCallerArticle); //*

	neuralNetworkInput.push_back(nounPossible); //*
	neuralNetworkInput.push_back(verbPossible); //*		//3
	neuralNetworkInput.push_back(adjectivePossible); //*

	neuralNetworkInput.push_back(nounEnding);
	neuralNetworkInput.push_back(verbEnding);		//0
	neuralNetworkInput.push_back(adjectiveEnding);

	double totalNodesUsed = 0;
	for (int i = 0; i < neuralNetworkInput.size(); i++) {
		if (neuralNetworkInput.at(i) != 0) {
			totalNodesUsed++;
		}
	}

	nn.process(neuralNetworkInput);

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
		else if (master->wordExistAll(wordStrings.at(i))) {
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
		else if (master->wordExistAll(wordStrings.at(i))) {
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

void PredictionEngine::mergeTesting(string phrase) {
	cout << "Phrase: " << phrase << endl;
	vector<string> wordStrings = breakDownV(phrase);
	vector<POS> wtype;

	for (int i = 0; i < wordStrings.size(); i++) {
		wtype.push_back(master->findWord(wordStrings.at(i)).type);
	}

	for (int i = 0; i < wordStrings.size(); i++) {
		if (wtype.at(i) != POS::Verb && wtype.at(i) != POS::Adjective && wtype.at(i) != POS::Noun) {
			continue;
		}

		findTypeDeployment(wtype, phrase, i);

		double one = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(0).value;
		double two = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(1).value;
		double three = nn.neuralNetwork.at(nn.neuralNetwork.size() - 1).at(2).value;

		POS type;

		if (one >= two && one >= three ) {
			type = POS::Noun;
		}
		else if (two > one && two > three) {
			type = POS::Verb;
		}
		else if (three > one && three > two) {
			type = POS::Adjective;
		}

		cout << wordStrings.at(i) << ": " << POStoString(type) << " Actual: " << POStoString(wtype.at(i)) << endl;
	}
}
