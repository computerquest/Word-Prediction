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
	map<int, vector<NGram<Word>>> ngramML; //word length, word
	map<int, vector<NGram<SStructure>>> sNGramML; //structure length, structure
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
	vector<Word> findWord(string word);
	//Word findDouble(string phrase, string target);

	SStructure& findStructurePercision(vector<POS> order);

	vector<Word> findProbationWord(string word);

	vector<SStructure> matchPossibleSS(vector<POS> input);
	vector<SStructure> findPartailStructure(vector<POS> order, int missing); //looks for strucutres that could match from one that has a gap
	vector<SStructure> findPartailStructure(vector<POS> order); //looks for strucutres that partially fit from the beginning

	vector<NGram<SStructure>*> findNGramSS(vector<POS> order);

	NGram<Word> findNGram(Word word);
	vector<NGram<Word>> findNGram(string word);
	vector<NGram<Word>*> findNGramP(string word);
	NGram<Word>& findNGramP(Word word);
	Word findWordContext(Word last, string current);
	NGram<Word> findWordContextNG(Word last, string current);

	NGram<Word> findInFile(string search, int file);
	string decypherType(string secondType);

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
	//void findNew(string phrase); todo needs to be updated and put to use (if found worthy)
	//Word findWord(string word, POS type);
	//SStructure findStructure(vector<POS> components, int variation, POS alternate);
	bool isDouble(string word);
	vector<POS> doublePossibilities(string word);
	Word findDouble(Word before, string target);
};