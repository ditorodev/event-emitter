# event-emitter
Emisor de eventos en C

## USO
En el main adjunto se encuentra un ejemplo de como usar las funciones.

- Clonar el repositorio.
- Crear tu propio main.
- #include "event-emitter.h"
- gcc event-emitter.c ejemplo.c -l pthread -o example
- ./example

## EXPLICACION DE LAS FUNCIONES
- Permitir crear un centro de notificaciones que se diferencie del resto mediante un nombre.

          void newCenter(char * name)

- Asociar un centro de notificaciones “A” a otro “B” mediante la escucha de un evento en particular
de tal forma que cuando el centro A genere dicho evento el centro B lo replique a todos los que
están asociados a el.

          void observeCenterFromCenter(char * centerA,
          char * centerB,
          char * event)

- Asociar un cliente (cualquier cosa) a un evento en un centro en particular. De tal forma que
cuando dicho evento se genere el cliente sea notificado con todos los parámetros asociados a
dicho evento. Solo sender puede ser NULL e indica que se espera dicha notificación de
cualquiera que la mande.

          void observeCenterFromClient(char * centerA,
          void * client,
          void * sender,
          char * event,
          void (* methodToCall) (void *))

- Emitir un evento determinado en un centro de notificaciones en particular. Solo params puede ser
NULL lo que indica que no se desea pasar información adicional con el evento.

          void post(char *center,
          char *event,
          void *sender,
          void *params)

- Emitir un evento después de n milisegundos en un centro de notificaciones en particular.

          void postDelayed(char *center,
          char *event,
          void *sender,
          void *params, int miliseconds)

- Desasociar un cliente de un evento en particular en un centro de notificaciones.

          void removeObserver(char * center, void * client, char * event)

- Desasociar un cliente de todos los eventos en un centro en particular.

          void removeObservers(char * center, void * client)

- Desasociar un cliente de todos los eventos en todos los centros de notificaciones.

          void removeAllObservers(void * client)

- Desasociar un centro de notificaciones de un evento en particular que este escuchando de otro.

         void removeObserverFromCenter(char * centerA,
         char * centerB,
         char * event)

- Desasociar un centro de notificaciones de otro al que este “escuchando”.

         void removeObserversFromCenter(char * centerA, char * centerB)

- Eliminar un centro de notificaciones.

         void removeCenter(char * center)
 