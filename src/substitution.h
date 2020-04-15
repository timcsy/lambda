#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include "parser.h"

void visit(Node * node, ostream & output, type_t output_form = INDEXED, Node * scope = NULL) {
	if (node != NULL) {
		if (node->out) {
			if (node->in) {
				// application
				output << "( ";
				visit(node->out, output, output_form, scope);
				visit(node->in, output, output_form, scope);
				output << ") ";
			} else {
				// abstraction
				output << "^ ";
				node->in = scope;
				visit(node->out, output, output_form, node);
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

string alpha(type_t input_form = LAMBDA, type_t output_form = INDEXED, istream & input = cin, ostream & output = cout) {
	stringstream ss;
	Parser * parser;
	switch (input_form) {
		case LAMBDA: parser = new LambdaParser(input); break;
		case INDEXED: parser = new IndexedParser(input); break;
		default: break;
	}
	Node * node = parser->expr();
	visit(node, ss, output_form);
	if (parser->getLastToken().type != END) {
		error("Incomplete intput format, you can try to use parentheses on the outside of the all.");
	}
	return ss.str();
}

#endif
