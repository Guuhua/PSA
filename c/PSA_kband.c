#include "PSA_kband.h"


int parse(BYTE b, int s)
{
    b >>= (4 - s * 2);
    return (b & 3) - 1;
} 

int freemem(int **p, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(p[i]);
    }
    free(p);
}

void Init(int ***pm, BYTE ***bt, int m, int k, int diff)
{
    *pm = (int **)malloc(sizeof(int *) * 3);
    *bt = (BYTE **)malloc(sizeof(BYTE *) * (m + 1));
    if ((*pm) == NULL || (*bt) == NULL) 
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; i++)
    {
        (*pm)[i] = (int *)malloc(sizeof(int) * (diff + 2 * k + 1));
        // memset((*pm)[i], ne, diff + 2 * k + 1);
        for (int j = 0; j < diff + 2*k + 1; ++j)
        {
            (*pm)[i][j] = NEGINF;
        }
    }
    for (int i = 0; i < m + 1; i++)
    {
        (*bt)[i] = (BYTE *)calloc(diff + 2 * k + 1, sizeof(BYTE));
    }

    (*pm)[0][k] = 0;
    (*bt)[0][k] = 16;
    for (int j = 1; j < k + diff + 1; j++)
    {
        (*pm)[1][j + k] = -OPENGAP - EXGAP * (j - 1);
        (*bt)[0][j + k] = 8;
    }
    for (int i = 1; i < k + 1; ++i)
    {
        (*bt)[i][k - i] = 3;
    }
}

void Init2(int ***pm, int ii, int k, int diff)
{
    *pm = (int **)malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++)
    {
        (*pm)[i] = (int *)malloc(sizeof(int) * (diff + 2 * k + 1));
        // memset((*pm)[i], NEGINF, diff + 2 * k + 1);
        for (int j = 0; j < diff + 2*k + 1; ++j)
        {
            (*pm)[i][j] = NEGINF;
        }
    }
    if (ii < k + 1)
    {
        (*pm)[2][k-ii] = -OPENGAP - EXGAP * (ii - 1);
    }
}

char** align(char A[], char B[])
{
    // n >= m require
    int m = strlen(A), n = strlen(B), k = 1;

    if (m == 0 && n == 0)
    {
        return NULL;
    }
    else if (m == 0)
    {
        char **p2c = (char **)malloc(sizeof(char *) * 2);
        p2c[0] = (char *)malloc(sizeof(char) * (n + 1));
        p2c[1] = (char *)malloc(sizeof(char) * (n + 1));
        memset(p2c[0], '-', n);
        strncpy(p2c[1], B, n);
        p2c[0][n] = '\0';
        p2c[1][n] = '\0';
        return p2c;
    }

    int diff = n - m;
    int maxk = MIN(m, MAX(m / 5, 10));
    int value_Old = -NEGINF, value_New;

    while (k <= maxk)
    {
        // BYTE bt[m + 1][diff + 2 * k + 1];
        // int pm[3][diff + 2 * k + 1];
        BYTE **bt = NULL;
        int **pm = NULL;
        Init(&pm, &bt, m, k, diff);
        for (int i = 1; i < m + 1; i++)
        {
            // int pm2[3][diff + 2 * k + 1];
            int **pm2 = NULL;
            Init2(&pm2, i, k, diff);
            for (int ii = -k; ii < diff + k + 1; ++ii) 
            {
                int j = ii;
                if ((1 <= j + i) && (j + i <= n))
                {
                    j += k;
                    BYTE bt1, bt2 = 0, bt3 = 0;
                    bt1 = ChooseONE(pm[0][j], pm[1][j], pm[2][j]);
                    pm2[0][j] = MAX3(pm[0][j], pm[1][j], pm[2][j]) + MATCH(A[i-1], B[j+i-k-1]);
                    
                    if (Inside(i, j+i-k-1, k, diff))
                    {
                        pm2[1][j] = MAX(pm2[0][j-1] - OPENGAP, pm2[1][j-1] - EXGAP);
                        bt2 = pm2[0][j-1] - OPENGAP > pm2[1][j-1] - EXGAP ? 4 : 8;
                    }
                    if (Inside(i-1, j+i-k, k, diff))
                    {
                        pm2[2][j] = MAX(pm[0][j+1] - OPENGAP, pm[2][j+1] - EXGAP);
                        bt3 = pm[0][j+1] - OPENGAP > pm[2][j+1] - EXGAP ? 1 : 3;
                    }

                    bt[i][j] = bt1 + bt2 + bt3;
                }
            }
            freemem(pm, 3);
            pm = pm2;
        }
        value_New = MAX3(pm[0][diff+k], pm[1][diff+k], pm[2][diff+k]);
        if (value_New == value_Old || (k * 2) > maxk)
        {
            BYTE channel = (BYTE) ChooseMAX(pm[0][diff+k], pm[1][diff+k], pm[2][diff+k]);
            freemem(pm, 3);
            return TraceBack(bt, A, B, k, channel);
        }
        else
        {
            freemem(pm, 3);
            k *= 2;
            value_Old = value_New;
        }
    }
}

char** TraceBack(BYTE **bt, char A[], char B[], int k, BYTE channel)
{
    char **p2c = (char **)malloc(sizeof(char *) * 2);
    int m = strlen(A), n = strlen(B);
    int i = m, b_j = n, j = n - m + k;
    int length = 0;
    BYTE ch = channel;

    // to get length
    while (i > 0 || j > k)
    {   
        length++;
        switch (channel)
        {
        case 0:
            channel = parse(bt[i][j], 0);
            i--;
            b_j--;
            break;
        case 1:
            channel = parse(bt[i][j], 1);
            b_j--;
            j--;
            break;
        case 2:
            channel = parse(bt[i][j], 2);
            i--;
            j++;
            break;
        default:
            printf("\nwrong channel = %d\n", channel);
            exit(1);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        p2c[i] = (char *)malloc(sizeof(char) * (length + 1));
        p2c[i][length] = '\0';
    }

    i = m, b_j = n, j = n - m + k;
    int idx = length - 1;
    channel = ch;
    while (i > 0 || j > k)
    {   
        switch (channel)
        {
        case 0:
            channel = parse(bt[i][j], 0);
            p2c[0][idx] = A[i-1];
            p2c[1][idx] = B[b_j-1];
            i--;
            b_j--;
            break;
        case 1:
            channel = parse(bt[i][j], 1);
            p2c[0][idx] = '-';
            p2c[1][idx] = B[b_j-1];
            b_j--;
            j--;
            break;
        case 2:
            channel = parse(bt[i][j], 2);
            p2c[0][idx] = A[i-1];
            p2c[1][idx] = '-';
            i--;
            j++;
            break;
        }
        idx--;
    }
    for (int i = 0; i < m+1; i++)
    {
        free(bt[i]);
    }
    free(bt);
    return p2c;
}

char** PSA(char A[], char B[])
{
    int m = strlen(A), n = strlen(B);
    if (n >= m)
    {
        return align(A, B);
    }
    else
    {
        char **temp = align(B, A);
        char *p = temp[0];
        temp[0] = temp[1];
        temp[1] = p;
        return temp;
    }
}