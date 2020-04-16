#ifndef NODE_H
#define NODE_H

struct Node {
	Node * in;
	Node * out;

	Node(Node * in, Node * out): in(in), out(out) {}
	~Node() {
		if (in) { delete in; }
		if (out) { delete out; }
	}
};

#endif
