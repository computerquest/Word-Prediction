#include "stdafx.h"
#include "PredictionEngine.h"
#include "MasterResource.h"
#include "MLP.h"

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
int main()
{
	applicationTest();
	cout << "end" << endl;
	return 0;
}