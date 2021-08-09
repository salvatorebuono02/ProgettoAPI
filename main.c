#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MAXBUFFER 100
#define MAXWEIGHT 100
void addGraph(int);

int main() {

    char buffer[MAXBUFFER];
    char *firstRow,*numOfNodes,*kgraph;
    int i;
    int j=0;
    int n;
    char addgraph[MAXBUFFER]="AggiungiGrafo";
    int k;

    //lettura prima riga
    firstRow=(char *) malloc(sizeof(char )*MAXBUFFER);
    numOfNodes=(char *) calloc(MAXBUFFER,sizeof (char ));
    if(!numOfNodes){
        return -1;
    }
    kgraph=(char *) malloc(sizeof(char )*MAXBUFFER);
    strcpy(firstRow,fgets(buffer,10,stdin));
    printf("First row  : %s", firstRow);

    for(i=0;firstRow[i]!=',';i++){
        if(firstRow[i]=='\0')
            break;
        numOfNodes[i]=firstRow[i];
    }
    n= atoi(numOfNodes);
    printf("Num of nodes are: %d\n",n);
    for(i++;firstRow[i]!='\0';i++){
        kgraph[j]=firstRow[i];
        j++;
    }

    k= atoi(kgraph);
    printf("You want the best %d graphs\n",k);
    free(firstRow);
    free(numOfNodes);
    free(kgraph);

    //gestione di aggiunta grafo
    if(strcmp(addgraph, fgets(buffer,20,stdin))){
        //printf("Comando inviato correttamente\n ");
        addGraph(n);
    }
    return 0;
}

void addGraph(int n){
    int **g;
    int j=0;
    char w[MAXWEIGHT];
    char *row;
    char *buffer;
    row=malloc(sizeof(int )* MAXBUFFER);
    buffer=malloc(sizeof(int )* MAXBUFFER);
    g=(int**) malloc(sizeof(int)*n*n);
    for(int i=0;i<n;i++){
        strcpy(row, fgets(buffer,MAXBUFFER,stdin));
        for(int k=0;row[k]!=' ';k++){
            if(row[k]!=','){
                w[k]=row[k];
            }
        }
        g[j][i]= atoi(w);
        j++;
    }
    free(row);
    free(buffer);
    }
