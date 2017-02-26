#pragma once
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class FileManager {
public:
	string file;

	vector<vector<string>> fileInput(vector<char> delimeter);
	vector<vector<string>> fileInput(vector<char> delimeter, char stopChar);
	vector<vector<string>> fileInputKD(vector<char> delimeter);

	void save(vector<string> input, bool keepOld);
	vector<string> read();
};