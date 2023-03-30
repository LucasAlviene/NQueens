//https://github.com/prateek27/N-Queen-All-Optimisations/blob/master/n_queen_bitmask.cpp
#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;

int ans = 0, DONE;

/// More optimisized n queen code !
void solve(int rowMask, int ld, int rd)
{

    if (rowMask == DONE)
    {
        #if defined(_OPENMP)
            #pragma omp atomic
        #endif
        ans++;
        return;
    }

    int safe = DONE & (~(rowMask | ld | rd));
    while (safe)
    {
        int p = safe & (-safe);
        safe = safe - p;
        #if defined(_OPENMP)
            #pragma omp task firstprivate(p)
        #endif
        solve(rowMask | p, (ld | p) << 1, (rd | p) >> 1);
    }
}

double getTime(){
    #if defined(_OPENMP)
        return omp_get_wtime();
    #else
        return (double)clock();
    #endif
}

double getTotalTime(double value){
    #if defined(_OPENMP)
        return value;
    #else
        return value/CLOCKS_PER_SEC;
    #endif
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int nThreads = atoi(argv[2]);
    #if defined(_OPENMP)
        omp_set_num_threads(nThreads);
    #endif
    double start, end;
    start = getTime();
    DONE = ((1 << n) - 1);
    #pragma omp parallel
    {
        #pragma omp single nowait
        solve(0, 0, 0);
    }

    end = getTime();
    cout << "Solucoes: " << ans << endl;

    cout << "Tempo: " << getTotalTime(end-start) << endl;
    //printf("Tempo: %f\n", end - start);
    
}