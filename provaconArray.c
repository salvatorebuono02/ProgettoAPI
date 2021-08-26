//
// Created by salva on 20/08/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define MAXBUFFER 10000000
#define MAXWEIGHT 10

typedef struct heap{
    int index;
    int sum_path;
}heap;

int heap_size=0;
heap* q_priority;
int num_prog=0;

void swap(heap *first, heap *second);

void cancella_max();

void max_heapify(int i);

void inserisci_elem(int distfrom0);

int minDist(int *dist, int *spSet, int n) {
    int min=INT_MAX;
    int i_min=0;
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

void aggiornaCoda(int distfrom0,int k) {
    if(heap_size<k){

        inserisci_elem(distfrom0);
    }
    else{
        if(distfrom0<q_priority[0].sum_path){
            //cancella max
            cancella_max();
            //metto l'ultimo elemento in coda
            inserisci_elem(distfrom0);

        }
    }
    //printf("HEAPSIZE:%d",heap_size);
}

void inserisci_elem(int distfrom0) {
    heap_size++;
    q_priority= realloc(q_priority,sizeof(heap)*heap_size);
    q_priority[heap_size-1].sum_path=distfrom0;
    q_priority[heap_size-1].index=num_prog;
    int i=heap_size-1;
    while(i>0 && (q_priority[(i-1)/2].sum_path<q_priority[i].sum_path ||(q_priority[(i-1)/2].sum_path==q_priority[i].sum_path &&q_priority[(i-1)/2].index<q_priority[i].index))){
        swap(&q_priority[(i-1)/2],&q_priority[i]);
        i=(i-1)/2;
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

int calcDistFrom0(int **g, int n){

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
    //printf("Tot dist %d\n",distfrom0);
    return distfrom0;
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
    int a= calcDistFrom0(g, n);
    for (int i = 0; i < n; ++i) {
        free(g[i]);
    }
    free(g);
    return a;

}


void printBestGraph(int k) {
    if(heap_size==0){
        printf("\n");
    }
    else{
        for (int i = 0; i < heap_size-1; ++i) {
            printf("%d ",q_priority[i].index);
        }

        printf("%d\n",q_priority[heap_size-1].index);
    }

}

void cancella_max() {

    q_priority[0]=q_priority[heap_size-1];
    heap_size--;
    q_priority= realloc(q_priority,sizeof (heap)*(heap_size));
    max_heapify(0);
}

void max_heapify(int i) {
    int max=i;
    int l=2*i +1;
    int r=2*i +2;

    if(l<heap_size && (q_priority[l].sum_path>q_priority[max].sum_path ||(q_priority[l].sum_path==q_priority[max].sum_path && q_priority[l].index>q_priority[max].index)))
        max=l;
    if(r<heap_size && (q_priority[r].sum_path>q_priority[max].sum_path ||(q_priority[r].sum_path==q_priority[max].sum_path && q_priority[r].index>q_priority[max].index)))
        max=r;
    if(max!=i){
        swap(&q_priority[i],&q_priority[max]);
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
    free(kgraph);
    //printf("You want the best %d graphs\n",k);

    //printf("Type AggiungiGrafo\n");
    //gestione di aggiunta grafo
    buffer= fgets(buffer,MAXBUFFER,stdin);
    while(buffer!=NULL){

        if(strncmp("AggiungiGrafo",buffer,13)==0){
            //printf("hai scelto aggiungiGrafo\n");
            int a=addGraph(buffer,n);
            aggiornaCoda(a,k);
            num_prog++;
        }

        else if(strncmp("TopK",buffer,4)==0){
            //printf("Hai scelto topk\n");
            printBestGraph(k);
        }
        buffer= fgets(buffer,MAXBUFFER,stdin);
    }
    free(q_priority);
    free(buffer);
    return 0;
}

