#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>

typedef unsigned char BYTE;

#define MS      7
#define MIS     -3
#define OPENGAP 15
#define EXGAP   2
#define NEGINF 0xc0c0c0c0
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MATCH(a, b) (((a) == (b)) ? MS : MIS)
#define MAX3(a, b, c) (MAX(MAX((a), (b)), (c)))
#define ChooseMAX(p0, p1, p2) (((p0) >= (p1)) ? ((p0) >= (p2) ? 0 : 2) : ((p1) >= (p2) ? 1 : 2))
#define ChooseONE(p0, p1, p2) (((p0) >= (p1)) ? ((p0) >= (p2) ? 16 : 48) : ((p1) >= (p2) ? 32 : 48))
#define Inside(i, j, k, diff) (((-(k)) <= ((j) - (i))) && (((j) - (i)) <= ((k) + (diff))))


int parse(BYTE, int);

int freemem(int**, int);

void Init2(int***, int, int, int);

void Init(int***, BYTE***, int, int, int);

char **align(char*, char*);

char **TraceBack(BYTE**, char*, char*, int, BYTE);

char **PSA(char*, char*);