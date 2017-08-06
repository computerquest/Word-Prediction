#include "MasterResource.h"
#include "PredictionEngine.h"
using namespace std;

//todo make sure can return doubles based on string
//todo upgrade file system?
//todo UNDO CHANGES
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
	stream.open("C:/Workspace/New folder/tempDelete/tempDelete/"+fileName);

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
	stream.open("C:/Workspace/New folder/tempDelete/tempDelete/"+fileName);

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
	for (auto s : sNGramML) {
		for (int i = 0; i < s.second.size(); i++) {
			vector<POS> current = s.second.at(i).subject.component;

			for (int a = 0; a < current.size(); a++) {
				stream << POStoString(current.at(a)) << "|";
			}

			for (auto it : s.second.at(i).content) {
				vector<SStructure>& currentContent = it.second;
				for (int b = 0; b < currentContent.size(); b++) {
					stream << "-" << std::to_string(it.first) << ",";

					vector<POS> sCurrentContent = currentContent.at(b).component;

					for (int a = 0; a < sCurrentContent.size(); a++) {
						stream << POStoString(sCurrentContent.at(a)) << "|";
					}
				}
			}
		}
		stream << endl;

	}
	stream.close();
}

void MasterResource::readStruct() {
	vector<NGram<SStructure>> temp;
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

			temp.push_back(newNGram);
		}
	}

	{
		vector<char> delimeters;
		delimeters.push_back('|');
		delimeters.push_back(',');
		vector<vector<string>> partialInput = fileInput("sNGram.txt", delimeters);

		for (int i = 0; i < partialInput.size(); i++) {
			vector<string> line = partialInput.at(i);

			NGram<SStructure>* current = &(temp.at(i));

			map<int, vector<SStructure>> content;
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


						content[abs(stoi(line.at(firstIndex)))].push_back(findStructurePercision(temp));
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

				content[abs(stoi(line.at(firstIndex)))].push_back(findStructurePercision(temp));
			}
			current->content = content;
		}
	}

	for (NGram<SStructure> t : temp) {
		sNGramML[t.content.size()].push_back(t);
	}
}

