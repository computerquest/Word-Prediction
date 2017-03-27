#pragma once
#include <vector>
using namespace std;

//we will just be using the class to compare and see if we have the right value 
//so it can be type template and still work
//all the heavy work would need to be done by a driver class
class Node{
public:
	vector<Node> nextBranch; // can not have duplicates
	int value;
	int depth;

	Node(int value) {
		this->value = value;
	}

	Node& addNext(int inputValue) {
		for (int i = 0; i < nextBranch.size(); i++) {
			if (nextBranch[i].value == inputValue) {
				return nextBranch[i];
			}
		}
		
		insert(inputValue);

		return nextBranch[nextBranch.size()-1];
	}

	bool operator==(Node rightSide) {
		if (rightSide.value == value) {
			return true;
		}

		return false;
	}

	bool operator==(Node* rightSide) {
		if (rightSide->value == value) {
			return true;
		}

		return false;
	}

private:
	void insert(int value) {
		Node newRoot(value);
		for (int a = nextBranch.size() - 1; a >= -1; a--) {
			if (a == -1) {
				nextBranch.insert(nextBranch.begin(), newRoot);
				break;
			}
			else if (nextBranch[a].value < value) {
				nextBranch.insert(nextBranch.begin() + a + 1, newRoot);
				break;
			}
		}
	}
};

