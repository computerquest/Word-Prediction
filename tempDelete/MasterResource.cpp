#pragma once
#include "MasterResource.h"
#include "PredictionEngine.h"

MasterResource::MasterResource() {
	readNGram();
	readProbationSS();
	readProbationWord();
	readStruct();
	cout << "new mr" << endl;
}

///////////////////////////////////////////////////////////////////////////FILE
vector<vector<string>> MasterResource::fileInput(string fileName, vector<char> delimeter) {
	ifstream stream;
	stream.open(fileName);

	vector<vector<string>> answer;

	string line;
	while (std::getline(stream, line)) {
		vector<string> thisLine;
		for (int a = 0; a < line.length(); a++) {
			for (int i = 0; i < delimeter.size(); i++) {
				if (line[a] == delimeter.at(i)) {
					thisLine.push_back(line.substr(0, a));
					line = line.substr(a + 1);
					a = 0;
					if (line == "") {
						break;
					}
				}
			}
		}
		answer.push_back(thisLine);
	}

	return answer;
}
vector<vector<string>> MasterResource::fileInput(string fileName, vector<char> delimeter, char stopChar) {
	ifstream stream;
	stream.open(fileName);

	vector<vector<string>> answer;

	string line;
	while (std::getline(stream, line)) {
		vector<string> thisLine;
		int a = 0;
		while (a < line.length()) {
			bool activate = false;
			if (line[a] != stopChar) {
				for (int i = 0; i < delimeter.size(); i++) {
					if (line[a] == delimeter.at(i)) {
						thisLine.push_back(line.substr(0, a));
						line = line.substr(a + 1);

						a = 0;
						activate = true;

						break;
					}
				}
			}
			else {
				break;
			}

			if (activate == true) {
				continue;
			}
			a++;
		}
		answer.push_back(thisLine);
	}

	return answer;
}

//////////////////////////////////////////structure
void MasterResource::saveStruct() {
	ofstream stream;
	stream.open("sNGram.txt", ios::trunc);
	//noun|verb|adjective|-8,noun|article|preposition|conjunction|verb|
	for (int i = 0; i < sNGramML.size(); i++) {
		vector<POS> current = sNGramML.at(i).subject.component;

		for (int a = 0; a < current.size(); a++) {
			stream << POStoString(current.at(a)) << "|";
		}

		if (sNGramML.at(i).content.size() > -1) {
			LinkedList<int, SStructure> currentContent = sNGramML.at(i).content;
			for (int b = 0; b < currentContent.size(); b++) {
				stream << "-" << std::to_string(currentContent.getKeyI(b)) << ",";

				vector<POS> sCurrentContent = currentContent.getValueI(b).component;

				for (int a = 0; a < sCurrentContent.size(); a++) {
					stream << POStoString(sCurrentContent.at(a)) << "|";
				}
			}
		}
		stream << endl;
	}
	stream.close();
}
void MasterResource::readStruct() {
	//article|noun|verb|preposition|article|noun|-6,noun|conjunction|noun|verb|adjective|noun|-8,noun|verb|preposition|article|noun|conjunction|verb|
	//make all of the structures
	{
		vector<char> delimeters;
		delimeters.push_back('|');
		delimeters.push_back(',');
		vector<vector<string>> partialInput = fileInput("sNGram.txt", delimeters, '-');

		for (int i = 0; i < partialInput.size(); i++) {
			vector<string> line = partialInput.at(i);

			NGram<SStructure> newNGram;
			SStructure structure;

			for (int b = 0; b < line.size(); b++) {
				structure.addItem(toType(line.at(b)));
			}

			newNGram.subject = structure;

			sNGramML.push_back(newNGram);
		}
	}

	{
		vector<char> delimeters;
		delimeters.push_back('|');
		delimeters.push_back(',');
		vector<vector<string>> partialInput = fileInput("sNGram.txt", delimeters);

		for (int i = 0; i < partialInput.size(); i++) {
			vector<string> line = partialInput.at(i);

			NGram<SStructure>* current = &sNGramML.at(i);

			LinkedList<int, SStructure> content;
			int firstIndex = -1;
			int secondIndex = line.size() - 1;
			for (int a = 0; a < line.size(); a++) {
				int index = line.at(a).find("-");
				if (index > -1) {
					if (firstIndex == -1) {
						firstIndex = a;
					}
					else {
						secondIndex = a;

						vector<POS> temp;
						for (int c = firstIndex + 1; c < secondIndex; c++) {
							temp.push_back(toType(line.at(c)));
						}


						content.add(abs(stoi(line.at(firstIndex))), findStructurePercision(temp));
						firstIndex = secondIndex;
						secondIndex = line.size() - 1;

					}
				}
			}

			if (firstIndex != -1) {
				vector<POS> temp;
				for (int c = firstIndex + 1; c <= secondIndex; c++) {
					temp.push_back(toType(line.at(c)));
				}

				content.add(abs(stoi(line.at(firstIndex))), findStructurePercision(temp));
			}
			current->content = content;
		}
	}
}