/////////////////////////////////////////ngram
void MasterResource::saveNGram() {
	LinkedList<int, vector<string>> fileInput;
	for (auto &a : ngramML) {
		vector<string> tempVector;

		for (int b = 0; b < a.second.size(); b++) {
			NGram<Word> currentNGram = a.second[b];
			string allContent = "";
			string subject = currentNGram.subject.name;

			allContent += subject + "," + POStoString(currentNGram.subject.type) + "|";
			for (auto it : currentNGram.content) {
				int k = it.first;
				for (int i = 0; i < it.second.size(); i++) {
					allContent += to_string(k) + "," + it.second.at(i).name + "," + POStoString(it.second.at(i).type) + "|";
				}
			}
			tempVector.push_back(allContent);
		}

		fileInput.add(a.first, tempVector);
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

	//ex: 24|a|bona-fide|at1|jj
	vector<vector<string>> input = fileInput("NGrams.txt", delimeter);

	std::cout << "creating words" << endl;

	map<int, LinkedList<string, string>> data;
	for (int i = 0; i < input.size(); i++) {
		std::cout << i << endl;
		string type;
		string type1;

		if ((type = decypherType(input.at(i).at(3))) == "unknown" | (type1 = decypherType(input.at(i).at(4))) == "unknown") {
			continue;
		}

		string name = input.at(i).at(1);// +";" + POStoString(type) + "|";

		string search = input[i].at(1) + "*" + type;

		int value = stringToInt(name);

		map<int, LinkedList<string, string>>::iterator it;
		//if we have that number already
		if ((it = data.find(value)) != data.end()) {

			//if we already have that
			if (it->second.containsK(search)) {
				LinkedList<string, string>& dataValue = it->second;
				int nameIndex = dataValue.getPostionK(search);

				string newInfo = input.at(i).at(0) + "," + input.at(i).at(2) + "," + type + "|";
				dataValue.changeValueI(nameIndex, it->second.getValueI(nameIndex) + newInfo);
			}
			else {
				string newInfo = name + "|" + type + "|" + input.at(i).at(0) + "," + input.at(i).at(2) + "," + type + "|";
				LinkedList<string, string>& dataValue = it->second;

				dataValue.add(search, newInfo);
			}
		}
		else {
			string newInfo = name + "|" + type + "|" + input.at(i).at(0) + "," + input.at(i).at(2) + "," + type1 + "|";
			LinkedList<string, string> newLL;

			newLL.add(search, newInfo);

			data[value] = newLL;
		}
	}

	std::cout << "writing" << endl;


	for (auto &it : data) {
		ofstream stream;
		stream.open(to_string(it.first) + ".txt", ios::trunc);
		for (int i = 0; i < it.second.size(); i++) {
			stream << it.second.getValueI(i) << endl;
		}
	}
	//writes this all back to files 
	/*for (int i = 0; i < data.size(); i++) {
		ofstream stream;
		stream.open(to_string(data.getKeyI(i)) + ".txt", ios::trunc);

		for (int a = 0; a < data.getValueI(i).size(); a++) {
			stream << data.getValueI(i).getValueI(a) << endl;
		}

		stream.close();
	}*/
}

vector<NGram<Word>> MasterResource::findInFile(string search, int file) {
	vector<char> delimeter;
	delimeter.push_back('|');
	vector<vector<string>> input = fileInput(to_string(file) + ".txt", delimeter);
	vector<NGram<Word>> answer;

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

					//newNGram.updateSafe(secondaryTempWord, stoi(secondaryVector.at(0)));
					newNGram.addItem(secondaryTempWord, stoi(secondaryVector.at(0)));
				}
			}

			map<int, vector<NGram<Word>>>::iterator it;
			if ((it = ngramML.find(file)) != ngramML.end()) {
				it->second.push_back(newNGram);
			}
			else {
				vector<NGram<Word>> temp;
				temp.push_back(newNGram);
				ngramML[file] = temp;
			}

			answer.push_back(newNGram);
		}
		else if (answer.size() > 0 && input.at(i).at(0) != search) {
			return answer;
		}
	}

	NGram<Word> defaultNG;
	Word defaultWord("not found", POS::Unknown);
	defaultNG.subject = defaultWord;
	answer.push_back(defaultNG);
	return answer;
}
string MasterResource::decypherType(string secondType) {
	if (secondType == "appge" || secondType.find("db") != string::npos || secondType.find("dd") != string::npos || secondType == "ex" || secondType.find("nn") || secondType.find("np") != string::npos || secondType.find("np") != string::npos || secondType.find("pn") != string::npos || secondType.find("pp") != string::npos || secondType == "zz2") {
		return "noun";
	}
	else if (secondType.find("at") != std::string::npos) {
		return "article";
	}
	else if (secondType.find("cs") != std::string::npos || secondType.find("cc") != std::string::npos || secondType == "bcl") {
		return "conjunction";
	}
	else if (secondType == "if" || secondType == "ii" || secondType == "io" || secondType == "iw" || secondType == "to" || secondType == "vb0" || secondType == "vbdz" || secondType == "vbg" || secondType == "vbi" || secondType == "rp" || secondType == "rpk") {
		return "preposition";
	}

	else if (secondType.find("jj") != std::string::npos || secondType.find("mc") != string::npos) {
		return "adjective";
	}
	else if (secondType == "ra" || secondType.find("da") != string::npos || secondType == "rex" || secondType == "rg" || secondType == "rgqv" || secondType == "rgr" || secondType == "rgt" || secondType == "rr" || secondType == "rrr" || secondType == "rrt" || secondType == "rrqv" || secondType == "xx") {
		return "adverb";
	}
	else if (secondType.find("vv") != std::string::npos || secondType.find("vm") || secondType == "vd" || secondType.find("vh") != string::npos) {
		string ext = "";
		if (secondType == "vv0") {
			ext = "/pres";
		}
		else if (secondType == "vvd") {
			ext = "/pas";
		}
		else if (secondType.find("vvg") != string::npos) {
			ext = "/do";
		}
		else if (secondType.find("vvz") != string::npos) {
			ext = "/theo";
		}
		return "verb" + ext;
	}
	else if (secondType.find("vb") != std::string::npos || secondType.find("ddq") != std::string::npos || secondType == "rgq" || secondType == "rrq" || secondType == "uh" || secondType == "vbdr") {
		return "unique";
	}
	else {
		return "unknown";
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

/*bool MasterResource::wordExistAll(string word) {
	if (!wordExist(word)) {
		int value = 0;
		for (int i = 0; i < word.length(); i++) {
			value += word[i];
		}

		return findInFile(word, value).subject.type != POS::Unknown ? true : false;
	}

	return wordExist(word);
}*/
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
			//answer.push_back(findProbationWord(currentWord).type);
		}
		else {
			answer.push_back(POS::Unknown);
		}
	}

	return answer;
}

