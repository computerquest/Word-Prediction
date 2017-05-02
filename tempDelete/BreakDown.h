#pragma once
#include <string>
#include <vector>
#include "LinkedList.h"
#include "POS.h"
#include <iostream>
#include "Punctuation.h"
using namespace std;

vector<string> breakDownV(string phrase);
vector<string> breakDownV(string phrase, vector<char> delimeter);
int stringToInt(string s);
string trimWhite(string s);

LinkedList<POS, int> posToBlocks(vector<string> wordStrings, vector<POS> blocks);

bool binaryExist(vector<int> input, int target);