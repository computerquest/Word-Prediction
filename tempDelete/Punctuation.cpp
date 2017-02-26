#include "Punctuation.h";

string punctuationToString(Punctuation p)
{
	if (p == Punctuation::PARENTHESESL) {
		return "(";
	}
	else if (p == UNDEFINED) {
		return "udefined";
	}
	else if (p == Punctuation::PARENTHESESR) {
		return ")";
	}
	else if (p == Punctuation::PERIOD) {
		return ".";
	}
	else if (p == Punctuation::COMMA) {
		return ",";
	}
	else if (p == Punctuation::SEMICOLON) {
		return ";";
	}
	else if (p == Punctuation::QMARK) {
		return "?";
	}
	else if (p == EPOINT) {
		return "!";
	}
	else if (p == ANGLEBR) {
		return ">";
	}
	else if (p == ANGLEBL) {
		return "<";
	}
	else if (p == COLON) {
		return ":";
	}
	else if (p == BRACEL) {
		return "{";
	}
	else if (p == BRACER) {
		return "}";
	}
	else if (p == DASH) {
		return "-";
	}
	else if (p == UNDERSCORE) {
		return "_";
	}
	else if (p == SQUAREBR) {
		return "]";
	}
	else if (p == SQUAREBL) {
		return "[";
	}
	else if (p == APOSTROPHE) {
		return "'";
	}
	else if (p == SLASH) {
		return "/";
	}
	else if (p == ELLIPSES) {
		return ". . .";
	}
}

Punctuation stringToPunctuation(string p)
{
	if (p == "(") {
		return Punctuation::PARENTHESESL;
	}
	else if (p == ")") {
		return Punctuation::PARENTHESESR;
	}
	else if (p == ".") {
		return Punctuation::PERIOD;
	}
	else if (p == ",") {
		return Punctuation::COMMA;
	}
	else if (p == ";") {
		return Punctuation::SEMICOLON;
	}
	else if (p == "?") {
		return Punctuation::QMARK;
	}
	else if (p == "!") {
		return EPOINT;
	}
	else if (p == ">") {
		return ANGLEBR;
	}
	else if (p == "<") {
		return ANGLEBL;
	}
	else if (p == ":") {
		return COLON;
	}
	else if (p == "{") {
		return BRACEL;
	}
	else if (p == "}") {
		return BRACER;
	}
	else if (p == "-") {
		return DASH;
	}
	else if (p == "_") {
		return UNDERSCORE;
	}
	else if (p == "]") {
		return SQUAREBR;
	}
	else if (p == "[") {
		return SQUAREBL;
	}
	else if (p == "'") {
		return APOSTROPHE;
	}
	else if (p == "/") {
		return SLASH;
	}
	else if (p == ". . .") {
		return ELLIPSES;
	}
	else {
		return UNDEFINED;
	}
}