vector<Word> MasterResource::findWord(string word)
{
	vector<Word> answer;
	vector<NGram<Word>*> possible = findNGramP(word);
	for (int i = 0; i < possible.size(); i++) {
		answer.push_back(possible.at(i)->subject);
	}

	return answer;
}

////////////////////////////////////////////////structure
vector<SStructure> MasterResource::matchPossibleSS(vector<POS> input) {
	vector<SStructure> match;
	for (auto s : sNGramML) {
		for (int i = 0; i < s.second.size(); i++) {
			vector<POS> components = s.second.at(i).subject.component;

			if (components.size() >= input.size()) {
				bool answer = true;
				for (int a = 0; a < input.size(); a++) {
					if (input.at(a) != components.at(a) && input.at(a) != POS::Unknown) {
						answer = false;
						break;
					}
				}

				if (answer == true) {
					match.push_back(s.second.at(i).subject);
				}
			}
			else {
				continue;
			}
		}
	}
	return match;
}
vector<SStructure> MasterResource::findPartailStructure(vector<POS> order, int missing) {
	vector<SStructure> ans;
	vector<NGram<SStructure>> s = sNGramML[order.size()];
	for (int i = 0; i < s.size(); i++) {
		vector<POS> current = s.at(i).subject.component;

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
				ans.push_back(s.at(i).subject);
			}
		}
	}

	return ans;
}
vector<SStructure> MasterResource::findPartailStructure(vector<POS> order) {
	vector<SStructure> ans;

	for (auto s : sNGramML) {
		if (s.first < order.size()) {
			continue;
		}

		for (int i = 0; i < s.second.size(); i++) {
			vector<POS> current = s.second.at(i).subject.component;
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
					ans.push_back(current);
				}
			}
		}
	}
	return ans;
}
SStructure& MasterResource::findStructurePercision(vector<POS> order) {
	vector<NGram<SStructure>>& a = sNGramML[order.size()];
	for (int i = 0; i < a.size(); i++) {
		vector<POS> current = a.at(i).subject.component;
		bool solution = true;
		if (current.size() == order.size()) {
			for (int a = 0; a < order.size(); a++) {
				if (current.at(a) != order.at(a)) {
					solution = false;
					break;
				}
			}
			if (solution == true) {
				return a.at(i).subject;
			}
		}
	}
}
/////////////////////////////////////ngram
vector<NGram<SStructure>*> MasterResource::findNGramSS(vector<POS> order) {
	map<int, vector<NGram<SStructure>>>::iterator it = sNGramML.find(order.size());
	if (it == sNGramML.end()) {
		return vector<NGram<SStructure>*>();
	}

	vector<NGram<SStructure>>& search = it->second;
	vector<NGram<SStructure>*> ans;
	for (int i = 0; i < search.size(); i++) {
		vector<POS> current = search.at(i).subject.component;

		bool correct = true;
		for (int b = 0; b < current.size(); b++) {
			if (current.at(b) != order.at(b)) {
				correct = false;
				break;
			}
		}

		if (correct == true) {
			ans.push_back(&(search.at(i)));
		}
	}

	return ans;
}

