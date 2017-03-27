#include "NGramWordContainer.h"
using namespace std;

void NGramWordContainer::populate(LinkedList<int, vector<NGram<Word>>> baseInput) {
	vector<int> rootValues;
	for (int i = 0; i < root.size(); i++) {
		rootValues.push_back(root.at(i).value);
	}

	for (int z = 0; z < baseInput.size(); z++) {
		vector<NGram<Word>> input = baseInput.getValueI(z);

		//this is the first one for the length()
		for (int i = 0; i < input.size(); i++) {
			dataKey.push_back(input.at(i));

			string currentValue = input.at(i).getSubject().getWord();

			vector<int> rootValues;
			for (int i = 0; i < root.size(); i++) {
				rootValues.push_back(root.at(i).value);
			}

			if (binaryExist(rootValues, currentValue.length())) {
				for (int a = 0; a < root.size(); a++) {
					if (currentValue.length() == root.at(a).value) {
						generateStringTree(root.at(a), currentValue, dataKey.size() - 1);
					}
				}
			}
			else {
				Node newRoot(currentValue.length());
				generateStringTree(newRoot, currentValue, dataKey.size() - 1);

				for (int a = root.size() - 1; a >= -1; a--) {
					if (a == -1) {
						root.insert(root.begin(), newRoot);
						break;
					}
					else if (root[a].value < currentValue.length()) {
						root.insert(root.begin() + a + 1, newRoot);
						break;
					}
				}
			}
		}
	}
}

void NGramWordContainer::generateStringTree(Node& node, Word word, int intFinalIndex) {
	string input = word.getWord();
	Node* currentNode = &node;

	for (int i = 0; i < input.length(); i++) {
		currentNode = &(currentNode->addNext(input[i]));
	}

	currentNode = &(currentNode->addNext(POStoInt(word.getType())));
	currentNode->addNext(intFinalIndex);
}

vector<Word> NGramWordContainer::findWord(string word)
{
	vector<NGram<Word>> data = search(word);
	vector<Word> answer;
	for (int i = 0; i < data.size(); i++) {
		answer.push_back(data[i].subject);
	}
	return answer;
}

NGram<Word> NGramWordContainer::findNGram(Word w)
{
	vector<NGram<Word>> data = search(w.name);
	for (int i = 0; i < data.size(); i++) {
		if (w == data[i].subject) {
			return data[i];
		}
	}
}

NGramWordContainer::~NGramWordContainer()
{
}

vector<NGram<Word>> NGramWordContainer::search(string word)
{
	vector<NGram<Word>> answer;
	Node* currentNode;
	int startIndex = binarySearch(root, word.length());
	if (startIndex != -1) {
		currentNode = &root[startIndex];
	}
	else {
		return vector<NGram<Word>>();
	}

	for (int i = 0; i < word.length(); i++) {
		startIndex = binarySearch(currentNode->nextBranch, word[i]);
		if (startIndex != -1) {
			currentNode = &(currentNode->nextBranch[startIndex]);
		}
		else {
			return vector<NGram<Word>>();
		}
	}

	vector<int> index = toEnd(*currentNode);
	for (int i = 0; i < index.size(); i++) {
		answer.push_back(dataKey[index[i]]);
	}

	return answer;
}

int NGramWordContainer::binarySearch(vector<Node> input, int target)
{
	return binarySearchBack(input, 0, input.size()-1, target);
}

int NGramWordContainer::binarySearchBack(vector<Node> input, int begin, int end, int target)
{
	if (end > input.size() || begin < 0) {
		return -1;
	}
	int middle = (1+end-begin) / 2;

	if (input[begin+middle].value == target) {
		return begin+middle;
	}
	else if (end-begin == 1 && input[begin].value != target) {
		return -1;
	}
	else if (input[begin+middle].value > target) {
		return binarySearchBack(input, begin, begin+(middle-1), target);
	}
	else if (input[middle].value < target) {
		return binarySearchBack(input, begin+middle+1, end, target);
	}
}

vector<int> NGramWordContainer::toEnd(Node n)
{
	vector<int> answer;
	if (n.nextBranch.size() == 0) {
		answer.push_back(n.value);
		return answer;
	}
	for (int i = 0; i < n.nextBranch.size(); i++) {
		vector<int> ans = toEnd(n.nextBranch[i]);
		answer.insert(answer.end(), ans.begin(), ans.end());
	}
	return answer;
}


