#pragma once
#include "Connection.h"
using namespace std;

Connection::Connection(double weight, double connectToID, double sendId) : weight(weight), connectToID(connectToID), sendId(sendId) {}

void Connection::reciever(double id) {
	connectToID = id;
}

void Connection::setWeight(double value) {
	changes++;
	lastWeight = weight;
	weight = value;
}
double Connection::getWeight() {
	return weight;
}