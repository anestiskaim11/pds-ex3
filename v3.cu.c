#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define N 8

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

__global__ void moment(int *ising, int *newising, int n, int b){
      //create shared memory
      __shared__ int temp [N*N];
      for(int i = (threadIdx.x*b/n)*b ; i < (threadIdx.x*b/n)*b + b; i++){
        for(int j = (threadIdx.x%(n/b))*b ; j < (threadIdx.x%(n/b))*b + b; j++){
            temp[i*n + j] = ising[i*n + j];
        }
      }
      //wait all threads to finish
      __syncthreads();

      //do calculations
      for(int k = (threadIdx.x*b/n)*b ; k < (threadIdx.x*b/n)*b + b; k++){
        for(int l = (threadIdx.x%(n/b))*b ; l < (threadIdx.x%(n/b))*b + b; l++){
          int sum = temp[k*n + l + n -n*n*(k%n == n - 1)] + temp[k*n + l - n + n*n*(k%n == 0)]
          + temp[k*n + l + 1 - n*(l%n == n - 1)]
          + temp[k*n + l - 1 + n*(l%n == 0)]
          + temp[k*n + l];
          if(sum > 0){
            newising[k*n + l] = 1 ;
          }
          else
            newising[k*n + l] = -1 ;
        }
      }
}

int main(int argc, char **argv){

    //size of Ising model
    int n = 8 ;
    // number of iterations
    int k = 2 ;

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

    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            printf("%d " , ising[i*n + j]);
        }
        printf("\n");
    }
    printf("\n");

    int *newising = (int *)malloc(n * n * sizeof(int));
    
    int *d_ising;
    int *d_newising;
    int size = n * n * sizeof(int);
    
    //allocate on gpu
    cudaMalloc((void**)&d_ising, size);
    cudaMalloc((void**)&d_newising, size);
    
    //b size
    int b = 2;
    

    for(int l = 0 ; l < k ; l++){
        //copy data to gpu
        cudaMemcpy(d_ising, ising, size, cudaMemcpyHostToDevice);
        //call function on gpu with n*n threads
        moment<<<1,(n*n)/(b*b)>>>(d_ising, d_newising, n, b);
        //copy result from gpu
        cudaMemcpy(newising, d_newising, size, cudaMemcpyDeviceToHost);

        swap(&ising,&newising);

        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < n ; j++){
                printf("%d " , ising[i*n + j]);
            }
            printf("\n");
        }
        printf("\n");
        
    }
    //free pointers
    free(ising);
    free(newising);
    cudaFree(d_ising);
    cudaFree(d_newising);
    return 0 ;
}


