#include "stdafx.h"
#include "PredictionEngine.h"
#include "MasterResource.h"
#include "RBM.h"
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

//this most likely isnt a fiar test case because it is based on nothing
void randomTest() {
	MLP m;
	vector<int> hM;
	m.learningRate = .5;
	hM.push_back(4);
	hM.push_back(2);
	m.initialize(2, hM, 1, false);
	LinkedList<vector<double>, vector<double>> mInput;
	vector<double> in;
	in.push_back(0);
	in.push_back(0);

	vector<double> inA;
	inA.push_back(0);
	inA.push_back(1);

	vector<double> out;
	out.push_back(0);

	vector<double> outA;
	outA.push_back(1);

	mInput.add(in, out);
	mInput.add(inA, outA);
	
	m.trainingValues = mInput;
	m.train(10000, "testing");
}

void applicationTest() {
	MasterResource master;
	MasterResource* pMaster = &master;

	//hidden layer numbers
	vector<int> temprorary;
	temprorary.push_back(40); //was just 40 originally
	temprorary.push_back(10);

	PredictionEngine engine(43, temprorary, 3, pMaster, false);

	master.engine = &engine;

	engine.nn.learningRate = .1;
	engine.nn.momentum = .9;

	engine.nn.train(10000, "linearSeperable");
	delete pMaster;
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
	/*MLP m;
	vector<int> hM;
	hM.push_back(2);
	m.initializeNN(10, hM, 2, true);
	Neuron n = *m.findNeuron(13);
	/*RBM rbm;

	rbm.initializeNetwork(7, 2);

	vector<double> input;
	input.push_back(0);
	input.push_back(1);
	input.push_back(1);
	input.push_back(0);
	input.push_back(1);
	input.push_back(1);
	input.push_back(1);

	rbm.currentInput = input;
	rbm.process();

	cout << "end" << endl;*/

	/*MLP m;
	vector<int> hM;
	m.learningRate = .1;
	m.momentum = .9;
	hM.push_back(2);
	m.initializeNN(2, hM, 2, true);
	LinkedList<vector<double>, vector<double>> mInput;
	vector<double> in;
	in.push_back(.05);
	in.push_back(.1);
	vector<double> inA;
	inA.push_back(.3);
	inA.push_back(.21);
	vector<double> out;
	out.push_back(.01);
	out.push_back(.99);
	vector<double> outA;
	outA.push_back(.98);
	outA.push_back(.33);
	mInput.add(in, out);
	mInput.add(inA, outA);
	m.trainingValues = mInput;
	m.currentInput = in;
	m.batch = true;
	m.train(50000, "testing");

	/*MasterResource master;
	MasterResource* pMaster = &master;

	//hidden layer numbers
	vector<int> temprorary;
	temprorary.push_back(20);

	PredictionEngine engine(43, temprorary, 3, pMaster, true);

	master.engine = &engine;

	engine.nn.learningRate = .1;
	engine.nn.momentum = .9;

	engine.nn.train(10000, "linearSeperable");

	/*engine.mergeTesting("a cat skids into a room"); //skids WORKS
	engine.mergeTesting("dogs are crazy"); //dogs FINE (tough one)
	engine.mergeTesting("the imperials have the disadvantage"); //disadvantage WORKS
	engine.mergeTesting("the opposition have the flag"); //flag //WORKS
	engine.mergeTesting("jared is not the killer"); //killer n v a art n
	engine.mergeTesting("he lied to us"); //lied n v p n
	engine.mergeTesting("you have the right to remain silent"); //remain n v art n v a
	engine.mergeTesting("jared was the one selling secrets to the russians"); //selling n v art n v n  p art n
	//engine.multipleMissing("a cat skids into a church");
	//engine.multipleMissing("can he leave");

	/*need to find the structure for the double meaning words*/
	/*good
	/*havent added the data for
	engine.findTypeDeployment("jared is not the killer"); //killer n v a art n
	engine.findTypeDeployment("he lied to us"); //lied n v p n
	engine.findTypeDeployment("you have the right to remain silent"); //right n v art n a v
	engine.findTypeDeployment("jared was the one selling secrets to the russians"); //secrets n v art n v n  p art n*/


	cout << "end" << endl;
	return 0;
}