#include "getRank.h"


int **getRank(char *L)
{
    int length = strlen(L);
    int **p = (int **)malloc(sizeof(int *)*NUM);
    for (int i = 0; i < NUM; i++)
    {
        p[i] = NULL;
    }
    int count[NUM] = {0};
    int letters[NUM] = {0}, idxletter = 0;
    for (int i = 0; i < length; i++)
    {
        int idx = L[i];
        if (p[idx] == NULL)
        {
            letters[idxletter++] = idx;
            p[idx] = (int *)malloc(sizeof(int)*((length>>GAP)+1));
        }
    }

    for (int i = 0; i < length; i++)
    {
        int idx = L[i];
        count[idx]++;
        if ((i & 15) == 0)
        {
            for (int j = 0; j < idxletter; j++)
            {
                p[letters[j]][i>>GAP] = count[letters[j]];
            }
        }
    }
    return p;
}

int rank(int **ranks, char *L, char c, int offset)
{
    if (offset < 0)
    {
        return 0;
    }
    int numc = 0;
    while (offset & 15)
    {
        if (L[offset] == c)
        {
            numc++;
        }
        offset--;
    }
    int idx = c;
    if (ranks[idx] == NULL)
    {
        return 0;
    }
    return ranks[idx][offset>>GAP] + numc;
}