#pragma once
using namespace std;

template<typename T> bool vectorContains(vector<T> input, T check) {
	for (int i = 0; i < input.size(); i++) {
		if (input.at(i) == check) {
			return true;
		}
	}

	return false;
}