////////////////////////////////////////////probation
NGram<Word> MasterResource::findNGram(Word word) {
	return findNGramP(word);
}

vector<NGram<Word>> MasterResource::findNGram(string word)
{
	vector<NGram<Word>*> ans = findNGramP(word);
	vector<NGram<Word>> dAns;

	for (int i = 0; i < ans.size(); i++) {
		dAns.push_back(*ans[i]);
	}

	return dAns;
}

//could change to search until word not the same (cut down iterations)
vector<NGram<Word>*> MasterResource::findNGramP(string word)
{
	int value = stringToInt(word);
	vector <NGram<Word>*> ans;

	map<int, vector<NGram<Word>>>::iterator it;
	if ((it = ngramML.find(value)) != ngramML.end()) {
		vector<NGram<Word>>& ngrams = it->second;
		for (int i = 0; i < ngrams.size(); i++) {
			if (ngrams[i].subject.name == word) {
				ans.push_back(&ngrams[i]);
			}
		}

		if (ans.size() == 0) {
			vector<NGram<Word>> a = findInFile(word, value);

			if (a[0].subject.name != "not found") {
				vector<NGram<Word>>& ngrams = ngramML.find(value)->second;
				for (int i = 0; i < ngrams.size(); i++) {
					if (ngrams[i].subject == word) {
						ans.push_back(&ngrams[i]);
					}
				}
			}
		}
	}
	else { //todo redo this so that we get direct pointer from findInFile
		vector<NGram<Word>> a = findInFile(word, value);

		if (a[0].subject.name != "not found") {
			vector<NGram<Word>>& ngrams = ngramML.find(value)->second;
			for (int i = 0; i < ngrams.size(); i++) {
				if (ngrams[i].subject == word) {
					ans.push_back(&ngrams[i]);
				}
			}
		}
	}

	return ans;
}

NGram<Word>& MasterResource::findNGramP(Word word) {
	int value = stringToInt(word.name);

	map<int, vector<NGram<Word>>>::iterator it;
	if ((it = ngramML.find(value)) != ngramML.end()) {
		vector<NGram<Word>>& ngrams = it->second;
		for (int i = 0; i < ngrams.size(); i++) {
			if (ngrams[i].subject == word) {
				return ngrams[i];
			}
		}
	}
	else {
		vector<NGram<Word>> a = findInFile(word.name, value);

		if (a[0].subject.name != "not found") {
			vector<NGram<Word>>& ngrams = ngramML.find(value)->second;
			for (int i = 0; i < ngrams.size(); i++) {
				if (ngrams[i].subject == word) {
					return ngrams[i];
				}
			}
		}
	}

	NGram<Word>* temp = new NGram<Word>();
	Word defaultWord("not found", POS::Unknown);
	temp->subject = defaultWord;

	return *temp;
}
Word MasterResource::findWordContext(Word last, string current)
{
	NGram<Word>& word = findNGram(last);
	vector<Word> possible = findWord(current);
	
	int greatest = -1;
	int greatestPos = 0;
	for (int i = 0; i < possible.size(); i++) {
		int val = word.findValue(possible[i]);

		if (val > greatest) {
			greatest = val;
			greatestPos = i;
		}
	}
	return possible[greatestPos];
}
NGram<Word> MasterResource::findWordContextNG(Word last, string current)
{
	NGram<Word>& word = findNGram(last);
	vector<NGram<Word>> possible = findNGram(current);

	int greatest = -1;
	int greatestPos = 0;
	for (int i = 0; i < possible.size(); i++) {
		int val = word.findValue(possible[i].subject);

		if (val > greatest) {
			greatest = val;
			greatestPos = i;
		}
	}
	return possible[greatestPos];
}
vector<Word> MasterResource::findProbationWord(string word) {
	vector<Word> ans;
	for (int i = 0; i < probationWord.size(); i++) {
		if (probationWord.getValueI(i).name == word) {
			ans.push_back(probationWord.getValueI(i));
		}
	}

	return ans;
}

