#include "FileManager.h"
#include <fstream>
using namespace std;

/*
I would like to seperate the breaking delimeters with reading 
that makes it so that I can use the delimeter method at any time
I would then use that method to read the file and then break down so i could have vector<string>
*/
vector<vector<string>> FileManager::fileInput(vector<char> delimeter) {
	ifstream stream;
	stream.open(file);

	vector<vector<string>> answer;

	string line;
	while (std::getline(stream, line)) {
		vector<string> thisLine;
		for (int a = 0; a < line.length(); a++) {
			for (int i = 0; i < delimeter.size(); i++) {
				if (line[a] == delimeter.at(i)) {
					thisLine.push_back(line.substr(0, a));
					line = line.substr(a + 1);
					a = 0;
					if (line == "") {
						break;
					}
				}
			}
		}
		if (line != "") {
			thisLine.push_back(line);
		}

		answer.push_back(thisLine);
	}

	return answer;
}
vector<vector<string>> FileManager::fileInput(vector<char> delimeter, char stopChar) {
	ifstream stream;
	stream.open(file);

	vector<vector<string>> answer;

	string line;
	while (std::getline(stream, line)) {
		vector<string> thisLine;
		int a = 0;
		while (a < line.length()) {
			bool activate = false;
			if (line[a] != stopChar) {
				for (int i = 0; i < delimeter.size(); i++) {
					if (line[a] == delimeter.at(i)) {
						thisLine.push_back(line.substr(0, a));
						line = line.substr(a + 1);

						a = 0;
						activate = true;

						break;
					}
				}
			}
			else {
				break;
			}

			if (activate == true) {
				continue;
			}
			a++;
		}
		answer.push_back(thisLine);
	}

	return answer;
}

vector<vector<string>> FileManager::fileInputKD(vector<char> delimeter)
{
	ifstream stream;
	stream.open(file);

	vector<vector<string>> answer;

	string line;
	while (std::getline(stream, line)) {
		vector<string> thisLine;
		for (int a = 0; a < line.length(); a++) {
			for (int i = 0; i < delimeter.size(); i++) {
				if (line[a] == delimeter.at(i)) {
					thisLine.push_back(line.substr(0, a+1));
					line = line.substr(a + 1);
					a = 0;
					if (line == "") {
						break;
					}
				}
			}
		}
		if (line != "") {
			thisLine.push_back(line);
		}

		answer.push_back(thisLine);
	}

	return answer;
}

void FileManager::save(vector<string> input, bool keepOld)
{
	ofstream stream;
	if (!keepOld) {
		stream.open(file, ios::trunc);
	}
	else {
		stream.open(file, ios::ios_base::app);
	}

	for (int i = 0; i < input.size(); i++) {
		stream << input.at(i) << endl;
	}
}

vector<string> FileManager::read()
{
	ifstream stream;
	stream.open(file);

	vector<string> answer;
	string line;
	while (getline(stream, line)) {
		answer.push_back(line);
	}
	return answer;
}