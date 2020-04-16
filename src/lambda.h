#ifndef LAMBDA_H
#define LAMBDA_H

#include "parser.h"

class Lambda {
public:
	Lambda(type_t input_form = LAMBDA, type_t output_form = INDEXED, istream & input = cin, ostream & output = cout):
		input_form(input_form), output_form(output_form), input(input), output(output) {
		switch (input_form) {
			case LAMBDA: parser = new LambdaParser(input); break;
			case INDEXED: parser = new IndexedParser(input); break;
			case BLC_TEXT: parser = new BLCTextParser(input); break;
			default: break;
		}
	}
	~Lambda() {
		if (parser) { delete parser; }
		if (expr) { delete expr; }
	}

	void parse() {
		expr = parser->expr();
	}

	void reduction() {
		
	}

	void convert() {
		convert(expr);
	}
	
	void convert(Node * node, Node * scope = NULL) {
		if (node != NULL) {
			if (node->out) {
				if (node->in) {
					// application
					output << "( ";
					convert(node->out, scope);
					convert(node->in, scope);
					output << ") ";
				} else {
					// abstraction
					output << "^ ";
					node->in = scope;
					convert(node->out, node);
					node->in = NULL;
				}
			} else {
				// variable
				int n = 1;
				while (node->in != scope) {
					n++;
					if (scope) { scope = scope->in; }
					else { node = node->in; }
				}
				output << n << " ";
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
};

#endif
