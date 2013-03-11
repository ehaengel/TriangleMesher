#include "utility.h"

double get_rand(double r_min, double r_max) {
	return r_min + (r_max - r_min) * (double(rand()) / RAND_MAX);
}

double min(double x, double y) {
	if(x < y)
		return x;
	return y;
}
int min(int x, int y) {
	if(x < y)
		return x;
	return y;
}

double max(double x, double y) {
	if(x > y)
		return x;
	return y;
}
int max(int x, int y) {
	if(x > y)
		return x;
	return y;
}

int string_startswith(char* str, const char* s) {
	return strncmp(str, s, strlen(s));
}

int string_tolowercase(char* str) {
	char* p = str;
	while(*p)
		*(p++) = tolower(*p);

	return true;
}

int string_remove_whitespace(char* str) {
	char* p = str;
	char* q = str;
	while(*p) {
		if(*p != ' ' && *p != '\n')
			*(q++) = *p;
		p++;
	}
	*q = '\0';
	return true;
}

int isAlphaNumeric(char c, bool start) {
	if(start)	return (isdigit(c) || c=='.' || c=='+' || c=='-');
	else		return (isdigit(c) || c=='.' || c=='+' || c=='-' || c=='e' || c=='E');
}

unsigned int nchoosek(unsigned int n, unsigned int k) {
	double value = 1;
	for(int i=1; i<=k; i++)
		value *= double(n - (k - i)) / double(i);

	return (unsigned int)(value);
}

int** combntns(int* set, int length, int subset) {
	int* set_buf = set;
	int** output = new int*[nchoosek(length, subset)];
	for(int i=0; i<nchoosek(length, subset); i++)
		output[i] = new int[subset];

	int row_index = 0;

	while(length >= subset && subset >= 1) {
		//base case if we only want combinations involving one element
		if(subset == 1) {
			for(int k=0; k<length; k++)
				output[k][0] = set_buf[k];
			return output;
		}

		int len_cur = nchoosek(length-1, subset-1);
		int start_row = row_index;
		int end_row = row_index + len_cur;
		for(int k=start_row; k<end_row; k++)
			output[k][0] = set_buf[0];

		set_buf = set_buf+1;
		length = length-1;
		int** temp = combntns(set_buf, length, subset-1);

		for(int k=start_row; k<end_row; k++)
			for(int i=1; i<subset; i++)
				output[k][i] = temp[k-start_row][i-1];
		delete temp;

		row_index = end_row;
	}
	return output;
}
