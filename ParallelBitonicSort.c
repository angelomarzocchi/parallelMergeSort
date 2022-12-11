

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>



struct boundary {
    int start;
    int end;
};


void singlemergesort(int* a, int p, int r);
int* merge(int* vet, int start, int center, int end);
void coEx(int* a, struct boundary b1, struct boundary b2);
void printVector(int* a, int start, int end);
int getMaxThreads();

#define max 65535 //massimo valore raggiungibile da un elmento di un vettore
#define dimension 100000 //massima dimensione del vettore





int main()
{

    printf("Benvenuto. Questo Software effettua il BitonicSort su N numeri utilizzando piu core.\n");
    printf("Attenzione: il numero di core utilizzati e una potenza di 2. Verra calcolato il massimo numero di core utilizzabili tra quelli messi a disposizione per ottenere un risultato corretto\n");


    srand(time(NULL));
    int dim, t;
    int nproc = getMaxThreads();

    printf("Numero di thread realmente usati: %d\n", nproc);

    //Struttura che memorizzerá l'inizio e la fine di ogni sezione del vettore
    struct boundary* positions = (struct boundary*)malloc(nproc * sizeof(struct boundary));

    printf("Inserire dimensione array: ");
    scanf_s("%d", &dim);
   
    int* vet = (int*)malloc(dim * sizeof(int));
    

    
   
    

    
    for (int i = 0; i < dim; i++) {
        vet[i] = rand() % max + 1;
    }

    printf("Vettore disordinato:\n");
    printVector(vet, 0, dim - 1);

    printf("\n");

    double t0, t1, t2;
    int start, center, end;

    //ordinamento dei p vettori di dimensione n/p con il merge sort
    t0 = omp_get_wtime();
#pragma omp parallel num_threads(nproc) private(start,center,end)
    {
        t = omp_get_num_threads();
        int nloc = dim / t;
        int r = dim % t;
        int id = omp_get_thread_num();
        int step;
        //gestisco il caso della non esatta divisibilitá del vettore
        if (id < r) {
            nloc++;
            step = 0;

        }
        else { step = r; }

        start = nloc * id + step;
        end = start + nloc - 1;

        positions[id].start = start;
        positions[id].end = end;
    
        //Ogni processore effettua il mergeSort sulla sua sezione di vettore
        singlemergesort(vet, start, end);

       

        

    }
    t1 = omp_get_wtime();
    //t1-t0 : tempo per effettuare l'ordinamento dei p vettori


    //qui inizia implementazione del bitonic sort
    int ibit, jbit, proc, menum, r;
    int m = log2(t);
   
    
   
    
#pragma omp parallel num_threads(nproc) shared(m) private(ibit,jbit,proc,menum,r)
    {


        menum = omp_get_thread_num();
        
        int i,j;
        
    
            for ( i = 1; i <= m; i++) {
                ibit = (menum >> i) & 1; 
                for ( j = i - 1; j >= 0; j--) {




                    jbit = (menum >> j) & 1;
                    int potenza = pow(2, j + 1);
                    r = menum % potenza;
                    

                    if (r < pow(2, j)) {
                        proc = menum + pow(2, j);
                    }
                    else { proc = menum - pow(2, j); }

                    if (ibit == jbit) {
                        //sono un processore basso, effettuando tutti gli scambi con coEx non c'é bisogno di un else

                        coEx(vet, positions[menum], positions[proc]);


                    }
                    

#pragma omp barrier
                    //dopo aver effettuato tutti gli scambi ogni sezione del vettore viene ri-ordinata
                    singlemergesort(vet, positions[menum].start, positions[menum].end); 


#pragma omp barrier



                    
                    







                }
                
            }
        
    }



    


    t2 = omp_get_wtime();
    //t2-t1 : tempo per ordinare completamente il vettore con il BitonicSort


    
    printf("\nTotal time: %f (s)\nTime for mergeSort: %f (s)\nTime for bitonicSort: %f (s)\n", t2 - t0, t1 - t0, t2 - t1);
    

    
    
    printf("\nVettore: ");
    printVector(vet, 0, dim - 1);
    
    
    
  
    
    return 0;

}

//funzione che unisce 2 vettori ordinati
int* merge(int a[], int p, int q, int r) {
    int i, j, k = 0;
    int b[dimension];
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

//metodo per effettuare la CoExLo e la CoExHi
void coEx(int* a, struct boundary b1, struct boundary b2) {
    

    //scambio tutti gli elementi finché il processore "basso" non ha tutti gli elementi piú piccoli e quello "alto quelli piú grandi"
    int indexmax = b1.end;
    int indexmin = b2.start;
    


    while (a[indexmax] > a[indexmin] && indexmax >= b1.start && indexmin <= b2.end) {

        a[indexmax] = a[indexmax] ^ a[indexmin];
        a[indexmin] = a[indexmin] ^ a[indexmax];
        a[indexmax] = a[indexmax] ^ a[indexmin];

        if (a[indexmax] < a[indexmax - 1]) {
            indexmax--;


        }

        if (a[indexmin] > a[indexmin + 1]) {
            indexmin++;


        }

    }

}



    
    
  

//Funzione di utility per stampare il vettore
void printVector(int* a, int start, int end) {

    for (int i = start; i <= end; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

}

//funzione per calcolare il massimo numero di core utilizzabili
int getMaxThreads() {

    int t = omp_get_max_threads()+10;
    int i = 1;
    while (pow(2, i) <= t) {
        i++;
    }
    return pow(2, i - 1);


}

