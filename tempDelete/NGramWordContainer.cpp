#include "NGramWordContainer.h"
using namespace std;

NGramWordContainer::NGramWordContainer(): root(-1){}

void NGramWordContainer::populate(LinkedList<int, vector<NGram<Word>>> baseInput) {
	for (int z = 0; z < baseInput.size(); z++) {
		vector<NGram<Word>> input = baseInput.getValueI(z);

		//this is the first one for the length()
		for (int i = 0; i < input.size(); i++) {
			dataKey.push_back(input.at(i));

			string currentValue = input.at(i).getSubject().getWord();
			generateStringTree(root.addNext(currentValue.length()), currentValue, dataKey.size() - 1);
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

NGram<Word>& NGramWordContainer::findNGram(Word w)
{
	vector<NGram<Word>> data = search(w.name);
	for (int i = 0; i < data.size(); i++) {
		if (w == data[i].subject) {
			return data[i];
		}
	}
}

vector<NGram<Word>> NGramWordContainer::search(string word)
{
	vector<NGram<Word>> answer;
	Node* currentNode = &(root.getNext(word.length()));
	if (currentNode->value == -1) {
		return vector<NGram<Word>>();
	}

	for (int i = 0; i < word.length(); i++) {
		currentNode = &(currentNode->getNext(word[i]));
		if (currentNode->value == -1) {
			return vector<NGram<Word>>();
		}
	}
	vector<int> index = toEnd(*currentNode);
	for (int i = 0; i < index.size(); i++) {
		answer.push_back(dataKey[index[i]]);
	}

	return answer;
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


