#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include <inttypes.h>

#include <omp.h>

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
	int worldSize, myRank;
	MPI_Status st;

    MPI_Init(NULL, NULL);  // Inicialização
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize); // Quantos processos envolvidos?
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // Meu identificador


	if(myRank == 0){
		double start, end;
    	start = getTime();

		n = (argc > 1) ? atoi(argv[1]) : 8;
		int64_t maxIter = getMaxIter(n,argc,argv);
		int64_t iter = maxIter/worldSize;
		int64_t index = iter;
		int p = 0;
		char buffer[BUFFER_SIZE];
		for(int i =1;i<worldSize;i++){
			p = 0;
			MPI_Pack(&index,1,MPI_LONG_LONG_INT,buffer,BUFFER_SIZE,&p,MPI_COMM_WORLD);
			MPI_Pack(&iter,1,MPI_LONG_LONG_INT,buffer,BUFFER_SIZE,&p,MPI_COMM_WORLD);
			MPI_Pack(&n,1,MPI_INT,buffer,BUFFER_SIZE,&p,MPI_COMM_WORLD);
			MPI_Ssend((void *)buffer,p,MPI_PACKED,i, 0, MPI_COMM_WORLD);
			index  = index + iter;
		}
		int sols[worldSize];
		MPI_Request req[worldSize];
		MPI_Status status[worldSize];
		for(int i =1;i<worldSize;i++){
			MPI_Irecv(&sols[i],1, MPI_INT,i,MPI_ANY_TAG,MPI_COMM_WORLD,&req[i]);
		}

		int sol = NQueens(n,index,iter);
		for(int i =1;i<worldSize;i++){
			MPI_Wait( &req[i], &status[i] );
			sol += sols[i];
		}

		end = getTime();

		printf("Solucoes: %d\n",sol);
		printf("Tempo: %f\n",getTotalTime(end-start));
	}else{
		int p = 0;
		int64_t iter,index;
		char buffer[BUFFER_SIZE];
		MPI_Recv((void *)buffer,BUFFER_SIZE,MPI_PACKED,0,MPI_ANY_TAG,MPI_COMM_WORLD, &st);
		MPI_Unpack(buffer,BUFFER_SIZE,&p,&index,1,MPI_LONG_LONG_INT,MPI_COMM_WORLD);
		MPI_Unpack(buffer,BUFFER_SIZE,&p,&iter,1,MPI_LONG_LONG_INT,MPI_COMM_WORLD);
		MPI_Unpack(buffer,BUFFER_SIZE,&p,&n,1,MPI_INT,MPI_COMM_WORLD);
		int sol = NQueens(n,index,iter);
		MPI_Send(&sol,1,MPI_INT, 0,0, MPI_COMM_WORLD);
	}


	MPI_Finalize(); // Finalização
	return 0;
}