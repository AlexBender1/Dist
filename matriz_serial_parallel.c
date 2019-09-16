#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#pragma omp parallel 

int **Alocar_matriz_real (int flag, int m, int n){
    int **v;  /* ponteiro para a matriz */
    int i = 0, j = 0;    /* variavel auxiliar      */
    if (m < 1 || n < 1) { /* verifica parametros recebidos */
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }

    /* aloca as linhas da matriz */
    v = (int **) calloc (m, sizeof(int *));	
    // Um vetor de m ponteiros para float */
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    /* aloca as colunas da matriz */
    for ( i = 0; i < m; i++ ) {
        v[i] = (int*) calloc (n, sizeof(int));	/* m vetores de n floats */

        if (v[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }

    srand(time(NULL));

    if ( flag == 1){
        #pragma omp parallel for
        for ( i = 0; i < m; ++i){
            for ( j = 0; j < n; ++j){
                v[i][j] = rand() % 10;
            }
        }
    } 

    return v;
}

int **Liberar_matriz_real (int m, int n, int **v){
    int  i;  /* variavel auxiliar */ 
    if (v == NULL) return (NULL);
    if (m < 1 || n < 1) {  /* verifica parametros recebidos */
        printf ("** Erro: Parametro invalido **\n");
        return (v);
    }
    for (i=0; i<m; i++) free (v[i]); /* libera as linhas da matriz */
    free (v);      /* libera a matriz (vetor de ponteiros) */
    return (NULL); /* retorna um ponteiro nulo */
}

void showMatriz(int m, int n, int **v){
    int i, j;

    for (i = 0; i < m; ++i){
        for (j = 0; j < n; ++j){
            printf("Valor[%d][%d]: %d. \n", i, j, v[i][j]);
        }
    }
}

void main (void){
    int **mat1, **mat2, **mult;  /* matriz a ser alocada */
    int l1 = 2000, c1 = 2000;   /* numero de linhas e colunas da matriz */
    int l2 = 2000, c2 = 2000;   /* numero de linhas e colunas da matriz */
    int i, j, k, sum = 0;
    clock_t t; 
    double time_taken; 

    mat1 = Alocar_matriz_real (1, l1, c1);
    mat2 = Alocar_matriz_real (1,l2, c2);
    mult = Alocar_matriz_real (0, l1, c1);    

    t = clock();
    
    omp_set_num_threads(16);

    #pragma omp parallel for private(sum, i, j, k) shared(mat1, mat2, l1, c2, l2) schedule(static)
    for (i = 0; i < l1; ++i) {
        for (j = 0; j < c2; ++j) {
            for (k = 0; k < l2; k++) {
                sum += mat1[i][k]*mat2[k][j];
            }

        mult[i][j] = sum;
        sum = 0;
        }
    }

    t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Mult took %f seconds to execute \n", time_taken); 
/*
    mat1 = Liberar_matriz_real (l, c, mat1);
    mat2 = Liberar_matriz_real (l, c, mat2);
    mult = Liberar_matriz_real(l, c, mult);*/
}
