#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <iostream>
#include <sstream>

class Lexer {
public:
	Lexer(istream & input = cin): input(input), line(1), offset(0) {}
	virtual ~Lexer() {}
	virtual Token next() = 0;
	int getLine() { return line; }
	int getOffset() { return offset; }
	string getPosition() {
		stringstream ss;
		ss << "line: " << line << ", offset: " << offset;
		return ss.str();
	}

protected:
	istream & input;
	int line;
	int offset;
};


class LambdaLexer: public Lexer {
public:
	LambdaLexer(istream & input = cin): Lexer(input) {}
	Token next() {
		char c;
		while ((c = input.get()) != EOF) {
			offset++;
			if (isspace(c)) {
				if (c == '\n') { line++; offset = 0; }
				continue;
			}
			if (c == '^') {
				return Token(LAMBDA, "^");
			} else if (c == '.') {
				return Token(DOT, ".");
			} else if (c == '(') {
				return Token(LPAREN, "(");
			} else if (c == ')') {
				return Token(RPAREN, ")");
			} else {
				// mapping variable to number
				string s = "";
				while (c != EOF && !isspace(c) && c != '^' && c != '.' && c != '(' && c != ')') {
					if (s != "") { c = input.get(); offset++; }
					s += c;
					c = input.peek();
				};
				return Token(VAR, s);
			}
		}
		return Token(END, "EOF");
	}
};


class IndexedLexer: public Lexer {
public:
	IndexedLexer(istream & input = cin): Lexer(input) {}
	Token next() {
		char c;
		while ((c = input.get()) != EOF) {
			offset++;
			if (isspace(c)) {
				if (c == '\n') { line++; offset = 0; }
				continue;
			}
			if (c == '^') {
				return Token(LAMBDA, "^");
			} else if (c == '(') {
				return Token(LPAREN, "(");
			} else if (c == ')') {
				return Token(RPAREN, ")");
			} else {
				// mapping variable to number
				string s = "";
				while (isdigit(c)) {
					if (s != "") { c = input.get(); offset++; }
					s += c;
					c = input.peek();
				};
				return Token(VAR, s, stoi(s));
			}
		}
		return Token(END, "EOF");
	}
};


class BLCTextLexer: public Lexer {
public:
	BLCTextLexer(istream & input = cin): Lexer(input) {}
	Token next() {
		char c, status = 0;
		int n = 0;
		while ((c = input.get()) != EOF) {
			offset++;
			if (isspace(c)) {
				if (c == '\n') { line++; offset = 0; }
				continue;
			}
			if (c == '0') {
				switch (status) {
					case ZERO: status = ABS; break;
					case ONE: status = VAR; break;
					default: status = ZERO; n = 0; break;
				}
			} else if (c == '1') {
				switch (status) {
					case ZERO: status = APP; break;
					default: status = ONE; n++; break;
				}
			}
			if (status == VAR) {
				string s = "";
				for (int i = 0; i < n; i++) { s += "1"; }
				s += "0";
				return Token(VAR, s, n);
			} else if (status ==  ABS) {
				return Token(ABS, "00");
			} else if (status == APP) {
				return Token(APP, "01");
			}
		}
		return Token(END, "EOF");
	}
};

#endif
