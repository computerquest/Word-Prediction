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
		int pos = -1;
		if ((pos = binarySearch(nextBranch, 0, nextBranch.size() - 1, inputValue)) != -1) {
			return nextBranch[pos];
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
		if (end > input.size() || begin < 0) {
			return -1;
		}
		int middle = (1 + end - begin) / 2;

		if (input[begin + middle].value == target) {
			return begin + middle;
		}
		else if (end - begin == 1 && input[begin].value != target) {
			return -1;
		}
		else if (input[begin + middle].value > target) {
			return binarySearch(input, begin, begin + (middle - 1), target);
		}
		else if (input[middle].value < target) {
			return binarySearch(input, begin + middle + 1, end, target);
		}
	}
};

