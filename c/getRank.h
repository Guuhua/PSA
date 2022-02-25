#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAP 4
#define NUM 128

// Return all letters' ranks (sampled)
int **getRank(char*);

// Return letter's rank given char and location
int rank(int**, char*, char, int);