#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void error(int n){
    printf("\n ERROR: ");
    switch(n){
        case 1:
            printf("Uno de los centros no existe \n");
            break;
        case 2:
            printf("El cliente es nulo o no existe \n");
            break;
    }
}

Centro *CENTROS;

void imprimirCentros(void) {
    node_center *aux1;
    Centro *aux;
    node_e *aux2;
    node_s *aux3;
    node_c *aux4;

    aux = CENTROS;
    while(aux){
        printf("\n Centros asociados a %s \n", aux->name);
        if(aux->events){ 
            // aux = aux->next;
            // continue;
            aux2 = aux->events;
            while(aux2){
                printf("-->Centros registrados al evento %s\n", aux2->name);
                aux1 = aux2->centers;
                while(aux1){
                    printf("\n---->%s \n", aux1->center->name);
                    //printf("\n   %s \n", (aux1->center) == (aux1->next->center)?"iguales":"distintos");
                    aux1 = aux1->next;
                }
                printf("\n-->Clientes registrados al evento %s \n", aux2->name);
                if(aux2->senders){
                    aux3 = aux2->senders;
                    while(aux3){
                        printf("\n----> Sender %p\n", aux3->sender);
                        aux4 = aux3->clients;
                        while(aux4){
                            printf("\n------> %p\n", aux4->client);
                            aux4 = aux4->next;
                        }
                        aux3 = aux3->next;
                    }
                }
                if(aux2->clients){
                    aux4 = aux2->clients;
                    while(aux4){
                        printf("\n----> %p\n", aux4->client);
                        aux4 = aux4->next;
                    }
                }
                aux2 = aux2->next;
            }
            
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
    return aux;
}

node_s * locateSender(node_e *event, void *sender){
    node_s *aux = event->senders;
    
    //printf("\n Sender = %p \n", sender);
    while(aux && aux->sender != sender) aux = aux->next;
    return aux;
}

void observeCenterFromCenter (char *centerA, char *centerB, char *event){
    Centro *cA = locateCenter(centerA);
    Centro *cB = locateCenter(centerB);
    node_e *aux;
    node_e *eventToSub = locateEvent(cA, event);
    node_center *aux1;
    
    if(!cA || !cB){ 
        error(1);
        return;
    }

    // ASOCIAMOS EL EVENTO AL CENTRO A
    aux = cA->events;
    if(!eventToSub){  
        eventToSub = (node_e *)malloc(sizeof(node_e));
        strcpy(eventToSub->name, event);
        if(!aux) cA->events = eventToSub;
        else {
            while(aux->next) aux = aux->next;
            aux->next = eventToSub;
        }
    }
        
    // ASOCIAMOS EL CENTRO B AL EVENTO
    aux1 = eventToSub->centers;
    if(!aux1){ 
        aux1 = (node_center *)malloc(sizeof(node_center));
        aux1->center = cB;
        eventToSub->centers = aux1;    
    }else{
        while(aux1->next){ 
            printf("\n  %s  \n", aux1->center->name);
            aux1 = aux1->next;
        } 
        aux1->next = (node_center *) malloc(sizeof(node_center));
        aux1->next->center = cB;
    }
}

void observeCenterFromClient(char *centerA, void *client, void *sender, char *event, void (* methodToCall)(void *)){
    Centro *c = locateCenter(centerA);
    node_e *eventToSub = locateEvent(c, event);
    node_s *senderToSub;
    node_s *p_sender;
    node_c *p_client;
    node_c *clientToSub = (node_c *) malloc(sizeof(node_c));

    clientToSub->client = client;
    
    if (!client){
        error(2);
        return;
    }
    if(!c){
        error(1);
        return;
    }
    // SI EL EVENTO NO EXISTE, LO AGREGAMOS A LA LISTA DE EVENTOS EN EL CENTRO
    if (!eventToSub){
        eventToSub = (node_e *) malloc(sizeof(node_e));
        strcpy(eventToSub->name, event);
        node_e *aux = c->events;
        if(!aux) c->events = eventToSub;
        else { 
            while(aux->next) aux = aux->next;
            aux->next = eventToSub;
        }
    }
    // SI HAY UN SENDER VINCULADO AL EVENTO, AGREGAMOS EL SENDER EN EL EVENTO. EN CASO DE QUE
    // EL SENDER SEA (NULL) AGREGAMOS EL CLIENTE A LA LISTA DE CLIENTES    
    if(sender){
        // YA QUE AGREGAMOS EL EVENTO AL CENTRO, CREAMOS LA LISTA DE ESTE SENDER
        senderToSub = locateSender(eventToSub, sender);
        // SI EL SENDER NO EXISTE DENTRO DEL EVENTO, LO AGREGAMOS A LA LISTA DE SENDERS DEL EVENTO
        if(!senderToSub) {
            senderToSub = (node_s *)malloc(sizeof(node_s));
            senderToSub->sender = sender;

            p_sender = eventToSub->senders;
            if(!p_sender) eventToSub->senders = senderToSub;
            else {
                while(p_sender->next) p_sender = p_sender->next;
                p_sender->next = senderToSub;
            }
        }
        //AGREGAMOS EL CLIENTE A LA LISTA DEl SENDER
        p_client = senderToSub->clients;
        if(!p_client){
             senderToSub->clients = clientToSub;
             return;
        }
    } else{ 
        p_client = eventToSub->clients; //AGREGAMOS EL CLIENTE A LA LISTA DEL EVENTO
        if(!p_client) {
            eventToSub->clients = clientToSub;
            return;
        }
    }
    // AHORA AGREGAMOS EL CLIENTE QUE ESCUCHA AL SENDER EN LA LISTA CORRESPONDIENTE
    while(p_client->next) p_client = p_client->next;
    p_client->next = clientToSub;


}

void eventToFire (void *a) {
    printf("\n El evento ha sido disparado con %s \n", (char *)a);
}

int main (void) {
    char nombre[100];
    int i = 0;
    int a,b,c,d,e;
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
    observeCenterFromClient("A", &i, NULL, "hola", eventToFire);
    observeCenterFromClient("A", &b, &c, "hola", eventToFire);
    observeCenterFromClient("A", &c, NULL, "hola", eventToFire);
    observeCenterFromClient("A", &a, &b, "hola", eventToFire);
    observeCenterFromClient("A", &a, &c, "hola", eventToFire);
    observeCenterFromCenter("A", "D", "chao");
    imprimirCentros();
    return 0;
}