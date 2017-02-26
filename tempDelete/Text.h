#include "FileManager.h"
#pragma once

class Text {
public:
	FileManager file;
	vector<string> exposition;
	vector<string> dialogue;
	vector<vector<string>> rawFile;

	Text(string fileName) {
		file.file = fileName;
	}

	void createParagraph();

	void read();

	//vector<vector<string>> breakText();

	void filterDelimeter(char character);
	void filiterDataSize(int minSize);

	void trimEndsWhite();
	void breakText();
	void joinCharacters();
	void dialogueFilter();

	void addWhiteSpace();

	void joinDialogue();

	//this is pretty specific to sherlock holmes
	void textParagraph();
	//void removeCharacter();
};