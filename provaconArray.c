//
// Created by salva on 20/08/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define MAXBUFFER 100000000
#define MAXWEIGHT 10

typedef struct heap{
    int index;
    int sum_path;
}heap;

int heap_size=0;
heap* coda;

void swap(heap *first, heap *second);

int cancella_min(heap *h);

void min_heapify(heap *pHeap, int i,int size);

int minDist(int *dist, int *spSet, int n) {
    int min=INT_MAX, i_min=0;
    for (int i = 0; i < n; ++i) {
        if(dist[i]<=min && spSet[i]==0)
            min=dist[i],i_min=i;
    }
    return i_min;
}

int myAtoi(char* s)
{
    int res = 0;


    for (int i = 0; s[i] != '\0'; ++i)
        res = res * 10 + s[i] - '0';

    return res;
}

void aggiornaCoda(int distfrom0) {

    if(heap_size == 0){
        coda= malloc(sizeof(heap));
        coda[heap_size].sum_path=distfrom0;
        coda[heap_size].index=heap_size;
        heap_size++;
    }
    else{
        printf("riga51\n");
        coda= realloc(coda, sizeof(heap)*(heap_size + 1));
        coda[heap_size].index=heap_size;
        coda[heap_size].sum_path=distfrom0;
        int i=heap_size;
        while (i>0 && (coda[(i-1)/2].sum_path>coda[i].sum_path || (coda[(i-1)/2].sum_path==coda[i].sum_path && coda[(i-1)/2].index>coda[i].index))){
            swap(&coda[(i-1)/2],&coda[i]);
            i=(i-1)/2;
        }
        for (int j = 0; j <=heap_size; ++j) {
            printf("Index:%d Sum:%d\n",coda[j].index,coda[j].sum_path);
        }
        heap_size++;

    }
}

void swap(heap *first, heap *second) {
    heap *tmp= malloc(sizeof(heap));
    tmp->sum_path=first->sum_path;
    tmp->index=first->index;

    first->sum_path=second->sum_path;
    first->index=second->index;

    second->sum_path=tmp->sum_path;
    second->index=tmp->index;
    free(tmp);
}

void inserisciDistInCoda(int **g, int n){

    int *dist;
    int *spSet; //insieme dei nodi visitati

    dist= malloc(sizeof (int)*n);
    spSet= malloc(sizeof (int )*n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i==j)
                g[i][j]=0;
        }
        dist[i]=INT_MAX,spSet[i]=0;
    }
    dist[0]=0;

    for (int i = 0; i < n-1; ++i) {
        int minv=minDist(dist,spSet,n);

        spSet[minv]=1;
        for (int j = 0; j < n; ++j) {
            if(spSet[j]==0 && g[minv][j]>0 && dist[minv]!=INT_MAX && dist[minv]+g[minv][j]<dist[j])
                dist[j]=dist[minv]+g[minv][j];
        }

    }
    int distfrom0=0;
    for (int i = 0; i < n; ++i) {
        if(dist[i]!=INT_MAX)
            distfrom0+=dist[i];
    }
    free(dist);
    free(spSet);
    printf("Tot dist %d\n",distfrom0);
    aggiornaCoda(distfrom0);
}


void addGraph(char *buffer,int n){
    int **g;
    char w[MAXWEIGHT]="0";
    int col,j;


    g=(int **) malloc(sizeof(int*)*n);
    for(int i=0;i<n;i++){
        col=0;
        j=0;
        g[i]= malloc(sizeof (int)*n);
        buffer=fgets(buffer,MAXBUFFER,stdin);
        //printf("ROW: %ld\n",strlen(buffer));
        //isolo il peso
        for (int k = 0;k< strlen(buffer); k++) {
            if(buffer[k]==','|| k==strlen(buffer)-1) {
                j=0;
                //
                //printf("W: %s\n",w);

                g[i][col]=myAtoi(w);
                col++;
                for (int l = 0; l < MAXWEIGHT; ++l) {
                    w[l]='\0';
                }
            }else{
                w[j]=buffer[k];

                j++;

            }


        }

    }
    inserisciDistInCoda(g,n);
    for (int i = 0; i < n; ++i) {
        free(g[i]);
    }
    free(g);
}


void printBestGraph(int k) {
    heap *copia_coda=malloc(sizeof(heap)*(heap_size));
    for (int i = 0; i < heap_size; ++i) {
        copia_coda[i]=coda[i];
    }
    for (int i = 0; i < k; ++i) {
        if(i<k-1)
            printf("%d ",cancella_min(copia_coda));
        else
            printf("%d\n",cancella_min(copia_coda));
    }
    free(copia_coda);
}

int cancella_min(heap *h) {

    if(aux_size<1)
        return -1;
    int min=h[0].sum_path;
    h[0]=h[aux_size-1];
    aux_size--;//auaxsize viene ripristinata ad ogni ciclo di k
    min_heapify(h,0,aux_size);
    return min;
}

void min_heapify(heap *pHeap, int i,int size) {

}

int main() {


    char *numOfNodes,*kgraph;
    int i;
    int j=0;
    char *buffer;
    int k;
    int n;


    //lettura prima riga
    buffer=(char *) malloc(sizeof(char )*MAXBUFFER);
    numOfNodes=(char *) calloc(MAXBUFFER/2,sizeof (char ));
    if(!numOfNodes){
        return -1;
    }
    kgraph=(char *) calloc(MAXBUFFER/2,sizeof(char ));
    buffer=fgets(buffer,10,stdin);

    for(i=0;buffer[i]!=' ';i++){
        if(buffer[i]=='\0')
            break;
        numOfNodes[i]=buffer[i];
    }
    n= myAtoi(numOfNodes);
    free(numOfNodes);
    //printf("Num of nodes are: %d\n",n);
    for(i++; isdigit(buffer[i]);i++){
        kgraph[j]=buffer[i];
        j++;
    }

    k= myAtoi(kgraph);
    free(kgraph);
    //printf("You want the best %d graphs\n",k);

    //printf("Type AggiungiGrafo\n");
    //gestione di aggiunta grafo
    buffer= fgets(buffer,20,stdin);
    while(buffer != NULL){

        if(strncmp("AggiungiGrafo",buffer,13)==0){
            //printf("hai scelto aggiungiGrafo\n");
            addGraph(buffer,n);
        }

        else if(strncmp("TopK",buffer,4)!=0){
            //printf("Hai scelto topk\n");
            printBestGraph(k);
        }
        buffer= fgets(buffer,20,stdin);
    }

    free(buffer);
    return 0;
}

