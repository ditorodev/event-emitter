#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void error(int n){
    switch(n){
        case 1:
            printf("\n ERROR: Uno de los centros no existe \n");
            break;

    }
}

Centro *CENTROS;

void imprimirCentros(void) {
    node_center *aux1;
    Centro *aux;
    node_e *aux2;

    aux = CENTROS;
    while(aux){
        printf("\n Centros asociados a %s \n", aux->name);
        if(!(aux->events)){ 
            aux = aux->next;
            continue;
            }
        aux2 = aux->events;
        while(aux2){
            printf("  CENTROS REGISTRADOS AL EVENTO %s", aux2->name);
            aux1 = aux2->centers;
            while(aux1){
                printf("\n   %s \n", aux1->center->name);
                //printf("\n   %s \n", (aux1->center) == (aux1->next->center)?"iguales":"distintos");
                aux1 = aux1->next;
            }
            aux2 = aux2->next;
        }
        aux = aux->next;
    }

}

void newCenter(char *name) {

    Centro *new = (Centro *) malloc(sizeof(Centro));
    if(!CENTROS){ // NOS ASEGURAMOS QUE EXISTA ALGUIEN EN LA LISTA DE CENTROS
        CENTROS = new;
        strcpy(new->name, name);
        return;
    }

    Centro *aux = CENTROS;
    strcpy(new->name, name);
    aux = CENTROS;
    while(aux->next) aux = aux->next;
    aux->next = new;
}

Centro * locateCenter(char *center){
    Centro *aux = CENTROS;
    
    while(aux && strcmp(center, aux->name)) aux = aux->next;    
    return aux;
}
node_e * locateEvent(Centro *center, char *event){
    node_e *aux = center->events;

    while(aux && strcmp(event, aux->name)) aux = aux->next;
    printf("\n EVENTO LOCALIZADO %s \n", aux->name);
    return aux;
}

void observeCenterFromCenter (char *centerA, char *centerB, char *event){
    Centro *cA = locateCenter(centerA);
    Centro *cB = locateCenter(centerB);
    node_e *aux;
    node_e *new = locateEvent(cA, event);
    node_center *aux1;
    if(!cA || !cB){ 
        error(1);
        return;
    }
    // ASOCIAMOS EL EVENTO AL CENTRO A
    aux = cA->events;
    if(!new){  
        new = (node_e *)malloc(sizeof(node_e));
        strcpy(new->name, event);
        if(!aux) cA->events = new;
        else {
            while(aux->next) aux = aux->next;
            aux->next = new;
        }
    }
        
    // ASOCIAMOS EL CENTRO B AL EVENTO
    aux1 = new->centers;
    if(!aux1){ 
        aux1 = (node_center *)malloc(sizeof(node_center));
        aux1->center = cB;
        new->centers = aux1;    
    }else{
        while(aux1->next){ 
            printf("\n  %s  \n", aux1->center->name);
            aux1 = aux1->next;
        } 
        aux1->next = (node_center *) malloc(sizeof(node_center));
        aux1->next->center = cB;
    }
}

// void observeCenterFromClient(char *centerA, void *client, void *sender, char *event, void (* methodToCall)(void *)){
//     Centro *c = locateCenter(centerA);
//     node_e *new = locateEvent(c, event);
//     node_s *aux_s;
//     if (!new) new = (node_e *) malloc(sizeof(node_e));
//     else{
//         aux_s = 

//     }




// }

int main (void) {
    char nombre[100];
    int i = 0;
    for(; i <5; i++){
        scanf("%100s", nombre);
        newCenter(nombre);
    };

    Centro *aux = CENTROS;
    while(aux) {
        printf("---> %s", aux->name);
        aux = aux->next;
    }

    printf("\nOBSERVE CENTER FROM CENTER\n");
    observeCenterFromCenter("A", "B", "hola");
    observeCenterFromCenter("A", "D", "hola");
    observeCenterFromCenter("A", "C", "chao");

    imprimirCentros();
    observeCenterFromCenter("A", "D", "chao");
    imprimirCentros();
    return 0;
}