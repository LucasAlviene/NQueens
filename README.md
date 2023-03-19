## N Queens (N Rainhas)

* Juliano Strelow Buss
* Lucas Alviene Pereira

Esse programa foi adaptado para funciona em forma paralela em C++ para trabalho de IPPD/UFPEL.

#### Código original 
https://github.com/prateek27/N-Queen-All-Optimisations/blob/master/n_queen_bitmask.cpp

#### Como compilar

No Terminal, execute a seguinte linha:

```
g++ NQueens.cpp -o NQueens -fopenmp
```

#### Como executar

Você deve executar o programa NQueens pelo terminal, o **primeiro paramêtro** é o **N** que representa o Nº de Linhas, Colunas e Rainhas.
O **segudo paramêtro** represeta o número de threads que o OpenMP irá usar.

Por exemplo:
```
./NQueens 5 6 // 5 Linhas, Colunas e Rainhas, usando 6 Threads 
./NQueens 9 1 // 9 Linhas, Colunas e Rainhas, usando 1 Thread
```

#### BenchMark

Como solicitado no trabalho, foi realizado um benchmark para o conjunto de.
N = {8,9,10,11,12,13,14,15,16,17} para T = {1,2,3,4,5,6}.
E N = {14,15,16,17} para T = {7,8,9,10,11,12}