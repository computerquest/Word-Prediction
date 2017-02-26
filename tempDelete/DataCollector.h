#pragma once
#include "MasterResource.h"
#include <string>
#include "FileManager.h"
#include "Text.h"
#include "PredictionEngine.h"
using namespace std;

class DataCollector {
public:
	MasterResource* master;
	FileManager traingExampleOutput;
	FileManager testExampleOutput;
	Text t;
	vector<string> expos;
	vector<string> dialogue;
	PredictionEngine* engine;

	DataCollector(string fileName);

	/*void breakExpos();
	void joinCharacters();
	void dialogueFilter();

	void addWhiteSpace();
	//void removeCharacter();*/

	void findData();
};