#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

struct snakeDot {
	int x;
	int y;
	int vector;
	snakeDot* next;
};

struct Food {
	int x;
	int y;
};