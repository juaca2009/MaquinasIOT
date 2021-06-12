#include "phtrdsMsg.h"

Qmensaje queque[NUM_COLAS];

static void ponerMensaje(Qmensaje *_Qmensaje, Smensaje _mensaje){
    pthread_mutex_lock(&(_Qmensaje->buffer_lock));
    _Qmensaje->buffer[_Qmensaje->bufin] = _mensaje;
    _Qmensaje->bufin = (_Qmensaje->bufin + 1) % BUFSIZE;
    pthread_mutex_unlock(&(_Qmensaje->buffer_lock));
}

static Smensaje obtenerMensaje(Qmensaje *_Qmensaje){
    Smensaje msg;
    pthread_mutex_lock(&(_Qmensaje->buffer_lock));
    msg = _Qmensaje->buffer[_Qmensaje->bufout];
    _Qmensaje->bufout = (_Qmensaje->bufout + 1) % BUFSIZE;
    pthread_mutex_unlock(&(_Qmensaje->buffer_lock));
    return (msg);
}

void iniciarColas(void){
    int i;
    for (i=0; i<NUM_COLAS; i++){
        queque[i].bufin = 0;
        queque[i].bufout = 0;
        pthread_mutex_init(&(queque[i].buffer_lock), NULL);
        sem_init(&(queque[i].items), CRM, 0);
        sem_init(&(queque[i].items), CRM, 0);
        sem_init(&(queque[i].slots), CRM, BUFSIZE);
    }    
}

void destruirColas(void){
    int i;
    for (i = 0; i < NUM_COLAS; i++){
        pthread_mutex_destroy(&(queque[i].buffer_lock));
        sem_destroy(&(queque[i].items));
        sem_destroy(&(queque[i].slots));
    }
}

void enviarMensaje(Qmensaje *_Qmensaje, Smensaje _mensaje){
    sem_wait(&(_Qmensaje->slots));
    ponerMensaje(_Qmensaje, _mensaje);
    sem_post(&(_Qmensaje->items));
}

Smensaje recibirMensaje(Qmensaje *_Qmensaje){
    Smensaje msg;
    sem_wait(&(_Qmensaje->items));
    msg = obtenerMensaje(_Qmensaje);
    sem_post(&(_Qmensaje->slots));
    return (msg);
}
