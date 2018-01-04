#include "event-emitter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(int n){
	printf("\n ERROR: ");
	switch(n){
			case 1:
					printf("Uno de los centros no existe ");
					break;
			case 2:
					printf("El evento es nulo o no existe ");
					break;
			case 3:
					printf("El cliente no puede ser NULL ");
					break;
			
	}
	printf("\n");
}
Centro *CENTROS;

void deleteList_event(node_e *event){
	if(event == NULL) return;
	deleteList_event(event->next);
	free(event); 
}

void deleteList_sender(node_s *sender){
	if(sender == NULL) return;
	deleteList_sender(sender->next);
	free(sender); 
}

void deleteList_client(node_c *client){
	if(client == NULL) return;
	deleteList_client(client->next);
	free(client); 
}

void deleteList_nodeCenter(node_center *c){
	if(c == NULL) return;
	deleteList_nodeCenter(c->next);
	free(c); 
}


void removeFromList_c(node_c **list, void *del) {
	if(!*list) return;

	node_c *aux = *list;
	node_c *prev = *list;
	
	while(aux && aux->client != del){ 
			prev = aux;
			aux = aux->next;
	}
	if(aux == *list) *list = (*list)->next;
	if(aux && aux->client == del){
			if(prev) prev->next = aux->next;
			free(aux);
	}

}

void removeFromList_center(node_center **list, Centro *del){
	if(!*list) return;

	node_center *aux = *list;
	node_center *prev = *list;

	while(aux && aux->center != del){ 
			prev = aux;
			aux = aux->next;
	}
	if(aux == *list) *list = (*list)->next;
	if(aux && aux->center == del){
			if(prev) prev->next = aux->next;
			free(aux);
	}
}

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
	
	while(aux && aux->sender != sender) aux = aux->next;
	return aux;
}
// Centro B observa al centro A
void observeCenterFromCenter (char *centerA, char *centerB, char *event){
	Centro *cA = locateCenter(centerA);
	Centro *cB = locateCenter(centerB);
	if(!cA || !cB){ 
					error(1);
					return;
	}

	node_e *aux = NULL;
	node_e *eventToSub = locateEvent(cA, event);
	node_center *aux1 = NULL;
	
	

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
			if(aux1->center == cB) return; //CHEQUEAMOS QUE EL PRIMER CENTRO DE LA LISTA NO SEA EL MISMO QUE VAMOS A AGREGAR
			while(aux1->next){ 
					//printf("\n  %s  \n", aux1->center->name);
					if(aux1->center == cB) return; // CHEQUEAMOS QUE EL CENTRO QUE QUEREMOS AGREGR NO SE ENCUENTRE EN LA LISTA
					aux1 = aux1->next;
			} 
			aux1->next = (node_center *) malloc(sizeof(node_center));
			aux1->next->center = cB;
			aux1->next->next = NULL;
	}
}

void observeCenterFromClient(char *centerA, void *client, void *sender, char *event, void (* methodToCall)(void *)){
		if (!client){
			error(3);
			return;
	}

	Centro *c = locateCenter(centerA);
	if(!c){
			error(1);
			return;
	}

	node_e *eventToSub = locateEvent(c, event);
	node_s *senderToSub = NULL;
	node_s *p_sender = NULL;
	node_c *p_client = NULL;
	node_c *clientToSub = (node_c *) malloc(sizeof(node_c));
	
	clientToSub->client = client;
	clientToSub->f = methodToCall;
	
	
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
	// CHEQUEAMOS QUE EL PRIMER ELEMENTO DE LA LISTA NO SEA IGUAL AL CLIENTE
	if(p_client->client == client) return;
	
	// AHORA AGREGAMOS EL CLIENTE QUE ESCUCHA AL SENDER EN LA LISTA CORRESPONDIENTE
	while(p_client->next){
			if(p_client->client == client) return; // CHEQUEAMOS QUE EL CLIENTE NO ESTE EN LA LISTA
			p_client = p_client->next;
	} 
	p_client->next = clientToSub;
	clientToSub->next = NULL;
	
}

