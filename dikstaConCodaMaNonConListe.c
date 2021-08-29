//
// Created by salva on 26/08/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define MAXBUFFER 10000000
#define MAXWEIGHT 10


typedef struct coda{
    int info;
    int dist;
}heap_elem;

typedef struct Min{
    int size;
    int capacity;
    heap_elem *array;
}heap;

heap* q_priority;
int num_prog=0;


void cancella_max();

void max_heapify(int i);
void printHeap(heap*);

void createHeap(heap *pMin, int n);

void accoda(heap *pMin, int i, int dist);
void accodaInHeap(heap*,int,int);
void removeMin(heap *pMin);

int myAtoi(char* s)
{
    int res = 0;


    for (int i = 0; s[i] != '\0'; ++i)
        res = res * 10 + s[i] - '0';

    return res;
}

void aggiornaCoda(int distfrom0) {


    if(distfrom0<q_priority->array[0].dist && q_priority->size==q_priority->capacity){
        //cancella max
        cancella_max();
    }
    //metto l'ultimo elemento in coda
    accodaInHeap(q_priority,num_prog,distfrom0);
    //printHeap(q_priority);
    //printf("HEAPSIZE:%d",heap_size);
}
void accodaInHeap(heap* h,int i,int dist){
    heap_elem tmp;
    if(q_priority->size<q_priority->capacity){
        h->array[h->size].dist=dist;
        h->array[h->size].info=i;
        int j=h->size;
        while (j>0 && (h->array[(j-1)/2].dist<h->array[j].dist||(h->array[(j-1)/2].dist==h->array[j].dist && h->array[(j-1)/2].info<h->array[j].info))){
            tmp=h->array[(j-1)/2];
            h->array[(j-1)/2]=h->array[j];
            h->array[j]=tmp;
            j=(j-1)/2;
        }
        h->size++;
    }
}


void minHeapify(heap* minHeap, int i){
    int min,l,r;
    heap_elem tmp;
    l=(2*i)+1;
    r=(2*i)+2;
    if(l<minHeap->size && minHeap->array[l].dist<minHeap->array[i].dist)
        min=l;
    else
        min=i;
    if(r<minHeap->size && minHeap->array[r].dist<minHeap->array[min].dist)
        min=r;
    if(min!=i){
        tmp=minHeap->array[i];
        minHeap->array[i]=minHeap->array[min];
        minHeap->array[min]=tmp;
        minHeapify(minHeap,min);
    }
}

int calcDistFrom0(int **g, int n){
    int*a=(int*) malloc(n* sizeof(int));
    heap *Q=(heap*) malloc(sizeof(heap));
    int ndis;
    heap_elem u;
    createHeap(Q,n);
    int distfrom0=0;
    for (int i = 0; i < n; ++i) {
        heap_elem e;
        e.info=i;
        if(i==0){
            e.dist=0;
            a[i]=0;
        }
        else{
            e.dist=INT_MAX;
            a[i]=INT_MAX;
        }
        accoda(Q,i,e.dist);
        //printf("Q->SIZE: %d\n",Q->size);
    }
    while (Q->size>0){
        //printf("\nWHILE\n");
        //printf("size:%d\n",Q->size);
        //printHeap(Q);
        u=Q->array[0];
        if(u.dist==INT_MAX)
            return distfrom0;
        distfrom0=distfrom0+u.dist;
        //printf("Distfrom0: %d\n",distfrom0);
        removeMin(Q);
        for (int i = 0; i < n; ++i) {
            if(g[u.info][i]!=0 && i!=u.info && i!=0 ){
                ndis=u.dist+g[u.info][i];
                //printf("nids: %d\n",ndis);
                //printf("a[%d]=%d\n",i,a[i]);
                if(a[i]>ndis){
                    //mi vado a trovare l'elemento corrispondente nell'array all'indice selezionato
                    int j = 0;
                    for (; j < Q->size && Q->array[j].info!=i; ++j);
                    if(Q->array[j].info==i){
                        //aggiorno i valori
                        Q->array[j].dist=ndis;
                        a[i]=ndis;
                        //printf("AGGIORNO a[%d]=%d\n",i,a[i]);
                        //dal penultimo livello a salire, sistemo gli elementi secondo un heap
                        for(int m=(Q->size-1)/2;m>=0;m--){
                            minHeapify(Q,m);
                        }
                    }
                }
            }
        }
    }
    free(a);
    free(Q->array);
    //printf("Distfrom0 %d: \n",distfrom0);
    return distfrom0;
}


