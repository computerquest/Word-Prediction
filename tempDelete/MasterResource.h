#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "LinkedList.h"
#include "POS.h"
#include "Word.h"
#include "SStructure.h" 
#include "NGram.h"//
#include <algorithm>
#include "BreakDown.h"
#include <fstream>
#include <map>
using namespace std;
//
class PredictionEngine;

class MasterResource {
public:
	map<int, vector<NGram<Word>>> ngramML;
	vector<NGram<SStructure>> sNGramML;
	LinkedList<int, Word> probationWord;
	LinkedList<int, SStructure> probationSS;
	PredictionEngine* engine;

	MasterResource();
	///////////////////////////////////////////////////////////////////////////file
	vector<vector<string>> fileInput(string fileName, vector<char> delimeter);
	vector<vector<string>> fileInput(string fileName, vector<char> delimeter, char stopChar);
	void saveStruct();
	void readStruct();

	void saveNGram();
	void readNGram();
	void writeProbationWord();
	void writeProbationSS();
	void readProbationWord();
	void readProbationSS();

	////////////////////////////////////////////////////////////////////////////////find
	vector<POS> findAllWordType(string input);
	Word findWord(string word);
	//Word findDouble(string phrase, string target);
	SStructure findPartailStructure(vector<POS> order);

	SStructure& findStructurePercisionP(vector<POS> order);

	Word findProbationWord(string word);

	vector<SStructure> matchPossibleSS(vector<POS> input);
	SStructure findStructurePercision(vector<POS> order);
	SStructure findPartailStructure(vector<POS> order, int missing);

	NGram<SStructure>& findNGramSS(vector<POS> order);
	NGram<SStructure> findNGramS(vector<POS> parts);

	NGram<Word> findNGram(Word word);
	NGram<Word>& findNGramP(Word word);

	NGram<Word> findInFile(string search, int file);
	POS decypherType(string secondType);

	////////////////////////////////////////////////////////////////exist
	bool sstructureExist(vector<POS> order);

	bool wordExistAll(string word);

	bool probationExistWord(string word);
	bool probationExistSS(vector<POS> input);

	bool wordExist(string word);

	bool ngramExist(Word word);
	bool ngramExist(string word);

	////////////////////////////////////////////////////////////////////////misc
	void updateNGram(Word input, int newOccerence);
	void findNew(string phrase);
	//Word findWord(string word, POS type);
	//SStructure findStructure(vector<POS> components, int variation, POS alternate);
	bool isDouble(string word);
	vector<POS> doublePossibilities(string word);
	POS findDouble(string phrase, string target);
};