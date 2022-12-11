#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
void singlemergesort(int* a, int p, int r);
int* merge(int* vet, int start, int center, int end);
void printVector(int* a, int start, int end);
int getMaxThreads();

#define max 65000
#define dimension 100000

struct boundary {
    int start;
    int end;
};
int main()
{
    printf("Benvenuto. Questo Software effettua il MergeSort su N numeri utilizzando piu core.\n");
    printf("Attenzione: il numero di core utilizzati e una potenza di 2. Verra calcolato il massimo numero di core utilizzabili tra quelli messi a disposizione per ottenere un risultato corretto\n");
    
   


    srand(time(NULL));
    int dim,t;
    int nproc = getMaxThreads();
    printf("Numero di thread realmente usati: %d\n",nproc);
    struct boundary* positions = (struct boundary*)malloc(nproc * sizeof(struct boundary));
    
    printf("Inserire dimensione array: ");
    scanf_s("%d", &dim,1);
    printf("Dimensione: %d\n", dim);
    int* vet = (int*)malloc(dim * sizeof(int));

      
    for (int i = 0; i < dim; i++) {
        vet[i] = rand() % max + 1;
    }

    
        printf("Vettore disordinato:\n");
        printVector(vet, 0, dim - 1);
   
    printf("\n");

    double t0, t1,t2;
    int start, center, end;

    //ordinamento dei p vettori di dimensione n/p
    t0 = omp_get_wtime();
#pragma omp parallel num_threads(nproc) private(start,center,end)
    {
         t = omp_get_num_threads();
        int nloc = dim / t;
        int r = dim % t;
        int id = omp_get_thread_num();
        int step;

        if (id < r) {
            nloc++;
            step = 0;

        }
        else { step = r; }

        start = nloc * id + step;
        end = start + nloc - 1;
        positions[id].start = start;
        positions[id].end = end;
        

        singlemergesort(vet, start, end);

    }

    //merge dei p vettori
    
    int times = log2(t);
    int involved = t / 2;

    for (int i = 0; i < times; i++) {

#pragma omp parallel num_threads(involved)
        {
            int menum = omp_get_thread_num();
            
            merge(vet, positions[menum * 2].start, (positions[menum *2].start + positions[menum*2 +1 +i].end) / 2, positions[(menum * 2) + 1 +i].end);
            positions[menum * 2].end = positions[menum * 2 + 1 + i].end;
            positions[menum * 2 + 1 + i].start = positions[menum * 2].start;


        
        }
        involved /= 2;
    
    }

    


    
    

    t1 = omp_get_wtime();

    printf("\nTime for finalSorting: %f (s)\n", t1 - t0);

    printf("Vettore ordinato:\n");
    printVector(vet, 0, dim - 1);
    
  

    return 0;
}

//funzione che unisce 2 vettori ordinati
int* merge(int a[], int p, int q, int r) {
    int i, j, k = 0, b[dimension];
    i = p;
    j = q + 1;

    while (i <= q && j <= r) {
        
        if (a[i] < a[j]) {

            b[k] = a[i];
            i++;
        }
        else {
            b[k] = a[j];
            j++;
        }
        k++;
    }
    while (i <= q) {
        b[k] = a[i];
        i++;
        k++;
    }
    while (j <= r) {
        b[k] = a[j];
        j++;
        k++;
    }
    
    for (k = p; k <= r; k++) {
        a[k] = b[k - p];
        
    }
    return a;
}


//funzione che richiama il mergeSort (sequenziale)
void singlemergesort(int* a, int p, int r) {
    

    int q;


    if (p < r) {
        q = (p + r) / 2;


        singlemergesort(a, p, q);

        singlemergesort(a, q + 1, r);

        merge(a, p, q, r);
    }
    return;
}


//Funzione di utility per stampare il vettore
void printVector(int* a, int start, int end) {

    for (int i = start; i <= end; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

}

int getMaxThreads() {

    int t = omp_get_max_threads()-2;
    int i = 1;
    while (pow(2, i) <= t) {
        i++;
    }
    return pow(2, i - 1);


}














