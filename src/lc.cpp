#include "lambda.h"
#include <fstream>
using namespace std;

int main(int argc, char * argv[]) {
	type_t input_form = LAMBDA;
	type_t output_form = INDEXED;
	istream * input = &cin;
	ostream * output = &cout;
	bool is_input = false, is_output = false;
	char * form;
	char ** in_files;
	char ** out_files;
	int in_num = 0, out_num = 0;
	for (int i = 1; i < argc; i++) {
		if (strncmp(argv[i], "--i", 3) == 0) {
			is_input = true; is_output = false;
			if (strncmp(argv[i] + 3, "=", 1) == 0) {
				form = argv[i] + 4;
			}
		} else if (strncmp(argv[i], "--o", 3) == 0) {
			is_input = false; is_output = true;
			if (strncmp(argv[i] + 3, "=", 1) == 0) {
				form = argv[i] + 4;
			}
		} else {
			if (is_input) {
				if (!in_num) in_files = argv + i;
				in_num++;
			} else if (is_output) {
				if (!out_num) out_files = argv + i;
				out_num++;
			}
		}
		if (form) {
			type_t f;
			if (strcmp(form, "l") == 0) { f = LAMBDA; }
			else if (strcmp(form, "i") == 0) { f = INDEXED; }
			else if (strcmp(form, "bt") == 0) { f = BLC_TEXT; }
			else if (strcmp(form, "b") == 0) { f = BLC; }
			if (is_input) input_form = f;
			else if (is_output) output_form = f;
			form = NULL;
		}
	}

	if (in_num > 0) {
		input = new fstream(in_files[0], ios::in | ios::binary);
	}
	if (out_num > 0) {
		output = new fstream(out_files[0], ios::out | ios::binary);
	}
	Lambda lambda(input_form, output_form, *input, *output);
	lambda.parse();
	lambda.reduction();
	lambda.convert();

	if (in_num > 0) {
		((fstream *)input)->close();
		delete input;
	}
	if (out_num > 0) {
		((fstream *)output)->close();
		delete output;
	}

	return 0;
}