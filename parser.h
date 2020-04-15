#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "node.h"
#include <map>

class Parser {
public:
	Parser(Lexer * lexer): lexer(lexer) {
		current_token = lexer->next();
	}
	void match(type_t token_type) {
		if (current_token.type == token_type) {
			current_token = lexer->next();
		} else {
			error(string("Unexpected token: ") + current_token.value + " at " + lexer->getPosition());
		}
	}
	virtual AST * expr(Variable * var = new Variable()) = 0;
	Token getLastToken() {
		return current_token;
	}
protected:
	Lexer * lexer;
	Token current_token;
	map<string, Variable *> variables;
};


class LambdaParser: public Parser {
public:
	LambdaParser(istream & input = cin): Parser(new LambdaLexer(input)) {}
	AST * expr(Variable * var = new Variable()) {
		// <expr> ::= ^ <var> . <expr> | <app>
		if (current_token.type == LAMBDA) {
			match(LAMBDA);
			string value = current_token.value;
			match(VAR);
			match(DOT);
			var = new Variable(value, var->scope + 1);
			return new Abstraction(var, expr(var));
		} else {
			return app(var);
		}
	}
	AST * app(Variable * var) {
		// <app> ::= <app> <item> | <item>
		vector<AST *> items;
		AST * node;
		while ((node = item(var)) != NULL) {
			items.push_back(node);
		}
		return new Application(items);
	}
	AST * item(Variable * var) {
		// <item> ::= ( <expr> ) | <var>
		if (current_token.type == LPAREN) {
			match(LPAREN);
			AST * node = expr(var);
			match(RPAREN);
			node->precedence = true;
			return node;
		} else if (current_token.type == VAR) {
			string value = current_token.value;
			match(VAR);
			if (var->value == value) {
				return var;
			}
			// TODO something with scope and binding
			else return NULL;
		} else {
			return NULL;
		}
	}
};


class IndexedParser: public Parser {
public:
	IndexedParser(istream & input = cin): Parser(new IndexedLexer(input)) {}
	AST * expr() {
		// <expr> ::= ^ <expr> | <app>
		if (current_token.type == LAMBDA) {
			match(LAMBDA);
			return new Abstraction(new Variable(), expr());
		} else {
			return app();
		}
		// <expr> ::= ^ <var> . <expr> | <app>
		if (current_token.type == LAMBDA) {
			match(LAMBDA);
			string value = current_token.value;
			match(VAR);
			match(DOT);
			if (variables.count(value) == 0) {
				variables[value] = new Variable();
			}
			return new Abstraction(variables[value], expr());
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
		return new Application(items);
	}
	AST * item() {
		// <item> ::= ( <expr> ) | <var>
		if (current_token.type == LPAREN) {
			match(LPAREN);
			AST * node = expr();
			match(RPAREN);
			node->precedence = true;
			return node;
		} else if (current_token.type == VAR) {
			string value = current_token.value;
			match(VAR);
			if (variables.count(value) == 0) {
				variables[value] = new Variable();
			}
			return variables[value];
		} else {
			return NULL;
		}
	}
};


class Parser2 {
public:
	Parser2(Lexer * lexer): lexer(lexer) {
		current_token = lexer->next();
	}
	void match(type_t token_type) {
		if (current_token.type == token_type) {
			current_token = lexer->next();
		} else {
			error(string("Unexpected token: ") + current_token.value + " at " + lexer->getPosition());
		}
	}
	virtual Node * expr(Node * scope = NULL) = 0;
	Token getLastToken() {
		return current_token;
	}
protected:
	Lexer * lexer;
	Token current_token;
	map<string, Variable *> variables;
};

class NodeParser: public Parser2 {
public:
	NodeParser(istream & input = cin): free_variable(NULL), Parser2(new LambdaLexer(input)) {}
	Node * expr(Node * scope = NULL) {
		// <expr> ::= ^ <var> . <expr> | <app>
		// ^x.M => Node(NULL, M), M has x which is Node(^x.M, NULL)
		if (current_token.type == LAMBDA) {
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
			Node * abstraction = new Node(NULL, NULL);
			variables[value] = new Node(NULL, NULL);
			Node * body = expr(abstraction); // with new scope
			// bind the variable with the new abstraction
			variables[value]->in = abstraction;
			// bind the abstraction with body
			abstraction->out = body;
			// restore the last variable
			if (last_var) { variables[value] = last_var; }
			return abstraction;
		} else {
			return app(scope);
		}
	}
	Node * app(Node * scope) {
		// <app> ::= <app> <item> | <item>
		// (M N) => Node(N, M)
		Node * items = item(scope); // problem
		Node * node;
		while ((node = item(scope)) != NULL) {
			items = new Node(node, items); // problem with var only
		}
		return items;
	}
	Node * item(Node * scope) {
		// <item> ::= ( <expr> ) | <var>
		// free variable is Node(NULL, NULL)
		if (current_token.type == LPAREN) {
			match(LPAREN);
			Node * node = expr(scope);
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
private:
	map<string, Node *> variables;
	Node * free_variable;
};

#endif
