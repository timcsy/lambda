#ifndef NODE_H
#define NODE_H

int NUMBER_OF_NODES = 0;

/**
 * in != NULL, out != NULL => APP (out in)
 * in == NULL, out != NULL => ABS ^out
 * out == NULL => VAR in(point to scope)
 */
struct Node {
	int addr;
	Node * in;
	Node * out;

	Node(Node * in, Node * out): in(in), out(out) {
		addr = ++NUMBER_OF_NODES;
	}
};

// void print_node(Node * node) {
// 	cout << "Node: ";
// 	if (node == NULL) cout << "0" << endl;
// 	else {
// 		cout << node->addr << endl;
// 		if (node->in) cout << " IN: " << node->in->addr << endl;
// 		else cout << " IN: 0" << endl;
// 		if (node->out) cout << "OUT: " << node->out->addr << endl;
// 		else cout << "OUT: 0" << endl;
// 		cout << endl;
// 		if (node->in && node->out) print_node(node->in);
// 		if (node->out) print_node(node->out);
// 	}
// }

#endif
