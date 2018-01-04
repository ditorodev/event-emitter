
typedef struct client node_c;
typedef struct event node_e;
typedef struct center Centro;
typedef struct centerList node_center;
typedef struct senderList node_s;

struct client {
    void *client;
    void (*f) (void *);
    struct client *next;
};

struct event {
    char name[100];
    node_s *senders;
    node_c *clients;
    node_center *centers;
    struct event *next;
};

struct center {
    char name[100];
    node_e *events;
    struct center *next;
};

struct centerList{
    Centro *center;
    struct centerList *next;
};

struct senderList {
    void *sender;
    node_c *clients;
    struct senderList *next;
};

void newCenter(char *name);
void observeCenterFromCenter (char *centerA, char *centerB, char *event);
void observeCenterFromClient(char *centerA, void *client, void *sender, char *event, void (* methodToCall)(void *));
void removeObserver(char *center, void *client, char *event);
void removeObservers(char *center, void *client);
void removeAllObservers(void * client);
void removeObserverFromCenter(char *centerA, char *centerB, char *event);
void removeObserversFromCenter(char * centerA, char * centerB);
void removeCenter (char *center);

void imprimirCentros(void);