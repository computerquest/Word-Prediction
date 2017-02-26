#include "MasterResource.h"
#include "PredictionEngine.h"

//UNDO CHANGES
MasterResource::MasterResource() {
	//readNGram();
	//readProbationSS();
	//readProbationWord();
	//readStruct();
	std::cout << "new mr" << endl;
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
		if (line != "") {
			thisLine.push_back(line);
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
	LinkedList<int, vector<string>> fileInput;
	for (int i = 0; i < ngramML.size(); i++) {
		vector<string> tempVector;
		for (int a = 0; a < ngramML.getValueI(i).size(); a++) {
			NGram<Word> currentNGram = ngramML.getValueI(i).at(a);
			string allContent = "";
			string subject = currentNGram.subject.name;

			allContent += subject + "," + POStoString(currentNGram.subject.type) + "|";
			for (int a = 0; a < currentNGram.content.size(); a++) {
				allContent += to_string(currentNGram.content.getKeyI(a)) + "," + currentNGram.content.getValueI(a).name + "," + POStoString(currentNGram.content.getValueI(a).type) + "|";
			}

			tempVector.push_back(allContent);
		}

		fileInput.add(ngramML.getKeyI(i), tempVector);
	}


	for (int i = 0; i < fileInput.size(); i++) {
		ofstream stream;
		stream.open(to_string(fileInput.getKeyI(i)) + ".txt", ios::trunc);

		for (int a = 0; a < fileInput.getValueI(i).size(); a++) {
			stream << fileInput.getValueI(i).at(a) << endl;
		}

		stream.close();
	}
}

void MasterResource::readNGram() {
	vector<char> delimeter;
	delimeter.push_back('|');

	vector<vector<string>> input = fileInput("NGrams.txt", delimeter);

	std::cout << "creating words" << endl;

	LinkedList<int, LinkedList<string, string>> data;
	for (int i = 0; i < input.size(); i++) {
		cout << i << endl;
		POS type;
		POS type1;

		if ((type = decypherType(input.at(i).at(3))) == POS::Unknown | (type1 = decypherType(input.at(i).at(4))) == POS::Unknown) {
			continue;
		}

		string name = input.at(i).at(1);// +";" + POStoString(type) + "|";

		int value = 0;
		for (int a = 0; a < name.length(); a++) {
			value += name[a];
		}

		if (data.containsK(value)) {
			int valueIndex = data.getPostionK(value);

			if (data.getValueI(valueIndex).containsK(name)) {
				int nameIndex = data.getValueI(valueIndex).getPostionK(name);
				LinkedList<string, string> dataValue = data.getValueI(valueIndex);

				string newInfo = input.at(i).at(0) + "," + input.at(i).at(2) + "," + POStoString(type1) + "|";
				dataValue.changeValueI(nameIndex, data.getValueI(valueIndex).getValueI(nameIndex) + newInfo);

				data.changeValueI(valueIndex, dataValue);
			}
			else {
				string newInfo = name + "|" + POStoString(type) + "|" + input.at(i).at(0) + "," + input.at(i).at(2) + "," + POStoString(type1) + "|";
				LinkedList<string, string> dataValue = data.getValueI(valueIndex);

				dataValue.add(name, newInfo);
				data.changeValueI(valueIndex, dataValue);
			}
		}
		else {
			string newInfo = name + "|" + POStoString(type) + "|" + input.at(i).at(0) + "," + input.at(i).at(2) + "," + POStoString(type1) + "|";
			LinkedList<string, string> newLL;

			newLL.add(name, newInfo);
			data.add(value, newLL);
		}
	}

	cout << "writing" << endl;

	for (int i = 0; i < data.size(); i++) {
		ofstream stream;
		stream.open(to_string(data.getKeyI(i)) + ".txt", ios::trunc);

		for (int a = 0; a < data.getValueI(i).size(); a++) {
			stream << data.getValueI(i).getValueI(a) << endl;
		}

		stream.close();
	}
}

NGram<Word> MasterResource::findInFile(string search, int file) {
	vector<char> delimeter;
	delimeter.push_back('|');
	vector<vector<string>> input = fileInput(to_string(file) + ".txt", delimeter);

	for (int i = 0; i < input.size(); i++) {
		if (input.at(i).at(0) == search) {
			vector<string> currentVector = input.at(i);
			Word tempWord(currentVector.at(0), toType(currentVector.at(1)));
			NGram<Word> newNGram(tempWord);

			delimeter[0] = ',';

			for (int a = 2; a < currentVector.size(); a++) {
				vector<string> secondaryVector = breakDownV(currentVector.at(a), delimeter);

				for (int b = 0; b < secondaryVector.size(); b++) {
					Word secondaryTempWord(secondaryVector.at(1), toType(secondaryVector.at(2)));

					newNGram.content.add(stoi(secondaryVector.at(0)), secondaryTempWord);
				}
			}

			if (ngramML.containsK(file)) {
				ngramML.getValueK(file).push_back(newNGram);
			}
			else {
				vector<NGram<Word>> temp;
				temp.push_back(newNGram);
				ngramML.add(file, temp);
			}

			return newNGram;
		}
	}

	NGram<Word> defaultNG;
	Word defaultWord("not found", POS::Unknown);
	defaultNG.subject = defaultWord;
	return defaultNG;
}
POS MasterResource::decypherType(string secondType) {
	if (secondType.find("np") != std::string::npos | secondType.find("nn") != std::string::npos | secondType == "appge" |
		secondType.find("pn") != std::string::npos | secondType.find("pp") != std::string::npos |
		secondType.find("nd") != std::string::npos | secondType.find("z") != std::string::npos) {
		return POS::Noun;
	}
	else if (secondType.find("at") != std::string::npos) {
		return POS::Article;
	}
	else if (secondType.find("cs") != std::string::npos | secondType.find("cc") != std::string::npos | secondType == "bcl") {
		return POS::Conjuction;
	}
	else if (secondType.find("da") != std::string::npos | secondType == "to" | secondType.find("i") != std::string::npos |
		secondType.find("dd") != std::string::npos | secondType.find("db") != std::string::npos) {
		return POS::Preposition;
	}

	else if (secondType.find("j") != std::string::npos |
		secondType.find("r") != std::string::npos & secondType.find("b") == std::string::npos | secondType.find("x") != std::string::npos) {
		return POS::Adjective;
	}
	else if (secondType.find("v") != std::string::npos) {
		return POS::Verb;
	}
	else
	{
		return POS::Unknown;
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

bool MasterResource::wordExistAll(string word) {
	if (!wordExist(word)) {
		int value = 0;
		for (int i = 0; i < word.length(); i++) {
			value += word[i];
		}

		return findInFile(word, value).subject.type != POS::Unknown ? true : false;
	}

	return wordExist(word);
}
////////////////////////////////////////////////////////////////////////////////FIND

//INCOMPLETE
vector<POS> MasterResource::findAllWordType(string input) {
	vector<string> wordS = breakDownV(input);
	vector<POS> answer;

	for (int i = 0; i < wordS.size(); i++) {
		string currentWord = wordS.at(i);
		if (wordExist(wordS.at(i))) {
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
Word MasterResource::findWord(string word)
{
	int wordSum = stringToInt(word);
	if (!wordExist(word)) {
		return findInFile(word, wordSum).subject;
	}

	int kPos = ngramML.getPostionK(wordSum);
	for (int i = 0; i < ngramML.getValueI(kPos).size(); i++) {
		if (ngramML.getValueI(kPos).at(i).subject.name == word) {
			Word temp = ngramML.getValueI(kPos).at(i).subject;
			return temp;
		}
	}

	Word defaultWord("not found", POS::Unknown);
	return defaultWord;
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
SStructure* MasterResource::findStructurePercisionP(vector<POS> order) {
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
				return &sNGramML.at(i).subject;
			}
		}
	}
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
}//

////////////////////////////////////////////probation
NGram<Word> MasterResource::findNGram(Word word) {
	return *findNGramP(word);
}

NGram<Word>* MasterResource::findNGramP(Word word) {
	int value = stringToInt(word.name);
	int kPos = ngramML.getPostionK(value);

	if (!ngramExist(word)) {
		findInFile(word.name, value);
		return &ngramML.getValueI(kPos).at(ngramML.getValueI(kPos).size() - 1);
	}

	for (int i = 0; i < ngramML.getValueI(kPos).size(); i++) {
		if (ngramML.getValueI(kPos).at(i).subject.name == word.name) {
			return &ngramML.getValueI(kPos).at(i);
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
	int value = stringToInt(word);
	if (ngramML.containsK(value)) {
		int pos = ngramML.getPostionK(value);
		for (int i = 0; i < ngramML.getValueI(pos).size(); i++) {
			if (ngramML.getValueI(pos).at(i).subject.name == word) {
				return true;
			}
		}
	}
	return false;
}

bool MasterResource::ngramExist(Word word) {
	return wordExist(word.name);
}

bool MasterResource::ngramExist(string word) {
	return wordExist(word);
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

					int value = stringToInt(newNGram.subject.name);
					if (ngramML.containsK(value)) {
						ngramML.getValueK(value).push_back(newNGram);
					}
					else {
						vector<NGram<Word>> temp;
						temp.push_back(newNGram);

						ngramML.add(value, temp);
					}
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
	int value = stringToInt(word);
	bool count = false;
	int kPos = ngramML.getPostionK(value);
	for (int i = 0; i < ngramML.getValueI(value).size(); i++) {
		if (word == ngramML.getValueI(kPos).at(i).subject.name && count == false) {
			count = true;
		}
		else if (count == true && word == ngramML.getValueI(kPos).at(i).subject.name) {
			return true;
		}
	}

	return false;
}
vector<POS> MasterResource::doublePossibilities(string word) {
	vector<POS> answer;
	int value = stringToInt(word);
	int kPos = ngramML.getPostionK(value);
	for (int i = 0; i < ngramML.getValueI(value).size(); i++) {
		if (word == ngramML.getValueI(kPos).at(i).subject.name) {
			answer.push_back(ngramML.getValueI(value).at(i).subject.type);
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
			std::cout << POStoString(type) << endl;
			return possible.at(i);
		}
	}
}