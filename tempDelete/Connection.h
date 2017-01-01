#pragma once
#include <iostream>
using namespace std;

class Connection {
public:
	double weight;
	int connectToID;
	int sendId;
	double epochWeight;
	double lastWeight;
	double nextWeight;

	Connection(double weight, int connectToID, int sendId);

	void reciever(int id);

	void setWeight(double value);
	double getWeight();
};