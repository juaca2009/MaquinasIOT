

#include    <stdio.h>
#include    <unistd.h>
#include    <string.h>
#include    <errno.h>
#include 	"phtrdsMsg.h"

#define 	NUM_MACH 4


static void *Maquina();
static void *CRM();
static void *Nube();
static void *Environment();


int main ( void )
{
  pthread_t   env_tid;                    /* Env tid */
  pthread_t   CRM_tid;               /* CRM process tid */
  pthread_t   Nube_tid;                    /* Nube tid */

  /* Create queues */
  iniciarColas();

  /* Create threads */
  pthread_create ( &env_tid, NULL, pEnvironment, NULL );
  pthread_create ( &CRM_tid, NULL, CRM, NULL );
  pthread_create ( &Nube_tid, NULL, Nube, NULL );
  

  /* Wait for threads to finish */
  pthread_join ( env_tid, NULL );
  pthread_join ( CRM_tid, NULL );
  pthread_join ( Nube_tid, NULL );

  /* Destroy queues */
  destruirColas ();

  return ( 0 );
}
