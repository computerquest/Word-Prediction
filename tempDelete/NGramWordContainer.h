#pragma once
#include "LinkedList.h"
#include "Node.h"
#include "Word.h"
#include "POS.h"
#include "NGram.h"
using namespace std;

class NGramWordContainer {
public:
	Node root; //this contians all the intial sizes that the words can be
	vector<NGram<Word>> dataKey;

	/*
	will sort roots when adding them
	*/
	NGramWordContainer();

	void populate(LinkedList<int, vector<NGram<Word>>> input);

	void generateStringTree(Node& node, Word input, int inputNGram);

	vector<Word> findWord(string word);
	NGram<Word>& findNGram(Word w);

private:
	vector<NGram<Word>> search(string word);
	//int binarySearch(vector<Node> input, int target);
	//int binarySearchBack(vector<Node> input, int begin, int end, int target);
	vector<int> toEnd(Node n);
};