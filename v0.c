#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>


void swap(int  ***a, int  ***b) {
  int  **tmp = *a;
  *a = *b;
  *b = tmp;
}
    

int main(int argc, char **argv){

    //size of Ising model 
    int n = 1024 ;
    // number of iterations 
    int k = 100 ; 

    srand(time(NULL));

    int ** ising = (int **) malloc(n * sizeof(int *));
    for(int i = 0 ; i < n ; i++){
        ising[i] = (int *)malloc(n * sizeof(int));
    }

    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j ++){
            ising[i][j] = rand() % 2 ;
            if(ising[i][j] == 0){
                ising[i][j] = -1;
            }
        }
    }

    // for(int i = 0 ; i < n ; i++){
    //     for(int j = 0 ; j < n ; j++){
    //         printf("%d " , ising[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    int **newising = (int **)malloc(n * sizeof(int *));
    for(int i = 0 ; i  < n ; i++){
        newising[i] = (int *)malloc(n * sizeof(int));
    }

    struct timeval start, end;
    double time;
    gettimeofday(&start, NULL);

    for(int l = 0 ; l < k ; l++){
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < n ; j++){
                int sum = ising[((i+1)+n)%n][(j+n)%n] + ising[((i-1)+n)%n][(j+n)%n] + ising[(i+n)%n][((j+1)+n)%n] + ising[(i+n)%n][((j-1)+n)%n] + ising[(i+n)%n][(j+n)%n];
                if(sum > 0)
                    newising[i][j] = 1 ; 
                else
                    newising[i][j] = -1 ;
            }
        }

        swap(&ising,&newising);

       
        
    }

    // for(int i = 0 ; i < n ; i++){
    //     for(int j = 0 ; j < n ; j++){
    //         printf("%d " , ising[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    gettimeofday(&end, NULL);
    time = (double)((end.tv_usec - start.tv_usec)/1.0e6 + end.tv_sec - start.tv_sec);
    printf("time: %f\n", time);

    free(ising);
    free(newising);
    return 0 ; 
}
