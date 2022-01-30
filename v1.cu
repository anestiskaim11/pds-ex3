#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int *unroll(int **ising, int n){
    int *ising1d = (int *)malloc(n * n * sizeof(int));
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            ising1d[i*n + j] = ising[i][j];
        }
    }

    return ising1d;
}
void swap(int  **a, int  **b) {
  int  *tmp = *a;
  *a = *b;
  *b = tmp;
}

__global__ void moment(int *ising, int *newising, int n){
    //printf("%d\n", blockIdx.x);
    //if(threadIdx.x == 1023) printf("%d\n", threadIdx.x);
    int sum = ising[(blockIdx.x*1024 + threadIdx.x+n)%(n*n)] + ising[(blockIdx.x*1024 + threadIdx.x-n)%(n*n)]
    + ising[blockIdx.x*1024 + threadIdx.x + 1 - n*(threadIdx.x%n == n - 1)]
    + ising[blockIdx.x*1024 + threadIdx.x - 1 + n*(threadIdx.x%n == 0)]
    + ising[blockIdx.x*1024 + threadIdx.x];
    if(sum > 0)
        newising[blockIdx.x*1024 + threadIdx.x] = 1 ;
    else
        newising[blockIdx.x*1024 + threadIdx.x] = -1 ;
    
}

int main(int argc, char **argv){

    //size of Ising model
    int n = 2048 ;
    // number of iterations
    int k = 100 ;

    srand(time(NULL));

    int *ising = (int *) malloc(n * n * sizeof(int));
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j ++){
            ising[i*n + j] = rand() % 2 ;
            if(ising[i*n + j] == 0){
                ising[i*n + j] = -1;
            }
        }
    }

    /*for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            printf("%d " , ising[i*n + j]);
        }
        printf("\n");
    }
    printf("\n");*/

    int *newising = (int *)malloc(n * n * sizeof(int));
    
    int *d_ising;
    int *d_newising;
    int size = n * n * sizeof(int);
    
    //allocate on gpu
    cudaMalloc((void**)&d_ising, size);
    cudaMalloc((void**)&d_newising, size);
    
    
    struct timeval start, end;
    double time;
    

    int blocks = (n*n - 1)/1024 + 1;
    for(int l = 0 ; l < k ; l++){
        //copy data to gpu
        cudaMemcpy(d_ising, ising, size, cudaMemcpyHostToDevice);
        //call function on gpu with n*n threads
        gettimeofday(&start, NULL);
        moment<<<blocks,n*n/blocks>>>(d_ising, d_newising, n);
        gettimeofday(&end, NULL);
        //copy result from gpu
        cudaMemcpy(newising, d_newising, size, cudaMemcpyDeviceToHost);
        time += (double)((end.tv_usec - start.tv_usec)/1.0e6 + end.tv_sec - start.tv_sec);

        swap(&ising,&newising);
        
    }

    /*for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < n ; j++){
                printf("%d " , ising[i*n + j]);
            }
            printf("\n");
        }
        printf("\n");*/

    printf("time: %f\n", time);
    //free pointers
    free(ising);
    free(newising);
    cudaFree(d_ising);
    cudaFree(d_newising);
    return 0 ;
}
