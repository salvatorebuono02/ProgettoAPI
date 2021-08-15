#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#define MAXBUFFER 100
#define MAXWEIGHT 100
void addGraph();
void inserisciDistInCoda(int**);
int myAtoi(char*);
int minDist(int *, int *);

void aggiornaCoda(int distfrom0);

void checkQ();



typedef struct coda{
    int index_g;
    int sum_path;
    struct coda *next;

}coda;
coda *returnElem(int );

void changePos(int,int );

void printBestGraph(int k);

int n;
coda *q=NULL;
int num_prog=0;

int main() {

    char *buffer;
    char *numOfNodes,*kgraph;
    int i;
    int j=0;

    int k;

    //lettura prima riga
    buffer=(char *) malloc(sizeof(char )*MAXBUFFER);
    numOfNodes=(char *) calloc(MAXBUFFER,sizeof (char ));
    if(!numOfNodes){
        return -1;
    }
    kgraph=(char *) calloc(MAXBUFFER,sizeof(char ));
    strcpy(buffer,fgets(buffer,10,stdin));
    printf("First row  : %s", buffer);

    for(i=0;buffer[i]!=',';i++){
        if(buffer[i]=='\0')
            break;
        numOfNodes[i]=buffer[i];
    }
    n= myAtoi(numOfNodes);
    free(numOfNodes);
    printf("Num of nodes are: %d\n",n);
    for(i++; isdigit(buffer[i]);i++){
        kgraph[j]=buffer[i];
        j++;
    }

    k= myAtoi(kgraph);
    free(kgraph);
    printf("You want the best %d graphs\n",k);

    printf("Type AggiungiGrafo\n");
    //gestione di aggiunta grafo
    buffer= fgets(buffer,20,stdin);
    while(strncmp("TopK",buffer,4)!=0){

        if(strncmp("AggiungiGrafo",buffer,13)==0)
            addGraph();
        buffer= fgets(buffer,20,stdin);
    }
    printBestGraph(k);
    free(buffer);
    return 0;
}

void printBestGraph(int k) {
    coda *tmp=q;
    for (int i = 0; i < k; ++i) {
        printf("%d\t",tmp->index_g);
        tmp=tmp->next;
    }
}

void addGraph(){
    int **g;
    char *buffer;
    char w[MAXWEIGHT];
    int col;


    g=(int**) malloc(sizeof(int*)*n);
    buffer=(char *) malloc(sizeof(char)*MAXBUFFER);
    for(int i=0;i<n;i++){
        col=0;
        int j=0;
        g[i]= malloc(sizeof (int)*n);
        buffer=fgets(buffer,MAXBUFFER,stdin);
        printf("ROW: %ld\n",strlen(buffer));
        //isolo il peso
        for (int k = 0;k< strlen(buffer); k++) {
            if(buffer[k]==','|| k==strlen(buffer)-1) {
                j=0;
                printf("W: %s\n",w);

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

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            printf("%d ",g[i][k]);
        }
        printf("\n");
    }

    inserisciDistInCoda(g);
    free(buffer);
}

int myAtoi(char* s)
{
    int res = 0;


    for (int i = 0; s[i] != '\0'; ++i)
        res = res * 10 + s[i] - '0';

    return res;
}

void inserisciDistInCoda(int **g){

    int *dist;
    int *spSet; //insieme dei nodi visitati

    dist= malloc(sizeof (int)*n);
    spSet= malloc(sizeof (int )*n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i==j)
                g[i][j]=0;
        }
        dist[i]=MAXWEIGHT,spSet[i]=0;
    }
    dist[0]=0;

    for (int i = 0; i < n-1; ++i) {
        int minv=minDist(dist,spSet);

        spSet[minv]=1;
        for (int j = 0; j < n; ++j) {
            if(spSet[j]==0 && g[minv][j]>0 && dist[minv]!=MAXWEIGHT && dist[minv]+g[minv][j]<dist[j])
                dist[j]=dist[minv]+g[minv][j];
        }

    }
    int distfrom0=0;
    for (int i = 0; i < n; ++i) {
        distfrom0+=dist[i];
    }
    printf("%d\n",distfrom0);
    aggiornaCoda(distfrom0);
}

void aggiornaCoda(int distfrom0) { //inserisci in coda
    coda *last;
    coda* new_elem= malloc(sizeof(coda));
    new_elem->sum_path=distfrom0;
    new_elem->index_g=num_prog;
    num_prog++;
    if(q==NULL)
        q=new_elem;
    else{
        last=q;
        while(last->next!=NULL)
            last=last->next;
        last->next=new_elem;

    }
    printf("Elem inserito correttamente, ho inserito %d grafi\n",num_prog);
    checkQ();
    last=q;
    while (last!=NULL){
        printf("Index: %d Sum Of Path %d\n",last->index_g,last->sum_path);
        last=last->next;
    }
}

void checkQ() {
    printf("Sono in checkQ\n");

    int i=num_prog-1;
    printf("NUMPROG %d\n",num_prog);
    while (i>0 && (returnElem(i/2)->sum_path)>(returnElem(i)->sum_path)){

        changePos(i/2,i);
        printf("Fine change pos\n");
        i=i/2;
    }

}

void changePos(int i1,int i2) {
    coda *tmp=q;
    coda *elem1= returnElem(i1);
    coda *elem2= returnElem(i2);
    coda*prec1,*prec2;

    prec1= returnElem(i1-1);
    prec2= returnElem(i2-1);

    if(prec1!=NULL)
        prec1->next=elem2;
    if(prec2!=NULL)
        prec2->next=elem1;
    tmp=elem1->next;
    elem1->next=elem2->next;
    elem2->next=tmp;
    if(prec1==NULL)
        q=elem2;
    else if (prec2==NULL)
        q=elem1;
    printf("pos changed\n");
}


int minDist(int *dist, int *spSet) {
    int min=MAXWEIGHT, i_min=0;
    for (int i = 0; i < n; ++i) {
        if(dist[i]<=min && spSet[i]==0)
            min=dist[i],i_min=i;
    }
    return i_min;
}

coda* returnElem(int index){
    coda *last=q;
    if(index<0)
        return NULL;
    else{
        for (int i = 0; i < index; ++i) {
        last=last->next;
        }
        printf("fine return elem %d\n",last->sum_path);
        return last;
    }


}