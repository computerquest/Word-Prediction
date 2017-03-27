#include <iostream>
#include <string>
#include <vector>
#include "LinkedList.h"
#pragma once
template<typename T> class NGram {
public:
	T subject;
	int occerence;
	LinkedList<int, T> content;

	NGram() {}
	
	NGram(T subject) : subject(subject) {}

	void setSubject(T target) {
		subject = target;
	}

	T getSubject() {
		return subject;
	}

	int findGreatest() {
		int greatest = 0;
		int index = -1;
		for (int i = 0; i < content.size(); i++) {
			if (content.getKeyI(i) > greatest) {
				index = i;
				greatest = content.getKeyI(i);
			}
		}

		return index;
	}

	void addItem(T item) {
		content.add(1, item);
	}

	void updateItem(T item, int adjust) {
		content.changeKeyV(item, adjust);
	}


	void updateSafe(T item, int increment) {
		if (content.containsV(item)) {
			content.changeKeyV(item, content.getKeyV(item) + increment);
		}
		else {
			addItem(item);
			content.changeKeyV(item, increment);
		}
	}
	/*~NGram() {
		delete subject;
		delete occerence;
		delete content;
	}*/
};

