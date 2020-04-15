#include "substitution.h"
using namespace std;

int main(int argc, char * argv[]) {
	type_t input_form = LAMBDA;
	type_t output_form = INDEXED;
	if (argc > 1) {
		if (strcmp(argv[1], "-i") == 0) {
			input_form =  INDEXED;
		} else if (strcmp(argv[1], "-b") == 0) {
			input_form = BINARY_TEXT;
		} else if (strcmp(argv[1], "-l") == 0) {
			input_form = LAMBDA;
		}
	}
	if (argc > 2) {
		if (strcmp(argv[2], "-i") == 0) {
			output_form =  INDEXED;
		} else if (strcmp(argv[2], "-b") == 0) {
			output_form = BINARY_TEXT;
		} else if (strcmp(argv[2], "-l") == 0) {
			output_form = LAMBDA;
		}
	}
	cout << alpha(input_form, output_form) << endl;
	return 0;
}