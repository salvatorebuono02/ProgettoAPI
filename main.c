#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <limits.h>

#define MAXBUFFER 100000000
#define MAXWEIGHT 10
void addGraph();
void inserisciDistInCoda(int**);
int myAtoi(char*);
int minDist(int *, int *);

void aggiornaCoda(int );

void checkQ();

typedef struct coda{
    int index_g;
    int sum_path;
    struct coda *next;

}coda;
coda* returnElem(int ,coda*);

void changePos(int,int ,coda*);

void printBestGraph(int );


coda *copyList(coda *);

coda* heapify(coda *, int );
int size(coda*);

coda* delete(coda *, coda *);

//void printList(coda *);

void ordina(int *a, int i);

int n;
coda *q=NULL;
int num_prog=0;
char *buffer;

int main() {


    char *numOfNodes,*kgraph;
    int i;
    int j=0;

    int k;
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
    while(strncmp("TopK",buffer,4)!=0){

        if(strncmp("AggiungiGrafo",buffer,13)==0)
            addGraph();
        buffer= fgets(buffer,20,stdin);
    }
    printBestGraph(k);
    free(buffer);
    free(q);
    return 0;
}

void printBestGraph(int k) { //esegue k volte il cancella-min su q2
    coda* q2=copyList(q);
    //printList(q2);
    int *min= malloc(sizeof(int)*k);
    for (int i = 0; i < k; ++i){
        min[i]= returnElem(0,q2)->index_g;
        //printf("Size pre delete %d\n", size(q2));
        q2=changePos(0, size(q2)-1,q2);
        //printf("val nuova testa %d\n", returnElem(0,q2)->index_g);
        q2=delete(returnElem(size(q2) - 1, q2), q2);
        //printf("Size post delete %d\n", size(q2));
        q2=heapify(q2, 0);
        if(i<k-1)
            printf("%d ",min[i]);
        else
            printf("%d\n",min[i]);

    }
    #ifndef EVAL
    ordina(min, k);
    for (int i = 0; i < k; ++i) {
        if(i<k-1)
            printf("%d ",min[i]);
        else
            printf("%d\n",min[i]);

    }
    #endif
    free(min);
}

void ordina(int *a, int dim) {
int tmp;
    for (int i = 0; i < dim; ++i) {
        tmp=a[i];
        int j=i-1;
        while (j>=0 && a[j]>tmp){
            a[j+1]=a[j];
            j=j-1;
            a[j+1]=tmp;
        }
    }
}
/*
void printList(coda *p) {
    coda *tmp=p;
    while (tmp!=NULL){
        printf("Index=%d Sum=%d\n",tmp->index_g,tmp->sum_path);
        tmp=tmp->next;
    }
}
 */

coda* delete(coda *el_del, coda *p){
    coda *tmp=p,*prev=NULL;
    if(tmp!=NULL && tmp==el_del){
        p=tmp->next;
        free(tmp);
        return p;
    }

    while(tmp!=NULL && tmp!=el_del){
        prev=tmp;
        tmp=tmp->next;
    }

    prev->next=tmp->next;
    free(tmp);
    return p;
}


coda* heapify(coda *pCoda, int i) {
    int l=(2*i)+1;
    int r=(2*i)+2;
    int posmin;
    if(l<size(pCoda) && (returnElem(l,pCoda)->sum_path<returnElem(i,pCoda)->sum_path||(returnElem(l,pCoda)->sum_path==returnElem(i,pCoda)->sum_path && returnElem(l,pCoda)->index_g <
            returnElem(i,pCoda)->index_g)))
        posmin=l;
    else
        posmin=i;
    if(r<size(pCoda) && (returnElem(r,pCoda)->sum_path<returnElem(posmin,pCoda)->sum_path ||(returnElem(r,pCoda)->sum_path==returnElem(posmin,pCoda)->sum_path && returnElem(r,pCoda)->index_g <
    returnElem(posmin,pCoda)->index_g)))
        posmin=r;
    if(posmin != i){
        pCoda=changePos(i, posmin,pCoda);
        pCoda=heapify(pCoda, posmin);
    }
    return pCoda;
}

coda *copyList(coda *h) {
    coda *tmp=h;
    coda *newh=NULL;
    coda *last=NULL;

    while (tmp!=NULL){
        if(newh==NULL){
            newh= malloc(sizeof(coda));
            newh->sum_path=tmp->sum_path;
            newh->index_g=tmp->index_g;
            newh->next=NULL;
            last=newh;
        }
        else{
            last->next= malloc(sizeof(coda));
            last=last->next;
            last->sum_path=tmp->sum_path;
            last->index_g=tmp->index_g;
            last->next=NULL;
        }
        tmp=tmp->next;
    }
    last=last->next;
    free(last);
    return newh;
}


void addGraph(){
    int **g;
    char w[MAXWEIGHT]="0";
    int col;


    g=(int **) malloc(sizeof(int*)*n);
    for(int i=0;i<n;i++){
        col=0;
        int j=0;
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

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            //printf("%u ",g[i][k]);
        }
        //printf("\n");
    }

    inserisciDistInCoda(g);
    for (int i = 0; i < n; ++i) {
        free(g[i]);
    }
    free(g);
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
        dist[i]=INT_MAX,spSet[i]=0;
    }
    dist[0]=0;

    for (int i = 0; i < n-1; ++i) {
        int minv=minDist(dist,spSet);

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
    aggiornaCoda(distfrom0);
}

int minDist(int *dist, int *spSet) {
    int min=UINT_MAX, i_min=0;
    for (int i = 0; i < n; ++i) {
        if(dist[i]<=min && spSet[i]==0)
            min=dist[i],i_min=i;
    }
    return i_min;
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
    //printf("Elem inserito correttamente, ho inserito %d grafi\n",num_prog);
    checkQ();
    last=q;
    while (last!=NULL){
        //printf("Index: %d Sum Of Path %d\n",last->index_g,last->sum_path);
        last=last->next;
    }
}

void checkQ() {
    //printf("Sono in checkQ\n");

    int i=num_prog-1;
    //printf("NUMPROG %d\n",num_prog);
    while (i>0 && (returnElem((i-1)/2,q)->sum_path)>(returnElem(i,q)->sum_path)){

        q=changePos((i-1)/2,i,q);
        //printf("Fine change pos\n");
        i=(i-1)/2;
    }

}

void changePos(int i1,int i2,coda *q1) {
    coda *tmp;
    coda *elem1= returnElem(i1,q1);
    coda *elem2= returnElem(i2,q1);
    coda*prec1,*prec2;

    prec1= returnElem(i1-1,q1);
    prec2= returnElem(i2-1,q1);

    if(prec1!=NULL)
        prec1->next=elem2;
    if(prec2!=NULL)
        prec2->next=elem1;
    tmp=elem1->next;
    //printf("TMP %d\n",tmp->next->index_g);
    elem1->next=elem2->next;
    elem2->next=tmp;
    if(prec1==NULL)
        q1=elem2;
    else if (prec2==NULL)
        q1=elem1;
    //printf("pos changed\n");
}

coda* returnElem(int index,coda*q1){
    coda *last=q1;
    if(index<0)
        return NULL;
    else{
        for (int i = 0; i < index; ++i) {
        last=last->next;
        }
        //printf("fine return elem %d\n",last->index_g);
        return last;
    }

}
int size(coda* w){
    int c=0;
    coda *tmp=w;
    while (tmp!=NULL){
        c++;
        tmp=tmp->next;
    }
    return c;
}