#include "FMindex.h"
#include "PSA_kband.h"

#define DIM 3

int cmp2(const void *a, const void *b);

FM *FMbulid(char *);

int *selectprefixs(FM *fmA, char *p, int length);

int (*findALLCommonStrings(FM *fmA, char *B, int *numCS))[DIM];

int MultiToOne(int *idxs, int length, float offset);

int (*dpSelect(int (*results)[DIM], int *num))[DIM];

int (*trace(int (*results)[DIM], int *p, int *num))[DIM];

int getMAXidx(int *p, int num);

int (*eval(int (*results)[DIM], int *num, int lenA, int lenB))[DIM];

int (*selectCommonStrings(FM *fmA, char *B, int *num))[DIM];

char **chipAlign(char *A, char *B, int sA, int tA, int sB, int tB);

char **AlignStr(FM *fmA, char *A, char *B);