#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>

#if defined(_OPENMP)
#include <omp.h>
#endif

#define BUFFER_SIZE 1024 * 2//sizeoff(int64_t) * 2
#define MAX_N 50

//mpicc test.c -o test -fopenmp
//mpirun ./test 8

int check_acceptable(int queen_rows[MAX_N], int64_t n)
{
	int64_t i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i+1; j < n; j++)
		{
			// two queens in the same row => not a solution!
			if (queen_rows[i] == queen_rows[j]) return 0;
			
			// two queens in the same diagonal => not a solution!
			if (queen_rows[i] - queen_rows[j] == i - j ||
			    queen_rows[i] - queen_rows[j] == j - i)
			    return 0;
		}
	}

	return 1;
}


int64_t getMaxIter(int n,int argc, char* argv[]){
    int64_t max_iter = 1;
    
    double start_time, end_time;
    int number_solutions = 0;
        
	{
	    int num_workers;
        int i;
		#if defined(_OPENMP)
			num_workers = (argc > 2) ? atoi(argv[2]) : 30;
			
			omp_set_num_threads(num_workers);
		#endif   
        for (i = 0; i < n; i++)
        {
            max_iter *= n;
        }
    }
	return max_iter;
}

int NQueens(int n,int64_t index,int64_t iter)
{
    int number_solutions = 0;
            
	int64_t max_iter = index + iter;
	int64_t i;
	#if defined(_OPENMP)
		#pragma omp parallel for
	#endif
	for (i = index; i < max_iter; i++)
	{
		int64_t code = i;
		int j;
	    int queen_rows[MAX_N];
		// the index correspond to the queen's number and the queen's collumn
		// we only generate configurations where there's only one queen per collumn
		for (j = 0; j < n; j++)
		{
			queen_rows[j] = code % n;
			
			code /= n;
		}
		
		if (check_acceptable(queen_rows, n))
		{
			#if defined(_OPENMP)
				#pragma omp atomic
			#endif
		    number_solutions++;
        }
	}
	return number_solutions;
//    printf("Number of found solutions is %d\n", number_solutions);
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

int main(int argc, char* argv[]){
	int n;
    double start, end;
    start = getTime();

    n = (argc > 1) ? atoi(argv[1]) : 8;
    int64_t maxIter = getMaxIter(n,argc,argv);

    int sol = NQueens(n,0,maxIter);

    end = getTime();

    printf("Solucoes: %d\n",sol);
    printf("Tempo: %f\n",getTotalTime(end-start));
	return 0;
}