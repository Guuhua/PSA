#include "FMindex.h"


FM *FMindex(char *strs)
{
    strs = init(strs);
    FM *fmstr = (FM *)malloc(sizeof(FM));
    fmstr->length = strlen(strs);
    fmstr->sa = suffixArray(strs);
    fmstr->L = bwtSA(strs, fmstr->sa);
    fmstr->F = getFirstCol(strs);
    free(strs);
    fmstr->gRanks = getRank(fmstr->L);
    return fmstr;
}


void range(FM *fmstr, char *p, int *s, int *t)
{
    int l = 0, r = fmstr->length - 1;
    int lenp = strlen(p);
    for (int i = lenp - 1; i >= 0; i--)
    {
        int idx = p[i];
        l = rank(fmstr->gRanks, fmstr->L, p[i], l - 1) + (fmstr->F)[idx-1];
        r = rank(fmstr->gRanks, fmstr->L, p[i], r) + (fmstr->F)[idx-1] - 1;
        if (r < l)
        {
            break;
        }
    }
    *s = l;
    *t = r + 1;
}


int *occurrences(FM *fmstr, char *p, int *count)
{
    int l = 0, r = 0;
    range(fmstr, p, &l, &r);
    *count = r - l;
    if (*count < 1)
    {
        return NULL;
    }
    int *idxs = (int *)malloc(sizeof(int)*(*count));
    for (int i = 0; l < r; l++)
    {
        idxs[i++] = fmstr->sa[l];
    }
    return idxs;
}

void freeFM(FM *fm)
{
    free(fm->sa);
    free(fm->F);
    free(fm->L);
    for (int i = 0; i < NUM; i++)
    {
        free(fm->gRanks[i]);
    }
    free(fm->gRanks);
}