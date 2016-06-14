#include "stdafx.h"
#include "PredictionEngine.h"
#include "MasterResource.h"
int main()
{
	MasterResource master;
	MasterResource* pMaster = &master;
	//hidden layer numbers
	vector<int> temprorary;
	temprorary.push_back(2);
	temprorary.push_back(2);
	//temprorary.push_back(4);

	PredictionEngine engine(38, temprorary, 3, pMaster);

	master.engine = &engine;

	engine.multipleMissing("a cat skids into a room"); //skids WORKS
	engine.multipleMissing("dogs are crazy"); //dogs FINE
	engine.multipleMissing("a cat skids into a room"); //skids WORKS
	engine.multipleMissing("the imperials have the disadvantage"); //disadvantage
	engine.multipleMissing("the opposition have the flag"); //flag
	//engine.multipleMissing("a cat skids into a church");
	//engine.multipleMissing("can he leave");

	/*need to find the structure for the double meaning words*/
	/*good
	engine.findTypeDeployment("dogs are crazy"); //dogs FINE
	engine.findTypeDeployment("a cat skids into a room"); //skids WORKS
	engine.findTypeDeployment("the imperials have the disadvantage"); //disadvantage
	engine.findTypeDeployment("the opposition have the flag"); //flag*/

	/*havent added the data for
	engine.findTypeDeployment("jared is not the killer"); //killer
	engine.findTypeDeployment("he lied to us"); //lied
	engine.findTypeDeployment("you have the right to remain silent"); //right
	engine.findTypeDeployment("jared was the one selling secrets to the russians"); //secrets*/

	cout << "end" << endl;
	return 0;
}