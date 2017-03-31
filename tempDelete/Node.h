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

	Node(int value) {
		this->value = value;
	}

	Node& addNext(int inputValue) {
		int pos = -1;
		if ((pos = binarySearch(nextBranch, 0, nextBranch.size() - 1, inputValue)) != -1) {
			return nextBranch[pos];
		}

		return insert(inputValue);
	}

	Node& getNext(int value) {
		int pos = -1;
		if ((pos = binarySearch(nextBranch, 0, nextBranch.size() - 1, value)) != -1) {
			return nextBranch[pos];
		}

		return Node(-1);
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
	Node& insert(int value) {
		Node newRoot(value);
		for (int a = nextBranch.size() - 1; a >= -1; a--) {
			if (a == -1) {
				nextBranch.insert(nextBranch.begin(), newRoot);
				return nextBranch[0];
			}
			else if (nextBranch[a].value < value) {
				nextBranch.insert(nextBranch.begin() + a + 1, newRoot);
				return nextBranch[a+1];
			}
		}
	}

	bool binaryExist(vector<int> input, int target) {
		if (input.size() == 0) {
			return 0;
		}
		int middle = (input.size()) / 2;

		if (input[middle] == target) {
			return true;
		}
		else if (input.size() == 1 && input[0] != target) {
			return false;
		}
		else if (input[middle] > target) {
			for (int i = middle; i < input.size(); i++) {
				input.erase(input.begin() + middle);
			}

			return binaryExist(input, target);
		}
		else if (input[middle] < target) {
			for (int i = 0; i <= middle; i++) {
				input.erase(input.begin());
			}

			return binaryExist(input, target);
		}
	}

	int binarySearch(vector<Node> input, int begin, int end, int target)
	{
		if (begin > end) {
			return -1;
		}
		int middle = (begin + end) / 2;

		if (input[middle].value == target) {
			return middle;
		}
		else if (end - begin == 0 && input[middle].value != target) {
			return -1;
		}
		else if (input[middle].value > target) {
			return binarySearch(input, begin, (middle - 1), target);
		}
		else if (input[middle].value < target) {
			return binarySearch(input, middle + 1, end, target);
		}
	}
};