void accoda(heap *pMin, int i, int dist){
    heap_elem tmp;
    if(pMin->size<pMin->capacity){

        pMin->array[pMin->size].dist=dist;
        pMin->array[pMin->size].info=i;

        int k=pMin->size;
        while(k>0 && pMin->array[(k-1)/2].dist>pMin->array[k].dist){
            tmp=pMin->array[(k-1)/2];
            pMin->array[(k-1)/2]=pMin->array[k];
            pMin->array[k]=tmp;
            k=(k-1)/2;
        }
        pMin->size=(pMin->size)+1;
    }
}



void removeMin(heap *pMin) {
    pMin->array[0]=pMin->array[(pMin->size)-1];
    pMin->size--;
    minHeapify(pMin,0);
}

void createHeap(heap *pMin, int n) {
    pMin->size=0;
    pMin->capacity=n;
    pMin->array=(heap_elem*) malloc(sizeof(heap_elem) * n);
}


int addGraph(char *buffer,int n){
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

        for (int k = 0;buffer[k]!='\n' ; k++) {
            //printf("BUFFER[%d]=%c\n",k,buffer[k]);
            if(buffer[k]==','){
                //printf("W: %s\n",w);
                g[i][col]=myAtoi(w);
                col++;
                for (int l = 0; l < MAXWEIGHT; ++l) {
                    w[l]='\0';
                }
                j=0;
            }
            else{
                w[j]=buffer[k];
                j++;
            }

        }
        //
        g[i][col]=myAtoi(w);
        for (int l = 0; l < MAXWEIGHT; ++l) {
            w[l]='\0';
        }

    }
    /*
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            printf("%d\t",g[i][k]);
        }
        printf("\n");
    }
    */
    int a= calcDistFrom0(g, n);
    for (int i = 0; i < n; ++i) {
        free(g[i]);
    }
    free(g);
    return a;

}


void printBestGraph() {
    if(q_priority->size==0){
        printf("\n");
    }
    else{
        for (int i = 0; i <(q_priority->size)-1; ++i) {
            printf("%d ",q_priority->array[i].info);
        }

        printf("%d\n",q_priority->array[(q_priority->size)-1].info);
    }

}

void cancella_max() {

    q_priority->array[0]=q_priority->array[(q_priority->size)-1];
    q_priority->size--;
    max_heapify(0);
}

void max_heapify(int i) {
    int max=i;
    int l=2*i +1;
    int r=2*i +2;
    heap_elem tmp;

    if(l<q_priority->size && (q_priority->array[l].dist>q_priority->array[max].dist ||(q_priority->array[l].dist==q_priority->array[max].dist && q_priority->array[l].info>q_priority->array[max].info)))
        max=l;
    if(r<q_priority->size && (q_priority->array[r].dist>q_priority->array[max].dist ||(q_priority->array[r].dist==q_priority->array[max].dist && q_priority->array[r].info>q_priority->array[max].info)))
        max=r;
    if(max!=i){
        tmp=q_priority->array[i];
        q_priority->array[i]=q_priority->array[max];
        q_priority->array[max]=tmp;
        max_heapify(max);
    }
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
    buffer=fgets(buffer,MAXBUFFER,stdin);

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
    q_priority= malloc(sizeof(heap));
    k= myAtoi(kgraph);
    createHeap(q_priority,k);
    free(kgraph);
    //printf("You want the best %d graphs\n",k);

    //printf("Type AggiungiGrafo\n");
    //gestione di aggiunta grafo
    buffer= fgets(buffer,MAXBUFFER,stdin);
    while(buffer!=NULL){

        if(strncmp("AggiungiGrafo",buffer,13)==0){
            //printf("hai scelto aggiungiGrafo\n");
            int a=addGraph(buffer,n);
            aggiornaCoda(a);
            num_prog++;
        }

        else if(strncmp("TopK",buffer,4)==0){
            //printf("Hai scelto topk\n");
            printBestGraph();
        }
        buffer= fgets(buffer,MAXBUFFER,stdin);
    }
    free(q_priority->array);
    free(buffer);
    return 0;
}

void printHeap(heap *heap1){
    printf("\nHEAP\n");
    for(int i=0;i<heap1->size;i++)
        printf("Array[%d]: %d, info: %d\n",i,heap1->array[i].dist,heap1->array[i].info);
}
