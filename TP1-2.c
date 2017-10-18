#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void tri_bulle(int tab[], int MAX);

int main()
{
    int tab[100];
    int i=0,MAX;

    printf("Entrer le MAX de tableau, s'il vous plait.\n");
    scanf("%d",&MAX);
    printf("Entrer les elements de tableau desous, s'il vous plait.\n");
    for(i=0; i<MAX; i++){
        scanf("%d",&tab[i]);
    }
    tri_bulle(tab, MAX);
    for(i=0; i<MAX; i++){
        printf("%d\t",tab[i]);
    }
    return 0;
}

void tri_bulle(int tab[], int MAX){
    bool non_tri = true;
    int i=0,tmp=tab[0];
    while(i<MAX && non_tri==true){
        non_tri=false;
        for(int j=1; j<MAX-i; j++){
            if(tab[j]<tab[j-1]){
                tmp=tab[j-1];
                tab[j-1]=tab[j];
                tab[j]=tmp;
                non_tri=true;
            }
        }
    }
}