/////////////////////////////////////////ngram
void MasterResource::saveNGram() {
	ofstream stream;
	stream.open("NGrams.txt", ios::trunc);

	for (int i = 0; i < ngramML.size(); i++) {
		LinkedList<int, Word> content = ngramML.at(i).content;

		stream << ngramML.at(i).subject.name << "," << POStoString(ngramML.at(i).subject.type);

		for (int a = 0; a < content.size(); a++) {
			stream << "|" << content.getKeyI(a) << "," << content.getValueI(a).name;
		}

		stream << "|" << endl;
	}

	stream.close();
}
void MasterResource::readNGram() {
	vector<char> delimeter;
	delimeter.push_back(',');
	delimeter.push_back('|');
	delimeter.push_back(';');

	vector<vector<string>> input = fileInput("NGrams.txt", delimeter);

	//creates all the words
	for (int i = 0; i < input.size(); i++) {
		Word newWord(input.at(i).at(0), toType(input.at(i).at(1)));

		NGram<Word> newNGram(newWord);
		ngramML.push_back(newNGram);
	}

	//does all the ngram
	for (int i = 0; i < input.size(); i++) {
		vector<string> current = input.at(i);

		Word currentWord = findWord(current.at(0));

		LinkedList<int, Word> currentContent;

		for (int a = 2; a < current.size() - 1; a += 2) {
			currentContent.add(std::stoi(current.at(a)), findWord(current.at(a + 1)));
		}

		//content not being set
		NGram<Word>* ngram = findNGramP(currentWord);
		ngram->content = currentContent;
	}
}

/////////////////////////////////////////probation
void MasterResource::writeProbationWord() {
	ofstream stream;
	stream.open("probationWord.txt", ios::trunc);

	for (int i = 0; i < probationWord.size(); i++) {
		stream << probationWord.getKeyI(i) << "," << probationWord.getValueI(i).name << "," << POStoString(probationWord.getValueI(i).type) << ";";
	}
}
void MasterResource::writeProbationSS() {
	ofstream stream;
	stream.open("probationSS.txt", ios::trunc);
	//9|noun|verb|adjective|
	for (int i = 0; i < probationSS.size(); i++) {
		vector<POS> current = probationSS.getValueI(i).component;

		stream << std::to_string(probationSS.getKeyI(i)) << "|";

		for (int a = 0; a < current.size(); a++) {
			stream << POStoString(current.at(a)) << "|";
		}

		stream << endl;
	}
}
void MasterResource::readProbationWord() {
	vector<char> delimeter;
	delimeter.push_back(',');
	delimeter.push_back(';');

	vector<vector<string>> input = fileInput("probationWord.txt", delimeter);

	//creates all the words
	for (int i = 0; i < input.size(); i++) {
		vector<string> currentInput = input.at(i);

		Word newWord(currentInput.at(1), toType(currentInput.at(2)));
		probationWord.add(std::stoi(currentInput.at(0)), newWord);
	}
}
void MasterResource::readProbationSS() {
	vector<char> delimeter;
	delimeter.push_back('|');

	vector<vector<string>> input = fileInput("probationSS.txt", delimeter);

	for (int i = 0; i < input.size(); i++) {
		vector<string> currentInput = input.at(i);

		vector<POS> structureComponent;
		for (int a = 1; a < currentInput.size(); a++) {
			structureComponent.push_back(toType(currentInput.at(a)));
		}

		SStructure newStructure(structureComponent);
		probationSS.add(std::stoi(currentInput.at(0)), newStructure);
	}
}

