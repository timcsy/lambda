#ifndef LAMBDA_H
#define LAMBDA_H

#include "parser.h"

class Lambda {
public:
	Lambda(type_t input_form = LAMBDA, type_t output_form = INDEXED, istream & input = cin, ostream & output = cout):
		input_form(input_form), output_form(output_form), input(input), output(output), var_num(0) {
		switch (input_form) {
			case LAMBDA: parser = new LambdaParser(input); break;
			case INDEXED: parser = new IndexedParser(input); break;
			case BLC_TEXT: parser = new BLCTextParser(input); break;
			default: break;
		}
	}
	~Lambda() {
		if (parser) { delete parser; }
		// TODO: delete Nodes
	}

	void parse() {
		expr = parser->expr();
	}

	void reduction() {

	}

	void convert() {
		convert(expr);
		output << endl;
	}

	int var_index(Node * node, Node * scope) {
		int n = 1;
		while (node->in != scope) {
			n++;
			if (scope) { scope = scope->in; }
			else { node = node->in; }
		}
		return n;
	}

	int var_name(Node * node) { // node is the pointer of abstraction
		if (variables.count(node) == 0) { // if new variable
			variables[node] = ++var_num;
		}
		return variables[node];
	}
	
	void convert(Node * node, Node * scope = NULL) {
		if (node != NULL) {
			if (node->out) {
				if (node->in) {
					// application
					if (output_form == LAMBDA || output_form == INDEXED) {
						output << "( ";
					} else if (output_form == BLC_TEXT) {
						output << "01";
					}
					convert(node->out, scope);
					convert(node->in, scope);
					if (output_form == LAMBDA || output_form == INDEXED) {
						output << ") ";
					}
				} else {
					// abstraction
					if (output_form == LAMBDA) {
						output << "^ ";
						output << var_name(node) << " . ";
					} else if (output_form == INDEXED) {
						output << "^ ";
					} else if (output_form == BLC_TEXT) {
						output << "00";
					}
					node->in = scope;
					convert(node->out, node);
					node->in = NULL;
				}
			} else {
				// variable
				if (output_form == LAMBDA) {
					output << var_name(node->in) << " ";
				} else if (output_form == INDEXED) {
					output << var_index(node, scope) << " ";
				} else if (output_form == BLC_TEXT) {
					int n = var_index(node, scope);
					for (int i = 0; i < n; i++) { output << "1"; }
					output << "0";
				}
			}
		}
	}

protected:
	Parser * parser;
	Node * expr;
	type_t input_form;
	type_t output_form;
	istream & input;
	ostream & output;
	map<Node *, int> variables;
	int var_num;
};

#endif
