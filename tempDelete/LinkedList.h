#pragma once
#include <vector>
using namespace std;

//MAY HAVE ISSUES BECAUSE YOU ARE USING RETURNING A VALUE WHEN ACCESSING INSTEAD OF A POINTER
template<typename K, typename V> class LinkedList {
public:
	vector<K> key;
	vector<V> value;

	bool containsV(V input) {
		for (int i = 0; i < value.size(); i++) {
			if (value.at(i) == input) {
				return true;
			}
		}

		return false;
	}
	bool containsK(K input) {
		for (int i = 0; i < key.size(); i++) {
			if (key.at(i) == input) {
				return true;
			}
		}

		return false;
	}

	void filterAmountV(V type, int input) {
		int amount = 0;
		for (int i = 0; i < value.size(); i++) {
			if (type == value.at(i)) {
				amount++;
				if (amount > input) {
					deleteIndex(i);
				}
			}
		}//
	}//

	void filterAmountK(K type, int input) {
		int amount = 0;
		for (int i = 0; i < key.size(); i++) {
			if (type == key.at(i)) {
				amount++;
				if (amount > input) {
					deleteIndex(i);
				}
			}
		}
	}

	void filterDuplicatesK() {
		for (int i = 0; i < key.size(); i++) {
			for (int a = i + 1; a < key.size(); a++) {
				if (key.at(i) == key.at(a)) {
					deleteIndex(a);
				}
			}
		}
	}

	void filterDuplicatesV() {
		for (int i = 0; i < value.size(); i++) {

			for (int a = i + 1; a < value.size(); a++) {
				if (value.at(i) == value.at(a)) {
					deleteIndex(a);
				}
			}
		}
	}

	void findDuplicates() {
		filterDuplicatesK();
		filterDuplicatesV();
	} 
	/////////////////////////////////////////////delete//////////////////////////////////////
	void deleteIndex(int input) {
		//key.earse(key.begin() + i);
		//value.earse(value.begin() + i);
		vector<K> newKey;
		vector<V> newValue;

		for (int i = 0; i < key.size(); i++) {
			if (input == i) {
				continue;
			}

			newKey.push_back(key.at(i));
			newValue.push_back(value.at(i));
		}

		key = newKey;
		value = newValue;
	}

	///////////////////////////////////////////////////position//////////////////////////
	int getPostionK(K search) {
		for (int i = 0; i < key.size(); i++) {
			if (key.at(i) == search) {
				return i;
			}
		}

		return -1;;
	}

	int getPostionV(V search) {
		for (int i = 0; i < value.size(); i++) {
			if (value.at(i) == search) {
				return i;
			}
		}

		return -1;
	}

	/////////////////////////////////////////////////change/////////////////////////////////////
	void changeKeyV(V search, K change) {
		int index = getPostionV(search);

		key[index] = change;
	}

	void changeKeyI(int i, K change) {
		key[i] = change;
	}

	void changeValueK(K search, V change) {
		int index = getPostionK(search);

		value[index] = change;
	}

	void changeValueI(int i, V change) {
		value[i] = change;
	}

	void changeAll(int i, K changeK, V changeV) {
		changeKeyI(i, changeK);
		changeValueI(i, changeV);
	}

	void setKey(vector<K> key) {
		this->key = key;
	}
	void setValue(vector<V> value) {
		this->value = value;
	}
	void setAll(vector<K> key, vector<V> value) {
		setKey(key);
		setValue(value);
	}

	///////////////////////////////////////////////////////////////get///////////////////////////////////
	V getValueK(K keyValue) {
		int index = getPostionK(keyValue);

		return value.at(index);
	}

	V getValueI(int index) {
		return value.at(index);
	}

	K getKeyV(V valueInput) {
		int index = getPostionV(valueInput);

		return key.at(index);
	}

	K getKeyI(int index) {
		return key.at(index);
	}

	////////////////////////////////////////////////////////misc//////////////////////////////////
	int size() {
		return key.size();
	}

	void add(K addKey, V addValue) {
		value.push_back(addValue);
		key.push_back(addKey);
	}

	/*~LinkedList() {
		delete key;
		delete value;
	}*/
};