////////////////////////////////////////////////////////////////////////////////FIND
vector<POS> MasterResource::findAllWordType(string input) {
	vector<string> wordS = breakDownV(input);
	vector<POS> answer;

	for (int i = 0; i < wordS.size(); i++) {
		string currentWord = wordS.at(i);
		if (wordExist(wordS.at(i))) {
			/*if (isDouble(wordS.at(i)) == true) {
			answer.push_back(findDouble(input, wordS.at(i)).type);
			}
			else {
			answer.push_back(findWord(currentWord).type);
			}*/
		}
		else if (probationExistWord(wordS.at(i))) {
			answer.push_back(findProbationWord(currentWord).type);
		}
		else {
			answer.push_back(POS::Unknown);
		}
	}

	return answer;
}
Word MasterResource::findWord(string word) {
	for (int i = 0; i < ngramML.size(); i++) {
		if (ngramML.at(i).subject.name == word) {
			return ngramML.at(i).subject;
		}
	}
}

////////////////////////////////////////////////structure
vector<SStructure> MasterResource::matchPossibleSS(vector<POS> input) {
	vector<SStructure> match;
	for (int i = 0; i < sNGramML.size(); i++) {
		vector<POS> components = sNGramML.at(i).subject.component;

		if (components.size() >= input.size()) {
			bool answer = true;
			for (int a = 0; a < input.size(); a++) {
				if (input.at(a) != components.at(a) && input.at(a) != POS::Unknown) {
					answer = false;
					break;
				}
			}

			if (answer == true) {
				match.push_back(sNGramML.at(i).subject);
			}
		}
		else {
			continue;
		}
	}

	return match;
}
SStructure MasterResource::findStructurePercision(vector<POS> order) {
	for (int i = 0; i < sNGramML.size(); i++) {
		vector<POS> current = sNGramML.at(i).subject.component;
		bool solution = true;
		if (current.size() == order.size()) {
			for (int a = 0; a < order.size(); a++) {
				if (current.at(a) != order.at(a)) {
					solution = false;
					break;
				}
			}
			if (solution == true) {
				return sNGramML.at(i).subject;
			}
		}
	}
}
SStructure MasterResource::findPartailStructure(vector<POS> order, int missing) {
	for (int i = 0; i < sNGramML.size(); i++) {
		vector<POS> current = sNGramML.at(i).subject.component;

		if (current.size() == order.size()) {
			bool solution = true;
			for (int a = 0; a < order.size(); a++) {
				if (a == missing) {
					continue;
				}
				if (current.at(a) != order.at(a)) {
					solution = false;
					break;
				}
			}

			if (solution == true) {
				return sNGramML.at(i).subject;
			}
		}
	}
}
SStructure MasterResource::findPartailStructure(vector<POS> order) {
	for (int i = 0; i < sNGramML.size(); i++) {
		vector<POS> current = sNGramML.at(i).subject.component;
		bool correct = true;
		if (current.size() == order.size()) {
			for (int a = 0; a < current.size(); a++) {
				if (order.at(a) == POS::Unknown) {
					continue;
				}
				else if (current.at(a) != order.at(a)) {
					correct = false;
					break;
				}
			}

			if (correct == true) {
				return current;
			}
		}
	}

	SStructure defaultStruct;
	defaultStruct.component.push_back(POS::Unknown);

	return defaultStruct;
}

/////////////////////////////////////ngram
NGram<SStructure>* MasterResource::findNGramSS(vector<POS> order) {
	for (int i = 0; i < sNGramML.size(); i++) {
		vector<POS> current = sNGramML.at(i).subject.component;

		if (current.size() == order.size()) {
			bool correct = true;
			for (int b = 0; b < current.size(); b++) {
				if (current.at(b) != order.at(b)) {
					correct = false;
					break;
				}
			}

			if (correct == true) {
				return &sNGramML.at(i);
			}
		}
	}
}
NGram<SStructure> MasterResource::findNGramS(vector<POS> parts) {
	for (int i = 0; i < sNGramML.size(); i++) {
		if (sNGramML.at(i).subject.component.size() == parts.size()) {
			if (parts == sNGramML.at(i).subject.component) {
				return sNGramML.at(i);
			}
		}
	}
}

