# PSA

## PSA with kband

```python
from PSA_Kband import PSA_AGP_Kband

A = "ACTGACGTAA"
B = "ACGTGCATTAG"
print(PSA_AGP_Kband(A, B))
```

## PSA with suffix tree

```python
from PSA_STree import PSA_STree

A = "ACTGACGTAA"
B = "ACGTGCATTAG"
stree = PSA_STree(A)
print(stree.align(B))
```