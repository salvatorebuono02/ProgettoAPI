#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#define MAXBUFFER 100
#define MAXWEIGHT 100
void addGraph(int);
//void inserisciDistInCoda(int**);
void fromStrToInt(char*);
int main() {

    char buffer[MAXBUFFER];
    char buffer2[MAXBUFFER];
    char *firstRow,*numOfNodes,*kgraph;
    int i;
    int j=0;
    int n;

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
    printf("Type AggiungiGrafo\n");
    //gestione di aggiunta grafo
    if(strncmp("AggiungiGrafo", fgets(buffer2,20,stdin),13)==0){
        printf("Comando inviato correttamente\n ");
        addGraph(n);
    }
    return 0;
}

void addGraph(int n){
    int **g;
    char *buffer;
    char*row;
    char w[MAXWEIGHT];
    int col;


    g=(int**) malloc(sizeof(int*)*n);
    buffer= malloc(sizeof(char)*MAXBUFFER);
    row= malloc(sizeof(char)*MAXBUFFER);
    for(int i=0;i<n;i++){
        col=0;
        int j=0;
        g[i]= malloc(sizeof (int)*n);
        row=fgets(buffer,MAXBUFFER,stdin);
        printf("ROW: %ld\n",strlen(row));
        //isolo il peso
        for (int k = 0;k< strlen(row); k++) {
            if(row[k]==','|| k==strlen(row)-1) {
                j=0;

                g[i][col]=atoi(w);
                col++;

            }else{
                w[j]=row[k];
                printf("W: %s\n",w);
                j++;

            }


        }

    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            printf("%d ",g[i][k]);
        }
        printf("\n");
    }

    //inserisciDistInCoda(g);

}
