#include "PSA_fmindex.h"


int thorehold = 0;


int cmp2(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b); ;
}


FM *FMbulid(char *A)
{
    FM *fmA = FMindex(A);
    return fmA;
}


int *selectprefixs(FM *fmA, char *p, int length)
{
    int l = 0, r = fmA->length - 1, i;
    for (i = length - 1; i >= 0; i--)
    {
        int idx = p[i];
        int templ = rank(fmA->gRanks, fmA->L, p[i], l - 1) + (fmA->F)[idx-1];
        int tempr = rank(fmA->gRanks, fmA->L, p[i], r) + (fmA->F)[idx-1] - 1;
        if (tempr < templ)
        {
            break;
        }
        l = templ, r = tempr;
    }

    if (length - 1 - i < thorehold || l > r)
    {
        return NULL;
    }

    int *res = (int *)malloc(sizeof(int) * (r + 3 - l));
    res[0] = r + 3 - l;
    res[1] = length - i - 1;
    for (int k = 2; l < r + 1; l++, k++)
    {
        res[k] = fmA->sa[l];
    }

    qsort(res+2, res[0] - 2, sizeof(int), cmp2);

    return res;
}


int (*findALLCommonStrings(FM *fmA, char *B, int *numCS))[DIM]
{
    int index = strlen(B);
    int IdxLen[100] = {0}, count = 0;
    int **locx = (int **)malloc(sizeof(int *) * 100);

    while (index > 0)
    {
        int *idxsPre = selectprefixs(fmA, B, index);
        if (idxsPre != NULL)
        {
            locx[count] = idxsPre;
            index -= idxsPre[1];
            IdxLen[count++] = index;
        }
        else {
            index--;
        }
    }

    *numCS = count;

    if (count == 0)
    {
        free(locx);
        return NULL;
    }

    int start = fmA->length, end = 0;
    for (int i = 0; i < count; i++)
    {
        int lenLx = locx[i][0] - 2;
        for (int j = 2; j < lenLx; j++)
        {
            end = locx[i][j] > end ? locx[i][j] : end;
            start = locx[i][j] < start ? locx[i][j] : start;
        }
    }
    int length1 = IdxLen[0] - IdxLen[count-1];
    int length2 = end - start;

    int (*res)[DIM] = (int (*)[])malloc(sizeof(int[DIM]) * count), idxRes = 0;

    for (int i = count - 1; i >= 0; i--)
    {
        res[idxRes][0] = IdxLen[i];
        res[idxRes][1] = locx[i][1];
        if (locx[i][0] > 3)
        {
            int idx = MultiToOne(locx[i], length2, IdxLen[i] / length1);
            res[idxRes][2] = locx[i][idx];
        }
        else
        {
            res[idxRes][2] = locx[i][2];
        }
        idxRes++;
    }
    free(locx);
    return res;
}


int MultiToOne(int *idxs, int length, float offset)
{
    int idx = 0;
    float rate = 1.0;

    for (int i = 2; i < idxs[0] - 2; i++)
    {
        float temp = abs(idxs[i] / length - rate);
        if (temp < rate)
        {
            idx = i, rate = temp;
        }
    }

    return idx;
}


int (*dpSelect(int (*results)[DIM], int *num))[DIM]
{
    int *p = (int *)malloc(sizeof(int) * (*num));
    for (int i = 0; i < (*num); i++)
    {
        p[i] = results[i][1];
    }

    for (int i = 0; i < (*num); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (results[i][2] >= results[j][2] + results[j][1])
            {
                p[i] = MAX(p[i], p[j] + results[i][1]);
            }
        }
    }

    return trace(results, p, num);
}


int (*trace(int (*results)[DIM], int *p, int *num))[DIM]
{
    int idx = getMAXidx(p, (*num));
    int *record = (int *)malloc(sizeof(int) * (*num)), idxR = 0;
    record[idxR++] = idx;

    while (idx > 0)
    {
        if (p[idx] == results[idx][1])
        {
            break;
        }
        int j = idx - 1;
        while (j >= 0)
        {
            if ((results[idx][2] >= results[j][1] + results[j][2]) && (p[idx] == p[j] + results[idx][1]))
            {
                record[idxR++] = j;
                idx = j;
                break;
            }
            j--;
        }
    }

    int (*res)[DIM] = (int (*)[])malloc(sizeof(int[DIM]) * idxR);

    for (int i = idxR - 1, j = 0; i >= 0; i--, j++)
    {
        res[j][0] = results[record[i]][0];
        res[j][1] = results[record[i]][1];
        res[j][2] = results[record[i]][2];
    }

    free(p);
    free(record);
    free(results);

    *num = idxR;
    return res;
}


int getMAXidx(int *p, int num)
{
    int idx = 0, vmax = p[0];
    for (int i = 1; i < num; i++)
    {
        if (p[i] > vmax)
        {
            idx = i, vmax = p[i];
        }
    }
    return idx;
}


