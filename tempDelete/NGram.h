#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "LinkedList.h"
#pragma once
using namespace std;

template<typename T> class NGram {
public:
	T subject;
	int occerence;
	map<int, vector<T>> content;

	NGram() {}
	
	NGram(T subject) : subject(subject) {}

	void setSubject(T target) {
		subject = target;
	}

	T getSubject() {
		return subject;
	}

	int findGreatest() {
		return content.rbegin()->first;
	}

	void addItem(T item) {
		content[1].push_back(item);
	}

	void addItem(T item, int startAmount) {
		content[startAmount].push_back(item);
	}

	int updateItem(T item, int adjust) {
		int val = -1;
		for (auto& it : content) {
			vector<T>& v = it.second;
			for (int i = 0; i < v.size(); i++) {
				if (v[i] == item) {
					val = it.first;
					v.erase(v.begin()+i);
					content[val + adjust].push_back(item);
					return val + adjust;
				}
			}
		}


		return -1;
	}

	int findValue(T t) {
		for (auto it : content) {
			vector<T>& a = it.second;

			for (int i = 0; i < a.size(); i++) {
				if (t == a[i]) {
					return it.first;
				}
			}
		}

		return -1;
	}

	void updateSafe(T item, int increment) {
		int v = updateItem(item, increment);
		if (v == -1) {
			content[increment].push_back(item);
		}
	}
	/*~NGram() {
		delete subject;
		delete occerence;
		delete content;
	}*/
};