////////////////////////////////////////////probation
NGram<Word> MasterResource::findNGram(Word word) {
	for (int i = 0; i < ngramML.size(); i++) {
		if (ngramML.at(i).subject == word) {
			return ngramML.at(i);
		}
	}
}
NGram<Word>* MasterResource::findNGramP(Word word) {
	for (int i = 0; i < ngramML.size(); i++) {
		if (ngramML.at(i).subject == word) {
			return &ngramML.at(i);
		}
	}
}
Word MasterResource::findProbationWord(string word) {
	for (int i = 0; i < probationWord.size(); i++) {
		if (probationWord.getValueI(i).name == word) {
			return probationWord.getValueI(i);
		}
	}
}

////////////////////////////////////////////////////////////////EXIST
bool MasterResource::wordExist(string word) {
	for (int i = 0; i < ngramML.size(); i++) {
		if (ngramML.at(i).subject.name == word) {
			return true;
		}
	}
	return false;
}

bool MasterResource::ngramExist(Word word) {
	for (int i = 0; i < ngramML.size(); i++) {
		if (ngramML.at(i).subject == word) {
			return true;
		}
	}
	return false;
}

bool MasterResource::sstructureExist(vector<POS> order) {
	for (int i = 0; i < sNGramML.size(); i++) {
		if (sNGramML.at(i).subject.component == order) {
			return true;
		}
	}

	return false;
} 

////////////////////////////////////probation
bool MasterResource::probationExistWord(string word) {
	for (int i = 0; i < probationWord.size(); i++) {
		if (probationWord.getValueI(i).name == word) {
			return true;
		}
	}

	return false;
}
bool MasterResource::probationExistSS(vector<POS> input) {
	for (int i = 0; i < probationSS.size(); i++) {
		if (probationSS.getValueI(i).component == input) {
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////MISC
void MasterResource::findNew(string phrase) {
	vector<string> words = breakDownV(phrase);
	vector<POS> newFoundPOS = engine->multipleMissing(phrase);
	int newFoundPOSI = 0;

	for (int i = 0; i < words.size(); i++) {
		if (!wordExist(words.at(i))) {
			Word newWord(words.at(i), newFoundPOS.at(newFoundPOSI));
			newFoundPOSI++;

			if (probationExistWord(newWord.name)) {
				probationWord.changeKeyI(probationWord.getPostionV(newWord), probationWord.getKeyV(newWord) + 1);

				if (probationWord.getKeyI(probationWord.getPostionV(newWord)) >= 5) {
					NGram<Word> newNGram;
					newNGram.subject = newWord;

					ngramML.push_back(newNGram);
					probationWord.deleteIndex(probationWord.getPostionV(newWord));
				}
			}
			else {
				probationWord.add(1, newWord);
			}
		}
	}

};
void MasterResource::updateNGram(Word input, int newOccerence) {
	NGram<Word> * ngram = findNGramP(input);

	ngram->updateItem(input, newOccerence);
}

//////////////////////////////////////////////////////////////////////////DOUBLE
bool MasterResource::isDouble(string word) {
	bool count = false;
	for (int i = 0; i < ngramML.size(); i++) {
		if (word == ngramML.at(i).subject.name && count == false) {
			count = true;
		}
		else if (count == true && word == ngramML.at(i).subject.name) {
			return true;
		}
	}

	return false;
}
vector<POS> MasterResource::doublePossibilities(string word) {
	vector<POS> answer;
	for (int i = 0; i < ngramML.size(); i++) {
		if (ngramML.at(i).subject.name == word) {
			answer.push_back(ngramML.at(i).subject.type);
		}
	}

	return answer;
}
POS MasterResource::findDouble(string phrase, string target) {
	vector<POS> possible = doublePossibilities(target);

	LinkedList<POS, double> values;
	for (int i = 0; i < possible.size(); i++) {
		//engine->chooseDouble(phrase, target);

		//values.add(possible.at(i).type, engine->nn.neuralNetwork.at(engine->nn.neuralNetwork.size() - 1).at(possible.at(i).type).value);
	}

	double greatest = 0;
	POS type;
	for (int i = 0; i < values.size(); i++) {
		if (values.getValueI(i) > greatest) {
			type = values.getKeyI(i);
			greatest = values.getValueI(i);
		}
	}

	for (int i = 0; i < possible.size(); i++) {
		if (possible.at(i) == type) {
			cout << POStoString(type) << endl;
			return possible.at(i);
		}
	}
}