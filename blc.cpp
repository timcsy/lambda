#include <string>
#include <iostream>
#include <sstream>
#include <map>
using namespace std;

void error(string msg) {
	cout << msg.c_str() << endl;
	exit(0);
}

typedef enum { EXPR, VAR, ABS, APP, END } type_t;

struct Token {
	type_t type;
	int value;
	Token() {}
	Token(type_t type): type(type) {}
	Token(type_t type, int value): type(type), value(value) {}
	string to_string() {
		string s_type;
		switch (type) {
			case VAR: s_type = "variable"; break;
			case ABS: s_type = "00"; break;
			case APP: s_type = "01"; break;
			case END: s_type = "EOF"; break;
			default: s_type = ""; break;
		}
		stringstream ss;
		ss << value;
		return string("Token ") + s_type + ((type == VAR)? (", Value: " + ss.str()): "");
	}
};

class Lexer {
public:
	Lexer(istream & input = cin): input(input), line(1), offset(0) {}
	Token next() {
		char c;
		while ((c = input.get()) != EOF) {
			offset++;
			if (isspace(c)) {
				if (c == '\n') { line++; offset = 0; }
				continue;
			}
			if (c == '0') {
				c = input.get(); offset++;
				if (c == '0') {
					return Token(ABS);
				} else if (c == '1') {
					return Token(APP);
				} else {
					error(string("Unexpected character: ") + c + " at " + getPosition());
				}
			} else {
				// variable
				int n = 0;
				while (c == '1') {
					if (n != 0) { c = input.get(); offset++; }
					n++;
					c = input.peek();
				}
				if (c == '0') {
					c = input.get(); offset++;
					return Token(VAR, n);
				} else {
					error(string("Unexpected character: ") + c + " at " + getPosition());
				}
			}
		}
		return Token(END);
	}
	int getLine() { return line; }
	int getOffset() { return offset; }
	string getPosition() {
		stringstream ss;
		ss << "line: " << line << ", offset: " << offset;
		return ss.str();
	}
private:
	istream & input;
	int line;
	int offset;
};

class AST {
public:
	AST(type_t type): type(type) {}
	virtual ~AST() {}
	type_t type;
	virtual void visit() = 0;
};

class Variable: public AST {
public:
	Variable(int var): var(var), AST(VAR) {}
	~Variable() {}
	int var;
	void visit() {
		cout << "Variable: ";
		cout << var << endl;
	}
};

class Abstraction: public AST {
public:
	Abstraction(AST * expr): expr(expr), AST(ABS) {}
	~Abstraction() { delete expr; }
	AST * expr;
	void visit() {
		cout << "Abstraction:" << endl;
		expr->visit();
	}
};

class Application: public AST {
public:
	Application(): next(NULL), AST(APP) {}
	Application(AST * item, AST * next): item(item), next(next), AST(APP) {}
	~Application() { delete item; delete next; }
	AST * item;
	AST * next;
	void visit() {
		cout << "Application:" << endl;
		item->visit();
		if (next != NULL) {
			next->visit();
		}
	}
};

class Parser {
public:
	Parser(istream & input = cin): input(input), lexer(input) {
		current_token = lexer.next();
	}
	void match(type_t token_type) {
		if (current_token.type == token_type) {
			current_token = lexer.next();
		} else {
			error(string("Unexpected token: ") + current_token.to_string() + " at " + lexer.getPosition());
		}
	}
	AST * expr() {
		// <expr> ::= 00 <expr> | 01 <expr> <expr> | 1+0
		if (current_token.type == ABS) {
			match(ABS);
			return new Abstraction(expr());
		} else if (current_token.type == APP) {
			match(APP);
			AST * node = expr();
			if (node == NULL) {
				return NULL;
			} else {
				return new Application(node, expr());
			}
		} else if (current_token.type == VAR) {
			int value = current_token.value;
			match(VAR);
			return new Variable(value);
		} else {
			error(string("Unexpected token: ") + current_token.to_string() + " at " + lexer.getPosition() );
			return NULL;
		}
	}
private:
	Lexer lexer;
	istream & input;
	Token current_token;
};

int main(int argc, char * argv[]) {
	Parser parser;
	AST * node = parser.expr();
	node->visit();
	cout << "Successfully parsed!" << endl;
	return 0;
}