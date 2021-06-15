#include <pthread.h>
#include <semaphore.h>
#include "pMaquinasConf.h"

#define CONF 0

typedef struct{
    Smensaje buffer[BUFSIZE];
    int bufin;
    int bufout;
    pthread_mutex_t buffer_lock;
    sem_t items;
    sem_t slots;
} Qmensaje;

extern Qmensaje queque [NUM_COLAS];

void iniciarColas(void);
void destruirColas(void);
void enviarMensaje(Qmensaje *_Qmensaje, Smensaje _mensaje);
Smensaje recibirMensaje(Qmensaje *_Qmensaje);