int (*eval(int (*results)[DIM], int *num, int lenA, int lenB))[DIM]
{
    if (*num == 1)
    {
        int front = abs(results[0][2] - results[0][0]);
        int back = abs(lenA - results[0][2] - lenB + results[0][0]);
        if (front >= results[0][1] && back >= results[0][1])
        {
            free(results[0]);
            free(results);
            return NULL;
        }
        return results;
    }

    int idx = 0;
    // for first one
    while (idx < (*num))
    {
        int ave = 0;
        for (int i = 0; i < (*num); i++)
        {
            ave += (results[i][2] - results[i][0]);
        }
        ave /= (*num);

        int front = results[idx][2] - results[idx][0];
        if (front > ave && abs(front) >= results[idx][1])
        {
            idx++;
        }
        else
        {
            break;
        }
    }
    if (idx >= (*num))
    {
        return NULL;
    }

    // for other one
    int *records = (int *)malloc(sizeof(int) * (*num - idx)), idxR =0;
    records[idxR++] = idx;
    for (int i = idx + 1; i < (*num); i++)
    {
        int mid = abs(results[i][2]- results[i][0] - results[idx][2]  + results[idx][0]);
        if (mid < results[i][1])
        {
            idx = i;
            records[idxR++] = idx;
        }
    }
    
    int (*res)[DIM] = (int (*)[DIM])malloc(sizeof(int[DIM]) * idxR);
    for (int i = 0; i < idxR; i++)
    {
        res[i][0] = results[records[i]][0];
        res[i][1] = results[records[i]][1];
        res[i][2] = results[records[i]][2];
    }

    free(records);
    free(results);

    *num = idxR;
    return res;

}


int (*selectCommonStrings(FM *fmA, char *B, int *num))[DIM]
{
    thorehold = MIN(MAX(fmA->length / 100, 15), strlen(B));

    int (*results)[DIM] = findALLCommonStrings(fmA, B, num);

    if (*num == 0)
        return NULL;
    else if (*num == 1)
        return results;

    results = dpSelect(results, num);

    if (*num == 0)
        return NULL;

    return eval(results, num, fmA->length-1, strlen(B));
}


char **chipAlign(char *A, char *B, int sA, int tA, int sB, int tB)
{
    int lenA = tA - sA, lenB = tB - sB;
    char *strA = (char *)malloc(sizeof(char) * (lenA + 1));
    char *strB = (char *)malloc(sizeof(char) * (lenB + 1));
    strA[lenA] = '\0', strB[lenB] = '\0';

    strncpy(strA, A + sA, lenA);
    strncpy(strB, B + sB, lenB);

    char **res = PSA(strA, strB);
    free(strA);
    free(strB);

    return res;
}


char **AlignStr(FM *fmA, char *A, char *B)
{
    int num = 0;
    int (*results)[DIM] = selectCommonStrings(fmA, B, &num);
    if (results == NULL)
    {
        char **strsAligned = PSA(A, B);
        return strsAligned;
    }

    char **strsA = (char **)malloc(sizeof(char *) * (num + 1));
    char **strsB = (char **)malloc(sizeof(char *) * (num + 1));

    int ia = 0, ib = 0, lengthC = 0;
    for (int i = 0; i < num; i++)
    {
        char **p = chipAlign(A, B, ia, results[i][2], ib, results[i][0]);
        if (p == NULL)
        {
            strsA[i] = NULL;
            strsB[i] = NULL;
            lengthC += results[i][1];
        }
        else
        {
            strsA[i] = p[0];
            // printf("p0:%s\n", p[0]);
            strsB[i] = p[1];
            // printf("p1:%s\n", p[1]);
            lengthC += (results[i][1] + strlen(p[0]));
        }
        ia = results[i][1] + results[i][2];
        ib = results[i][1] + results[i][0];
    }
    char **p = chipAlign(A, B, ia, strlen(A), ib, strlen(B));
    if (p == NULL)
    {
        strsA[num] = NULL;
        strsB[num] = NULL;
    }
    else
    {
        strsA[num] = p[0];
        strsB[num] = p[1];
        lengthC += strlen(p[0]);
    }

    char **strsC = (char **)malloc(sizeof(char *) * 2);
    strsC[0] = (char *)malloc(sizeof(char) * (lengthC + 1));
    strsC[1] = (char *)malloc(sizeof(char) * (lengthC + 1));
    strsC[0][lengthC] = '\0';
    strsC[1][lengthC] = '\0';

    ia = 0, ib = 0;
    for (int i = 0; i < num; i++)
    {
        if (strsA[i] != NULL)
        {
            strncpy(strsC[0]+ia, strsA[i], strlen(strsA[i]));
            strncpy(strsC[1]+ib, strsB[i], strlen(strsB[i]));
            ia += strlen(strsA[i]);
            ib += strlen(strsB[i]);
        }
        strncpy(strsC[0]+ia, A + results[i][2], results[i][1]);
        strncpy(strsC[1]+ib, B + results[i][0], results[i][1]);
        ia += results[i][1];
        ib += results[i][1];
    }
    if (strsA[num] != NULL)
    {
        strncpy(strsC[0]+ia, strsA[num], strlen(strsA[num]));
        strncpy(strsC[1]+ib, strsB[num], strlen(strsB[num]));
    }

    free(results);
    for (int i = 0; i < num + 1; i++)
    {
        free(strsA[i]);
        free(strsB[i]);
    }
    free(strsA);
    free(strsB);

    return strsC;
}