#pragma once
#include <string>;
using namespace std;

enum Punctuation {
	PERIOD, COMMA, SEMICOLON, QMARK, EPOINT, ANGLEBL, ANGLEBR, COLON, BRACEL, BRACER, DASH, UNDERSCORE, SQUAREBR, SQUAREBL, APOSTROPHE, SLASH, ELLIPSES, PARENTHESESL, PARENTHESESR
};

string punctuationToString(Punctuation p);

Punctuation stringToPunctuation(string character);