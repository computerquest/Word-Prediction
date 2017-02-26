#include "stdafx.h"
#include "DataCollector.h"

DataCollector::DataCollector(string fileName) : t(fileName) {
	t.read();
	t.trimEndsWhite();
	t.createParagraph();
	t.textParagraph();
	t.joinDialogue();
	t.dialogueFilter();
	t.breakText();
	t.joinCharacters();
	t.addWhiteSpace();
	t.trimEndsWhite();
	t.filterDelimeter('\"');

	dialogue = t.dialogue;
	expos = t.exposition;
}

/*
want to collect more data
	word (ngram, words, etc)
	punctuation ngrams
	ngrams
	
want a similar process for dialogue
*/
void DataCollector::findData()
{
	for (int i = 0; i < expos.size(); i++) {
		vector <char> delimeter;
		delimeter.push_back(' ');
		vector<string> words = breakDownV(expos.at(i), delimeter);
		vector<POS> parts;
		vector<Punctuation> duringPunc;

		Punctuation end = stringToPunctuation(trimWhite(words.at(words.size() - 1)));
		words.erase(words.begin() + words.size() - 1);

		if (end == COMMA) {
			continue;
		}

		for (int i = 0; i < words.size(); i++) {
			Punctuation p = stringToPunctuation(trimWhite(words.at(i)));
			if (p != UNDEFINED) {
				duringPunc.push_back(p);
				words.erase(words.begin() + i);
			}
		}


		for (int b = 0; b < words.size(); b++) {
			Word word = master->findWord(trimWhite(words.at(b)));

			if (word.name != "not found") {
				parts.push_back(word.type);
			}
			else {
				break;
			}
		}

		if (parts.size() == words.size() && !master->sstructureExist(parts)) {
			SStructure newSS(parts);
			newSS.addEnding(end);

			engine->examples.push_back(expos.at(i));
			master->sNGramML.push_back(newSS);
		}
		//need to get the gram to update the occerence
		else if (master->sstructureExist(parts)) {
			SStructure* ss = master->findStructurePercisionP(parts);
			ss->addEnding(end);
			for (int i = 0; i < duringPunc.size(); i++) {
				ss->addPunctuation(duringPunc.at(i));
			}
		}
	}

	
	for (int i = 0; i < dialogue.size(); i++) {
		vector <char> delimeter;
		delimeter.push_back(' ');
		vector<string> words = breakDownV(dialogue.at(i), delimeter);
		vector<POS> parts;
		vector<Punctuation> duringPunc;

		Punctuation end = stringToPunctuation(trimWhite(words.at(words.size() - 1)));
		words.erase(words.begin() + words.size() - 1);

		if (end == COMMA) {
			continue;
		}

		for (int i = 0; i < words.size(); i++) {
			Punctuation p = stringToPunctuation(trimWhite(words.at(i)));
			if (p != UNDEFINED) {
				duringPunc.push_back(p);
				words.erase(words.begin() + i);
			}
		}


		for (int b = 0; b < words.size(); b++) {
			Word word = master->findWord(trimWhite(words.at(b)));

			if (word.name != "not found") {
				parts.push_back(word.type);
			}
			else {
				break;
			}
		}

		if (parts.size() == words.size() && !master->sstructureExist(parts)) {
			SStructure newSS(parts);
			newSS.addEnding(end);

			engine->examples.push_back(dialogue.at(i));
			master->sNGramML.push_back(newSS);
		}
		//need to get the gram to update the occerence
		else if (master->sstructureExist(parts)) {
			SStructure* ss = master->findStructurePercisionP(parts);
			ss->addEnding(end);
			for (int i = 0; i < duringPunc.size(); i++) {
				ss->addPunctuation(duringPunc.at(i));
			}
		}
	}
}
