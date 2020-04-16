#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "node.h"
#include "util.h"
#include <map>
#include <deque>

class Parser {
public:
	Parser(Lexer * lexer): free_variable(NULL), lexer(lexer) {
		current_token = lexer->next();
	}
	virtual ~Parser() { if (lexer) { delete lexer; } }
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
protected:
	map<string, Node *> variables;
};


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
			int index = current_token.index;
			match(VAR);
			while (index > scope.size()) { // if new variable
				// free variable
				scope.push_back(new Node(free_variable, NULL));
				free_variable = scope.back();
			}
			return scope[index - 1];
		} else {
			return NULL;
		}
	}
	Node * abs() {
		// <abs> ::= ^ <expr>
		// ^x.M => Node(NULL, M), M has x which is Node(^x.M, NULL)
		match(LAMBDA);
		// create a new scope
		scope.push_front(new Node(NULL, NULL));
		Node * abstraction = new Node(NULL, expr());
		// bind the variable with the new abstraction
		scope.front()->in = abstraction;
		// restore the last scope
		scope.pop_front();
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
protected:
	deque<Node *> scope;
};


class BLCTextParser: public Parser {
public:
	BLCTextParser(istream & input = cin): Parser(new BLCTextLexer(input)) {}
	Node * expr() {
		// <expr> ::= 00 <expr> | 01 <expr> <expr> | 1+0
		if (current_token.type == VAR) {
			int index = current_token.index;
			match(VAR);
			while (index > scope.size()) { // if new variable
				// free variable
				scope.push_back(new Node(free_variable, NULL));
				free_variable = scope.back();
			}
			return scope[index - 1];
		} else if (current_token.type == ABS) {
			match(ABS);
			// create a new scope
			scope.push_front(new Node(NULL, NULL));
			Node * abstraction = new Node(NULL, expr());
			// bind the variable with the new abstraction
			scope.front()->in = abstraction;
			// restore the last scope
			scope.pop_front();
			return abstraction;
		} else if (current_token.type == APP){
			match(APP);
			Node * M = expr();
			Node * N = expr();
			return new Node(N, M);
		} else {
			error(string("Unexpected token: ") + current_token.value + " at " + lexer->getPosition() );
			return NULL;
		}
	}
protected:
	deque<Node *> scope;
};

#endif
