#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "node.h"
#include "util.h"
#include <map>

class Parser {
public:
	Parser(Lexer * lexer): free_variable(NULL), lexer(lexer) {
		current_token = lexer->next();
	}
	void match(type_t token_type) {
		if (current_token.type == token_type) {
			current_token = lexer->next();
		} else {
			error(string("Unexpected token: ") + current_token.value + " at " + lexer->getPosition());
		}
	}
	virtual Node * expr() = 0;
	Token getLastToken() {
		return current_token;
	}
protected:
	Lexer * lexer;
	Token current_token;
	map<string, Node *> variables;
	Node * free_variable;
};

class LambdaParser: public Parser {
public:
	LambdaParser(istream & input = cin): Parser(new LambdaLexer(input)) {}
	Node * expr() {
		// <expr> ::= <abs> | <app>
		if (current_token.type == LAMBDA) {
			return abs();
		} else {
			return app();
		}
	}
	Node * item() {
		// <item> ::= ( <expr> ) | <var>
		// free variable is Node(NULL, NULL)
		if (current_token.type == LPAREN) {
			match(LPAREN);
			Node * node = expr();
			match(RPAREN);
			return node;
		} else if (current_token.type == VAR) {
			string value = current_token.value;
			match(VAR);
			if (variables.count(value) == 0) { // if new variable
				// free variable
				variables[value] = new Node(free_variable, NULL);
				free_variable = variables[value];
			}
			return variables[value];
		} else {
			return NULL;
		}
	}
	Node * abs() {
		// <abs> ::= ^ <var> . <expr>
		// ^x.M => Node(NULL, M), M has x which is Node(^x.M, NULL)
		match(LAMBDA);
		string value = current_token.value;
		match(VAR);
		match(DOT);
		// save the last variable with the same variable value
		Node * last_var = NULL;
		if (variables.count(value) != 0) {
			last_var = variables[value];
		}
		// create a new scope
		variables[value] = new Node(NULL, NULL);
		Node * abstraction = new Node(NULL, expr());
		// bind the variable with the new abstraction
		variables[value]->in = abstraction;
		// restore the last variable
		if (last_var) { variables[value] = last_var; }
		return abstraction;
	}
	Node * app() {
		// <app> ::= <item> | <app> (<abs> | <item>)
		// (M N) => Node(N, M)
		Node * items = item();
		Node * node;
		if (current_token.type == LAMBDA) {
			node = abs();
		} else {
			node = item();
		}
		while (node != NULL) {
			items = new Node(node, items);
			if (current_token.type == LAMBDA) {
				node = abs();
			} else {
				node = item();
			}
		}
		return items;
	}
};

// TODO: the variable need to change (from integer to address)
class IndexedParser: public Parser {
public:
	IndexedParser(istream & input = cin): Parser(new IndexedLexer(input)) {}
	Node * expr() {
		// <expr> ::= <abs> | <app>
		if (current_token.type == LAMBDA) {
			return abs();
		} else {
			return app();
		}
	}
	Node * item() {
		// <item> ::= ( <expr> ) | <var>
		// free variable is Node(NULL, NULL)
		if (current_token.type == LPAREN) {
			match(LPAREN);
			Node * node = expr();
			match(RPAREN);
			return node;
		} else if (current_token.type == VAR) {
			string value = current_token.value;
			match(VAR);
			if (variables.count(value) == 0) { // if new variable
				// free variable
				variables[value] = new Node(free_variable, NULL);
				free_variable = variables[value];
			}
			return variables[value];
		} else {
			return NULL;
		}
	}
	Node * abs() {
		// <abs> ::= ^ <expr>
		// ^x.M => Node(NULL, M), M has x which is Node(^x.M, NULL)
		match(LAMBDA);
		string value = "1";
		Node * last_var = NULL;
		if (variables.count(value) != 0) {
			last_var = variables[value];
		}
		// create a new scope
		variables[value] = new Node(NULL, NULL);
		Node * abstraction = new Node(NULL, expr());
		// bind the variable with the new abstraction
		variables[value]->in = abstraction;
		// restore the last variable
		if (last_var) { variables[value] = last_var; }
		return abstraction;
	}
	Node * app() {
		// <app> ::= <item> | <app> (<abs> | <item>)
		// (M N) => Node(N, M)
		Node * items = item();
		Node * node;
		if (current_token.type == LAMBDA) {
			node = abs();
		} else {
			node = item();
		}
		while (node != NULL) {
			items = new Node(node, items);
			if (current_token.type == LAMBDA) {
				node = abs();
			} else {
				node = item();
			}
		}
		return items;
	}
};

#endif
