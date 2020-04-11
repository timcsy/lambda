#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

void error(string msg) {
	cout << msg.c_str() << endl;
	exit(0);
}

typedef enum { EXPR, VAR, ABS, APP, ITEM, LAMBDA, DOT, LPAREN, RPAREN, END } type_t;

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
			case LAMBDA: s_type = "^"; break;
			case DOT: s_type = "."; break;
			case LPAREN: s_type = "("; break;
			case RPAREN: s_type = ")"; break;
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
	Lexer(istream & input = cin): n(0), input(input), line(1), offset(0) {}
	Token next() {
		char c;
		while ((c = input.get()) != EOF) {
			offset++;
			if (isspace(c)) {
				if (c == '\n') { line++; offset = 0; }
				continue;
			}
			if (c == '^') {
				return Token(LAMBDA);
			} else if (c == '.') {
				return Token(DOT);
			} else if (c == '(') {
				return Token(LPAREN);
			} else if (c == ')') {
				return Token(RPAREN);
			} else {
				// mapping variable to number
				string s = "";
				while (c != EOF && !isspace(c) && c != '^' && c != '.' && c != '(' && c != ')') {
					if (s != "") { c = input.get(); offset++; }
					s += c;
					c = input.peek();
				};
				if (variables.count(s) == 0) {
					variables[s] = n++;
				}
				return Token(VAR, variables[s]);
			}
		}
		return Token(END);
	}
	int getVariableNumber() { return n; }
	int getLine() { return line; }
	int getOffset() { return offset; }
	string getPosition() {
		stringstream ss;
		ss << "line: " << line << ", offset: " << offset;
		return ss.str();
	}
private:
	map<string, int> variables;
	int n;
	istream & input;
	int line;
	int offset;
};

class AST {
public:
	AST(type_t type, ostream & output = cout): type(type), output(output), bind(0), scope(0) {}
	virtual ~AST() {}
	type_t type;
	int bind;
	int scope;
	virtual void visit() = 0;
	ostream & output;
};

class Variable: public AST {
public:
	Variable(int var, ostream & output = cout): var(var), AST(VAR, output) {}
	~Variable() {}
	int var;
	void visit() {
		#ifdef DEBUG
		cout << "Variable: ";
		cout << var << endl;
		#endif
		if (bind < 1) {
			error("Sementic Error: There exists some free variables, use abstraction first.");
		} else {
			output << scope - bind + 1 << " ";
		}
	}
};

class Abstraction: public AST {
public:
	Abstraction(Variable * var, AST * expr, ostream & output = cout): var(var), expr(expr), AST(ABS, output) {}
	~Abstraction() { delete var; delete expr; }
	Variable * var;
	AST * expr;
	void visit() {
		#ifdef DEBUG
		cout << "Abstraction:" << endl;
		#endif
		output << "^ ";
		bind++;
		scope++;
		int var_bind = var->bind;
		int var_scope = var->scope;
		var->bind = bind;
		var->scope = scope;
		expr->bind = bind;
		expr->scope = scope;
		expr->visit();
		var->bind = var_bind;
		var->scope = var_scope;
	}
};

class Application: public AST {
public:
	Application(vector<AST *> items, ostream & output = cout): items(items), AST(APP, output) {}
	~Application() { for (int i = 0; i < items.size(); i++) { delete (items[i]); } items.clear(); }
	vector<AST *> items;
	void visit() {
		#ifdef DEBUG
		cout << "Application:" << endl;
		#endif
		output << "( ";
		for (int i = 0; i < items.size(); i++) {
			if (items[i]->type != VAR) items[i]->bind = bind;
			items[i]->scope = scope;
			items[i]->visit();
		}
		output << ") ";
	}
};

class Parser {
public:
	Parser(istream & input = cin, ostream & output = cout): input(input), output(output), lexer(input) {
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
		// <expr> ::= ^ <var> . <expr> | <app>
		if (current_token.type == LAMBDA) {
			match(LAMBDA);
			int value = current_token.value;
			match(VAR);
			match(DOT);
			if (variables.count(value) == 0) {
				variables[value] = new Variable(value, output);
			}
			return new Abstraction(variables[value], expr(), output);
		} else {
			return app();
		}
	}
	AST * app() {
		// <app> ::= <app> <item> | <item>
		vector<AST *> items;
		AST * node;
		while ((node = item()) != NULL) {
			items.push_back(node);
		}
		return new Application(items, output);
	}
	AST * item() {
		// <item> ::= ( <expr> ) | <var>
		if (current_token.type == LPAREN) {
			match(LPAREN);
			AST * node = expr();
			match(RPAREN);
			return node;
		} else if (current_token.type == VAR) {
			int value = current_token.value;
			match(VAR);
			if (variables.count(value) == 0) {
				variables[value] = new Variable(value, output);
			}
			return variables[value];
		} else {
			return NULL;
		}
	}
	Token getLastToken() {
		return current_token;
	}
private:
	Lexer lexer;
	istream & input;
	ostream & output;
	Token current_token;
	map<int, Variable *> variables;
};

string alpha(istream& input = cin, ostream & output = cout) {
	stringstream ss;
	Parser parser(input, ss);
	AST * node = parser.expr();
	node->visit();
	if (parser.getLastToken().type != END) {
		error("Incomplete intput format, you can try to use parentheses on the outside of the all.");
	}
	return ss.str();
} 

int main(int argc, char * argv[]) {
	cout << alpha() << endl;
	#ifdef DEBUG
	cout << "Successfully parsed!" << endl;
	#endif
	return 0;
}