#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
      for(int i = (threadIdx.x/b)*b ; i < (threadIdx.x/b)*b + b; i++){
        for(int j = (threadIdx.x%b)*b ; j < (threadIdx.x%b)*b + b; j++){
          int sum = ising[(i*n + j +n)%(n*n)] + ising[(i*n + j -n)%(n*n)]
          + ising[i*n + j + 1 - n*(j%n == n - 1)]
          + ising[i*n + j - 1 + n*(j%n == 0)]
          + ising[i*n + j];
          if(sum > 0)
            newising[i*n + j] = 1 ;
          else
            newising[i*n + j] = -1 ;
        }
      }
}

int main(int argc, char **argv){

    //size of Ising model
    int n = 4 ;
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