void removeObserver(char *center, void *client, char *event){
	Centro *c = locateCenter(center);
	if(!c) {
			error(1);
			return;
	}

	node_e *e = locateEvent(c, event);
	if(!e) {
			error(2);
			return;
	}

	// ELIMINAMOS DE LA LISTA DE CLIENTES QUE ESTAN SUSCRITOS SIN NINGUN SENDER EN ESPECIFICO
	removeFromList_c(&e->clients, client);

	// ELIMINAMOS DE LA LISTA DE CADA UNO DE LOS SENDERS A LOS QUE ESTA SUSCRITO
	node_s *p_sender = e->senders;
	node_s *prev_sender = e->senders;

	while(p_sender) {
			removeFromList_c(&p_sender->clients, client);

			// SI EL SENDER NO TIENE CLIENTES LO ELIMINAMOS DE LA LISTA
			if(!p_sender->clients) {
					if(p_sender == e->senders) {
							e->senders = p_sender->next;
							free(p_sender);
							p_sender = e->senders;
							continue;
					}
					else {
							prev_sender->next = p_sender->next;
							free(p_sender);
							p_sender = prev_sender->next;
							continue;
					} 
			}
			prev_sender = p_sender;
			p_sender = p_sender->next;
	}

}

void removeObservers(char *center, void *client) {
	Centro * c = locateCenter(center);
	if(!c) {
			error(1);
			return;
	}

	node_e *p_event = c->events;
	node_e *prev = c->events;

	// RECORREMOS TODOS LOS EVENTOS DEL CENTRO Y VAMOS ELIMINANDO EL CLIENTE DE CADA UNO 
	// DE ELLOS CON LA FUNCION removeObserver
	while(p_event){
			removeObserver(center, client, p_event->name);
			// SI EL EVENTO NO TIENE CLIENTES NI SENDERS NI CENTROS LO ELIMINAMOS DE LA LISTA
			if(!p_event->clients && !p_event->senders && !p_event->centers) {
					if(p_event == c->events) {
							c->events = p_event->next;
							free(p_event);
							p_event = c->events;
							continue;
					}
					else {
							prev->next = p_event->next;
							free(p_event);
							p_event = prev->next;
							continue;
					}
			}
			prev = p_event;
			p_event = p_event->next;
			
	}

}

void removeAllObservers(void * client){
	Centro *c = CENTROS;

	while(c){
			removeObservers(c->name, client);
			c = c->next;
	}

}
// Desasociar Centro B de un evento en el Centro A
void removeObserverFromCenter(char *centerA, char *centerB, char *event) {
	Centro *cA = locateCenter(centerA);
	Centro *cB = locateCenter(centerB); 
	if(!cA || !cB) {
		error(1);
		return;
	}

	node_e *p_event = locateEvent(cA, event);
	if(!p_event) {
		error(2);
		return;
	}

	removeFromList_center(&p_event->centers, cB);

}

void removeObserversFromCenter(char * centerA, char * centerB){
	Centro *cA = locateCenter(centerA);
	if(!cA){
		error(1);
		return;
	}
	node_e *p_event = cA->events;
	while(p_event){
		removeObserverFromCenter(centerA, centerB, p_event->name);
		p_event = p_event->next;
	}
}

void removeCenter (char *center) {
	if(!locateCenter(center)){
		error(1);
		return;
	}

	Centro *c, *prev;
	c = prev = CENTROS;
	// ELIMINAMOS EL CENTRO DE LA LISTA GLOBAL
	if(!strcmp(c->name, center)) {
		CENTROS = CENTROS->next;
		return;
	}else {
		while(c && strcmp(c->name, center)){
			prev = c;
			c = c->next;
		}
		prev->next = c->next;
	}

	deleteList_client(c->events->clients); // CLIENTES CON SENDER NULL
	deleteList_nodeCenter(c->events->centers); // CENTROS
	node_s *aux = c->events->senders;
	while(aux) { // CLIENTES DE LOS SENDERS
		deleteList_client(aux->clients);
		aux = aux->next;
	}
	deleteList_sender(c->events->senders); // SENDERS
	deleteList_event(c->events); // EVENTOS
	free(c);
}