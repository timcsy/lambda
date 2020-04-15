#ifndef AST_H
#define AST_H

#include "util.h"
#include "token.h"
#include <vector>

class AST {
public:
	AST(type_t type): type(type), bind(0), scope(0), precedence(false) {}
	virtual ~AST() {}
	virtual void visit(type_t output_form = INDEXED, ostream & output = cout) = 0;

	type_t type;
	int bind;
	int scope;
	bool precedence;
};

class Variable: public AST {
public:
	Variable(string value = "", int scope = 0): value(value), AST(VAR) {
		this->scope = scope;
	}
	~Variable() {}
	void visit(type_t output_form = INDEXED, ostream & output = cout) {
		if (bind < 1) {
			cout << bind << endl;
			error("Sementic Error: There exists some free variables, use abstraction first.");
		} else {
			if (output_form == INDEXED) { // De Bruijn indexed Lambda Calculus
				output << scope - bind + 1 << " ";
			} else if (output_form == BINARY_TEXT) { // Binary Lambda Calculus
				for (int i = 0; i < scope - bind + 1; i++) { output << "1"; }
				output << "0";
			} else {
				output << scope - bind + 1 << " ";
			}
		}
	}
	string value;
};

class Abstraction: public AST {
public:
	Abstraction(Variable * var, AST * expr): var(var), expr(expr), AST(ABS) {}
	~Abstraction() { delete var; delete expr; }
	Variable * var;
	AST * expr;
	void visit(type_t output_form = INDEXED, ostream & output = cout) { // De Bruijn indexed Lambda Calculus
		if (output_form == INDEXED || output_form == LAMBDA) {
			output << "^ ";
		} else if (output_form == BINARY_TEXT) { // Binary Lambda Calculus
			output << "00";
		}
		bind++; scope++;
		int var_bind = var->bind;
		var->bind = bind;
		var->scope = scope;
		if (output_form == LAMBDA) {
			var->visit(output_form, output);
			output << ". ";
		}
		expr->bind = bind;
		expr->scope = scope;
		expr->visit(output_form, output);
		var->bind = var_bind;
	}
};

class Application: public AST {
public:
	Application(vector<AST *> items): items(items), AST(APP) {}
	~Application() { for (int i = 0; i < items.size(); i++) { delete (items[i]); } items.clear(); }
	void visit(type_t output_form = INDEXED, ostream & output = cout) { // De Bruijn indexed Lambda Calculus
		for (int i = 0; i < items.size(); i++) {
			if (items[i]->precedence) {
				if (output_form == INDEXED) {
					output << "( ";
				} else if (output_form == BINARY_TEXT) { // Binary Lambda Calculus
					output << "01";
				}
			}

			if (items[i]->type != VAR) items[i]->bind = bind;
			items[i]->scope = scope;
			items[i]->visit(output_form, output);
			
			if (items[i]->precedence && output_form == INDEXED) { output << ") "; } // De Bruijn indexed Lambda Calculus
		}
	}
	vector<AST *> items;
};

#endif
