#ifndef TOKEN_H
#define TOKEN_H

#include "type.h"
#include <string>
using namespace std;

struct Token {
	type_t type;
	string value;
	int index;
	Token(type_t type = NONE, string value = "", int index = 0): type(type), value(value), index(index) {}
};

#endif
