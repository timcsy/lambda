#ifndef UTIL_H
#define UTIL_H

#include <iostream>
using namespace std;

void error(string msg) {
	cout << msg.c_str() << endl;
	exit(0);
}

union {
	char c[4];
  unsigned long l;
} endian_test = {{'l', '?', '?', 'b'}};
#define ENDIANNESS ((char)endian_test.l)

#endif