////////////////////////////////////////////////////////////////EXIST
bool MasterResource::wordExist(string word) {
	int value = stringToInt(word);

	map<int, vector<NGram<Word>>>::iterator it;
	if ((it = ngramML.find(value)) != ngramML.end()) {
		vector<NGram<Word>>& ngrams = it->second;
		for (int i = 0; i < ngrams.size(); i++) {
			if (ngrams[i].subject.name == word) {
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
	map<int, vector<NGram<SStructure>>>::iterator it = sNGramML.find(order.size());
	if (it == sNGramML.end()) {
		return false;
	}

	vector<NGram<SStructure>> ans = it->second;
	for (int i = 0; i < ans.size(); i++) {
		if (ans.at(i).subject.component == order) {
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
/*void MasterResource::findNew(string phrase) {
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
					map<int, vector<NGram<Word>>>::iterator it;
					if ((it = ngramML.find(value)) != ngramML.end()) {
						it->second.push_back(newNGram);
					}
					else {
						vector<NGram<Word>> temp;
						temp.push_back(newNGram);

						ngramML[value] = temp;
					}

					probationWord.deleteIndex(probationWord.getPostionV(newWord));
				}
			}
			else {
				probationWord.add(1, newWord);
			}
		}
	}

};*/
void MasterResource::updateNGram(Word input, int newOccerence) {
	NGram<Word> ngram = findNGramP(input);

	ngram.updateItem(input, newOccerence);
}

//////////////////////////////////////////////////////////////////////////DOUBLE
bool MasterResource::isDouble(string word) {
	int value = stringToInt(word);
	map<int, vector<NGram<Word>>>::iterator it;
	if ((it = ngramML.find(value)) != ngramML.end()) {
		vector<NGram<Word>> possible = it->second;
		bool exist = false;

		for (int i = 0; i < possible.size(); i++) {
			if (possible[i].subject.name == word) {
				exist = true;
			}
			else if (possible[i].subject.name == word && exist) {
				return true;
			}
		}
	}

	return false;
}
vector<POS> MasterResource::doublePossibilities(string word) {
	vector<POS> answer;
	int value = stringToInt(word);
	map<int, vector<NGram<Word>>>::iterator it;
	if ((it = ngramML.find(value)) != ngramML.end()) {
		vector<NGram<Word>> possible = it->second;

		for (int i = 0; i < possible.size(); i++) {
			if (possible[i].subject.name == word) {
				answer.push_back(possible[i].subject.type);
			}
		}
	}

	return answer;
}

//todo make this way more efficient 
//right now it is searching the ngrams the number of possibiliites
//should change data structure to word value, (word, occurence) and then sort that by number of occerences
Word MasterResource::findDouble(Word before, string target) {
	if (before.name == "not found") {
		vector<Word> possible = findWord(target);

		return possible[0];
	}
	vector<Word> possible = findWord(target);

	NGram<Word> ngram = findNGram(before);

	int greatest = -1;
	int index = 0;
	for (int i = 0; i < possible.size(); i++) {
		int a = ngram.findValue(possible.at(i));

		if (a > greatest) {
			greatest = a;
			index = i;
		}
	}

	if (possible.size() == 0) {
		Word stand("not found", Unknown);
		return stand;
	}

	return possible.at(index);
}