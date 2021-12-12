## Serial LU Decomposition

- compile

```bash
g++ serialLUD.cpp utils.cpp 
``` 

- run using some random matrix

```bash
./a.out -n <size>
```

- run using matrix from some file

```bash
./a.out <file>
```

## NOTES

- matrix input file should contain the size of matrix(an integer) in first line and next n lines should have matrix entries separated by space.