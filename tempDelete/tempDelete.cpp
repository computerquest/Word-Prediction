#include "stdafx.h"
#include "DataCollector.h"
#include "PredictionEngine.h"
#include "MasterResource.h"
#include "MLP.h"
#include "Text.h"
#include "NGramWordContainer.h"

void defaultTest() {
	MLP m;
	vector<int> hM;
	hM.push_back(2);
	hM.push_back(2);
	m.initialize(2, hM, 2, false);

	vector<double> input;
	input.push_back(.05);
	input.push_back(.1);

	vector<double> desired;
	desired.push_back(.01);
	desired.push_back(.99);

	LinkedList<vector<double>, vector<double>> mInput;
	mInput.add(input, desired);

	m.trainingValues = mInput;

	m.train(10000, "stuff");
}

void applicationTest() {
	//hidden layer numbers
	vector<int> temprorary;
	temprorary.push_back(40); //was just 40 originally
	temprorary.push_back(10);

	PredictionEngine engine(43, temprorary, 3, false, true);

	engine.nn.learningRate = .1;
	engine.nn.momentum = .9;

	engine.nn.train(10000, "linearSeperable");
}

void practicalTest() {
	MasterResource master;
	MasterResource* pMaster = &master;

	//hidden layer numbers
	vector<int> temprorary;
	temprorary.push_back(40);
	//temprorary.push_back(10);

	PredictionEngine engine(43, temprorary, 3, pMaster, false);

	master.engine = &engine;

	engine.nn.learningRate = .1;
	engine.nn.momentum = .9;

	engine.mergeTesting("a cat skids into a room"); //skids WORKS
	engine.mergeTesting("dogs are crazy"); //dogs FINE (tough one)
	engine.mergeTesting("the imperials have the disadvantage"); //disadvantage WORKS
	engine.mergeTesting("the opposition have the flag"); //flag //WORKS
	engine.mergeTesting("jared is not the killer"); //killer n v a art n
	engine.mergeTesting("he lied to us"); //lied n v p n
	engine.mergeTesting("you have the right to remain silent"); //remain n v art n v a
	engine.mergeTesting("jared was the one selling secrets to the russians"); //selling n v art n v n  p art n

	delete pMaster;
}

int binarySearch(vector<int> input, int begin, int end, int target)
{
	if (begin > end) {
		return -1;
	}
	int middle = (begin+end) / 2;

	if (input[middle] == target) {
		return middle;
	}
	else if (end - begin == 0 && input[middle] != target) {
		return -1;
	}
	else if (input[middle] > target) {
		return binarySearch(input, begin, (middle-1), target);
	}
	else if (input[middle] < target) {
		return binarySearch(input, middle+1, end, target);
	}
}
void dataCollectorTest() {
	vector<int> i;
	i.push_back(12);
	i.push_back(435);
	i.push_back(666);
i.push_back(10000);
cout <<	binarySearch(i, 0, i.size() - 1, 12);
cout <<	binarySearch(i, 0, i.size() - 1, 1);
	cout << binarySearch(i, 0, i.size() - 1, 2);
	cout << binarySearch(i, 0, i.size() - 1, 435);
	cout << binarySearch(i, 0, i.size() - 1, 666);
	cout << binarySearch(i, 0, i.size() - 1, 10000);

 	MasterResource master;
	//hidden layer numbers
	vector<int> temprorary;
	temprorary.push_back(1); //was just 40 originally
	temprorary.push_back(1);

	PredictionEngine engine(1, temprorary, 3, false, true);

	Text t("Sherlock.txt");
	t.read();
	t.createParagraph();

	//applicationTest();
	DataCollector d("Sherlock.txt");
	d.master = &master;
	d.engine = &engine;

	//d.read();
	//d.dialogueFilter();
	//d.breakExpos();
	//d.trimEndsWhite();
	//d.joinCharacters();
	//d.addWhiteSpace();
	//d.breakText();
	//d.trimEndsWhite();
	//d.joinCharacters();
	//d.filterDelimeter('\"');
	d.findData();

	NGramWordContainer n;

	n.populate(master.ngramML);
	vector<Word> a = n.findWord("scratch");
	vector<Word> w = n.findWord("he");

	//n.findNGram(w);
	//engine.createTraining();
}



void treeTest() {
	dataCollectorTest();
	MasterResource m;

	NGramWordContainer n;

	n.populate(m.ngramML);
}
int main()
{
	dataCollectorTest();

	cout << "end" << endl;
	return 0;
}