## N Queens (N Rainhas)

* Juliano Strelow Buss
* Lucas Alviene Pereira

Esse programa foi adaptado para funciona em forma paralela usando MPI e OpenMP em C++ para trabalho de IPPD/UFPEL.

* NQueens.cpp: Código Original com OpenMP
* MPI.cpp: Código com a implementação do MPI

#### Código original 
https://github.com/victoraldecoa/N-Queens-Solver_OpenMP_Example/blob/master/src/nqueens-openmp.c

#### Como compilar

No Terminal, execute a seguinte linha:

```
mpicc MPI.cpp -o MPI -fopenmp
```

#### Como executar

Você deve executar o programa NQueens pelo terminal, o **primeiro paramêtro** é o **N** que representa o Nº de Linhas, Colunas e Rainhas.
O **segudo paramêtro** represeta o número de threads que o OpenMP irá usar.

Por exemplo:
```
mpirun ./MPI 5 6 // 5 Linhas, Colunas e Rainhas, usando 6 Threads 
mpirun ./MPI 9 1 // 9 Linhas, Colunas e Rainhas, usando 1 Thread
```

#### BenchMark

Como solicitado no trabalho, foi realizado um benchmark para o conjunto de.
N = {8,9,10,11} para T = {1,2,3,4,5,6,7,8,9,10,11,12}.

O SpeedUp foi calculado para T = {8, 9, 10}

Relatório.pdf em anexo com mais detalhes