#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MAXBUFFER 10
int main() {

    char buffer[MAXBUFFER];
    char *firstRow,*numOfNodes,*kgraph;
    int i;
    int j=0;
    int n;

    int k;

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

    return 0;
}
