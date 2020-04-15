#ifndef UTIL_H
#define UTIL_H

#include <iostream>
using namespace std;

void error(string msg) {
	cout << msg.c_str() << endl;
	exit(0);
}

#endif
