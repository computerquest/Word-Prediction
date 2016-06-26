#pragma once
#include <iostream>
using namespace std;

class Connection {
public:
	double weight;
	double connectToID;
	double sendId;
	int changes = 0;
	double lastWeight = 0;

	Connection(double weight, double connectToID, double sendId);

	void reciever(double id);

	void setWeight(double value);
	double getWeight();
};