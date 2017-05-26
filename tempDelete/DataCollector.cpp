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


		/*NGram<Word>* lastNG = nullptr;
		for (int b = 0; b < words.size(); b++) {
			NGram<Word>& word = master->findNGramP(trimWhite(words.at(b))); //todo fix this

			if (word.subject.name != "not found") {
				parts.push_back(word.subject.type);

				if (b>0) {
					lastNG->updateSafe(word.subject, 1);
				}

				lastNG = &word;
			}
			else {
				delete &word;
				break;
			}
		}*/

		//todo add sentence starter ngram for this to try and determine first word type
		vector<Word> wordList;
		wordList.push_back(master->findWord(words.at(0)).at(0));
		parts.push_back(wordList.at(wordList.size() - 1).type);

		for (int b = 1; b < words.size(); b++) {
			Word w = master->findDouble(wordList.at(wordList.size() - 1), trimWhite(words.at(b)));

			if (w.name != "not found") {	
				wordList.push_back(master->findDouble(wordList.at(wordList.size()-1), trimWhite(words.at(b))));
				parts.push_back(w.type);
			}
			else {
				break;
			}
		}

		if (parts.size() == words.size() && !master->sstructureExist(parts)) {
			SStructure newSS(parts);
			newSS.addEnding(end);

			engine->examples.push_back(expos.at(i));
			master->sNGramML[newSS.component.size()].push_back(newSS);
		}
		//need to get the gram to update the occerence
		else if (master->sstructureExist(parts)) {
			SStructure ss = master->findStructurePercision(parts);
			ss.addEnding(end);
			for (int i = 0; i < duringPunc.size(); i++) {
				ss.addPunctuation(duringPunc.at(i));
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

		/*NGram<Word>* lastNG = nullptr;
		for (int b = 0; b < words.size(); b++) {
			NGram<Word>& word = master->findNGramP(trimWhite(words.at(b))); //todo shouldn't work

			if (word.subject.name != "not found") {
				parts.push_back(word.subject.type);

				if (b > 0) {
					lastNG->updateSafe(word.subject, 1);
				}

				lastNG = &word;
			}
			else {
				delete &word;
				break;
			}
		}*/

		Word temp("");
		parts.push_back(master->findWordContext(temp, trimWhite(words.at(0))).type);
		for (int b = 1; b < words.size(); b++) {
			NGram<Word> word = master->findWordContext(words[i-1],trimWhite(words.at(b))); //todo shouldn't work

			if (word.subject.name != "not found") {
				parts.push_back(word.subject.type);
			}
			else {
				break;
			}
		}

		if (parts.size() == words.size() && !master->sstructureExist(parts)) {
			SStructure newSS(parts);
			newSS.addEnding(end);

			engine->examples.push_back(dialogue.at(i));
			master->sNGramML[newSS.component.size()].push_back(newSS);
		}
		//need to get the gram to update the occerence
		else if (master->sstructureExist(parts)) {
			SStructure ss = master->findStructurePercision(parts);
			ss.addEnding(end);
			for (int i = 0; i < duringPunc.size(); i++) {
				ss.addPunctuation(duringPunc.at(i));
			}
		}
	}
}
