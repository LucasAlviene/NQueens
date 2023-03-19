//https://github.com/prateek27/N-Queen-All-Optimisations/blob/master/n_queen_bitmask.cpp
#include <iostream>
#include <omp.h>

using namespace std;

int ans = 0, DONE;

/// More optimisized n queen code !
void solve(int rowMask, int ld, int rd)
{

    if (rowMask == DONE)
    {
        #pragma omp atomic
        ans++;
        return;
    }

    int safe = DONE & (~(rowMask | ld | rd));
    while (safe)
    {
        int p = safe & (-safe);
        safe = safe - p;
        #pragma omp task firstprivate(p)
        solve(rowMask | p, (ld | p) << 1, (rd | p) >> 1);
    }
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int nThreads = atoi(argv[2]);
    omp_set_num_threads(nThreads);
    double start, end;
    start = omp_get_wtime();
    DONE = ((1 << n) - 1);
    #pragma omp parallel
    {
        #pragma omp single nowait
        solve(0, 0, 0);
    }

    end = omp_get_wtime();
    cout << "Solucoes: " << ans << endl;

    cout << "Tempo: " << end-start << endl;
    //printf("Tempo: %f\n", end - start);
    
}