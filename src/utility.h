#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <vector>
using namespace std;

#ifndef UTILITY
#define UTILITY

#ifndef EFF_ZERO
#define EFF_ZERO 1e-10
#endif

#ifndef PI
#define PI 3.14159265358979
#endif

double get_rand(double r_min, double r_max);

double min(double x, double y);
int min(int x, int y);

double max(double x, double y);
int max(int x, int y);

int string_startswith(char* str, const char* s);
int string_tolowercase(char* str);
int string_remove_whitespace(char* str);

int isAlphaNumeric(char c, bool start);

unsigned int nchoosek(unsigned int n, unsigned int k);
int** combntns(int* set, int length, int subset);

#endif
