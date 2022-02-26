# PSA

## PSA with kband

```java
String A = "AGCT";
String B = "GCT";
psa pkband = new psa(A, B, "kband");
String[] alignAB = pkband.getStrAlign();
```

## PSA with suffix tree

```java
String A = "AGCT";
String B = "GCT";
psa pStree = new psa(A, B, "suffix");
String[] alignAB = pStree.getStrAlign();
```

## PSA with FM-index

```java
String A = "AGCT";
String B = "GCT";
psa pfmindex = new psa(A, B, "kfmindex");
String[] alignAB = pfmindex.getStrAlign();
```