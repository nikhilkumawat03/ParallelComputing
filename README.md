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

## OpenMP LU decomposition

- compile

```bash
g++ -fopenmp ompLUD.cpp utils.cpp -o ompLUD
```

- run using some random matrix

```bash
./ompLUD -n <size>
```

- run using matrix from some file

```bash
./ompLUD <file>
```

## Verification program

- compile

```bash
g++ verification.cpp utils.cpp -o verification
```

- run(Make sure to run some LU decomposition program before running this verification program)

```bash
./verification
```


## NOTES

- matrix input/output file should contain the size of matrix(an integer) in first line and next n lines should have matrix entries separated by space.
