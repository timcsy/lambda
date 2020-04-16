#include "lambda.h"
using namespace std;

int main(int argc, char * argv[]) {
	type_t input_form = LAMBDA;
	type_t output_form = INDEXED;
	if (argc > 1) {
		if (strcmp(argv[1], "-l") == 0) {
			input_form =  LAMBDA;
		} else if (strcmp(argv[1], "-i") == 0) {
			input_form = INDEXED;
		} else if (strcmp(argv[1], "-bt") == 0) {
			input_form = BLC_TEXT;
		}
	}
	if (argc > 2) {
		if (strcmp(argv[2], "-l") == 0) {
			output_form =  LAMBDA;
		} else if (strcmp(argv[2], "-i") == 0) {
			output_form = INDEXED;
		} else if (strcmp(argv[2], "-bt") == 0) {
			output_form = BLC_TEXT;
		}
	}

	Lambda lambda(input_form, output_form, cin, cout);
	lambda.parse();
	lambda.reduction();
	lambda.convert();

	return 0;
}