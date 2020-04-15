#ifndef TOKEN_H
#define TOKEN_H

#include "type.h"
#include <string>
using namespace std;

struct Token {
	type_t type;
	string value;
	Token(type_t type = NONE, string value = ""): type(type), value(value) {}
};

#endif
