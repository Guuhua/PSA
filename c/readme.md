# PSA

## PSA with kband

```c
#include "PSA_kband.h"

char A[] = "ACGT";
char B[] = "GCT";

char **alignAB = PSA(A, B);
printf("alA:%s", alignAB[0]);
printf("alB:%s", alignAB[1]);
```

## PSA with FM-index

```c
#include "PSA_fmindex.h"

char A[] = "ACGT";
char B[] = "GCT";

FM *fmA = FMbulid(A);
char **alignAB = AlignStr(fmA, A, B);
printf("alA:%s", alignAB[0]);
printf("alB:%s", alignAB[1]);
```