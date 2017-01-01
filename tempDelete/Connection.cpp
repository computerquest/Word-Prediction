#pragma once
#include "Connection.h"
using namespace std;

Connection::Connection(double weight, int connectToID, int sendId) : weight(weight), connectToID(connectToID), sendId(sendId) {
	epochWeight = weight;
	lastWeight = weight;
	nextWeight = 0;
}

void Connection::reciever(int id) {
	connectToID = id;
}

void Connection::setWeight(double value) {
	weight = value;
}
double Connection::getWeight() {
	return weight;
}