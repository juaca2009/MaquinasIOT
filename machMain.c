

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


/* Environment thread */
static void *Environment ( void *arg )
{
  char  line [100];
  int   val,		/*dinero*/
  		maq,		/*num maquina*/
  		//sen,
  		tipo,		/*cliente o admin*/
  		prod,		/*numero del producto*/
  		decision,
		decision2,
		prodAñadir,
		prodSuplir,
		prodRemover;
  		
        /*Asub,
        Bsub;*/

  Smensaje OutMsg;
  
  
  
  for( ; ; ){
  	
	printf ( "Seleccione una de las maquinas (1..%d):\n", NUM_MACH );
    //printf ( "2. Enter Coin\n" );
    fflush ( stdout );
    fflush ( stdin );
    fgets ( line, sizeof (line), stdin );
    sscanf ( line, "%d", &maq );		/*choice*/
    
    
    printf ( "Es usted un administrador o un cliente? (admin=1 , cliente=0)\n" );
    //printf ( "2. Enter Coin\n" );
    fflush ( stdout );
    fflush ( stdin );
    fgets ( line, sizeof (line), stdin );
    sscanf ( line, "%d", &tipo );		/*tipo de persona*/
    
    if(tipo==0){
    	
    	for( ; ; ){
    		/*printf ( "1. Seleccione un producto: (1...5)\n" );*/
    		printf ( "1. Comprar\n" );
    		printf ( "2. Salir\n" );
		    fflush ( stdout );
		    fflush ( stdin );
		    fgets ( line, sizeof (line), stdin );
		    sscanf ( line, "%d", &decision );		/*choice*/
		    
		    
		    switch (decision){
				
				case 1:
					printf ( "Seleccione un producto (1...5):\n" );
			        fflush ( stdout );
			        fflush ( stdin );
			        fgets ( line, sizeof (line), stdin );
			        sscanf ( line, "%d", &prod);
			        
			        printf ( "Ingrese su dinero :\n" );
			        fflush ( stdout );
			        fflush ( stdin );
			        fgets ( line, sizeof (line), stdin );
			        sscanf ( line, "%d", &val);
			        
			        /*faltan señales y mensaje*/
			        OutMsg.maquina = maq;
			        OutMsg.noProducto =prod;
			        OutMsg.senal = enterCoin;
			        OutMsg.valor = val;
			        
			        //enviarMensaje ( &(queue [prod]), OutMsg ); 
			        //enviarMensaje ( &(queue [val]), OutMsg ); 
			        
			        
			    case 2:
			    	break;
			    	
				default:
        			break;
					    
			}
		    
		    
		}
    	
	}
	
	if(tipo==1){		/*es admin*/
		
		
		for( ; ; ){
			printf ( "1. Añadir producto\n" );
			printf ( "2. Suplir producto\n" );	
			printf ( "3. Remover producto\n" );	
			printf ( "4. Obtener dinero\n" );	
    		printf ( "5. Salir\n" );
		    fflush ( stdout );
		    fflush ( stdin );
		    fgets ( line, sizeof (line), stdin );
		    sscanf ( line, "%d", &decision2 );		/*choice*/
		    
		    
		    switch (decision2){
		    	
		    	case 1:
		    		printf ( "Seleccione un numero distinto de 1..%d :\n", NUM_MACH );
			        fflush ( stdout );
			        fflush ( stdin );
			        fgets ( line, sizeof (line), stdin );
			        sscanf ( line, "%d", &prodAñadir );
			        
			        /*faltan señales y mensaje*/
			        OutMsg.maquina = maq;
			        OutMsg.noProducto =prodAñadir;
			        OutMsg.senal = addProduct;
			        OutMsg.valor = 0;
			        
			        //enviarMensaje ( &(queue [prodAñadir]), OutMsg ); 
			        
			        
			        
			    case 2:
			    	printf ( "Seleccione un producto a suplir entre 1..%d :\n", NUM_MACH );
			        fflush ( stdout );
			        fflush ( stdin );
			        fgets ( line, sizeof (line), stdin );
			        sscanf ( line, "%d", &prodSuplir );
		    		
		    		/*faltan señales y mensaje*/
		    		
		    		OutMsg.maquina = maq;
			        OutMsg.noProducto = prodSuplir;
			        OutMsg.senal = supplyProduct;
			        OutMsg.valor = 0;
		    		
		    		//enviarMensaje ( &(queue [prodSuplir]), OutMsg ); 
		    	
		    	case 3:
		    		printf ( "Seleccione el producto que desea remover entre 1..%d :\n", NUM_MACH );
			        fflush ( stdout );
			        fflush ( stdin );
			        fgets ( line, sizeof (line), stdin );
			        sscanf ( line, "%d", &prodRemover );
			        
			        /*faltan señales y mensaje*/
			        OutMsg.maquina = maq;
			        OutMsg.noProducto = prodRemover;
			        OutMsg.senal = RemoveProduct;
			        OutMsg.valor = 0;
			        
			        //enviarMensaje ( &(queue [prodRemover]), OutMsg ); 
			        
			    case 4: 
			    	printf ( "Devolviendo dinero acumulado... :\n" );
			    	
			    	/*faltan señales y mensaje*/
			    	
			    	OutMsg.maquina = maq;
			        OutMsg.noProducto = 0;
			        OutMsg.senal = withdrawal;
			        OutMsg.valor = 0;
			        
			        //enviarMensaje ( &(queue [withdrawal]), OutMsg ); 
			    	
			    case 5:
			    	break;
			    	
			    default:
        			break;
			}
		}
	}
	return ( NULL );
  }
  
 /*crm*/ 
static void *CRM (  ){
	
	ESTADOS_CRM       state,
                      state_next;
	Smensaje          InMsg,
	                  OutMsg;
	pthread_t         maquinas[NUM_MACH+1];  /*plocals en el sdl lo volvi aca maquinas*/
	
	int               index;
	
	int               SenderQ;
	
	for ( index = 1; index <= NUM_MACH; index++ ){
		
	    pthread_create ( &maquinas[index], NULL, Maquina, (void *) &index );
	    sleep ( 1 );
	}
	
	printf ( "\t\t\t\tsReady\n" );                          /* "send" message to environment */
	fflush ( stdout );
	
	state_next = IdleC;
	
	
	//revisar esta monda
	for ( ; ; ){
		
	    state = state_next;
	    InMsg = recibirMensaje ( &(queue [CENTRAL_Q]) );
	
	    printf ( "\tpCentral received signal %d, value %d in state %d\n", InMsg.signal, InMsg.value, state );
	    fflush ( stdout );
	
	    switch ( state )
	    {
	      case IdleC:
	        switch ( InMsg.signal )
	        {
	          case sGetId:       /*sNumMach*/
	            index = InMsg.value;
	            if ( ( index >= 1 ) && ( index <= NUM_MACH ) )      	/*True*/
	            {
	              OutMsg.signal = (int) sId;
	              OutMsg.value = index;                       // pLocals[index] 
	              OutMsg.sender = 0;
	            }
	            else													/*False*/
	            {
	              OutMsg.signal = (int) sError;
	              OutMsg.value = 0;
	              OutMsg.sender = 0;
	            }
	            SenderQ = InMsg.sender;
	            enviarMensaje ( &(queue [SenderQ]), OutMsg );   // send message to sender process 
	            state_next = IdleC;
	            break;
	            
	          /*case createProduct:
	          	//saveData
	          	//envia a response donde decide que responde
	          	
			  case updateProduct:
			    //saveData
	          	//envia a response donde decide que responde
			  case requestInformation:                   //casos del SDL
			  	//saveData
	          	//envia a response donde decide que responde
			  case activateMachine:  
			  	//saveData
	          	//envia a response donde decide que responde
				  */
	            
	            
	          default:
	            break;
	        }
	        break;
	      default:
	        break;
	    }
	    printf ( "\tpCentral next state is %d\n", state_next );
	    fflush ( stdout );
	}

  for ( index = 1; index <= NUM_MACH; index++ )
    pthread_join ( maquinas[index], NULL );

  return ( NULL );
		
}
  
  
  
  
  /*

  for ( ; ; )
  {
    printf ( "1. Select Machine\n" );
    //printf ( "2. Enter Coin\n" );
    fflush ( stdout );
    fflush ( stdin );
    fgets ( line, sizeof (line), stdin );
    sscanf ( line, "%d", &maq );		//choice
    
    
    
    
    
    switch ( choice )
    {
      case 1:
        printf ( "Choose A subscriber (1..%d)\n", NUM_PHONE );
        fflush ( stdout );
        fflush ( stdin );
        fgets ( line, sizeof (line), stdin );
        sscanf ( line, "%d", &Asub );
        printf ( "Choose B subscriber (1..%d)\n", NUM_PHONE );
        fflush ( stdout );
        fflush ( stdin );
        fgets ( line, sizeof (line), stdin );
        sscanf ( line, "%d", &Bsub );
        OutMsg.signal = (int) sCall;
        OutMsg.value = Bsub;
        OutMsg.sender = 0;
        sendMessage ( &(queue [Asub]), OutMsg );          // send message to A subscriber 
        break;
      case 2:
        printf ( "Choose A subscriber (1..%d)\n", NUM_PHONE );
        fflush ( stdout );
        fflush ( stdin );
        fgets ( line, sizeof (line), stdin );
        sscanf ( line, "%d", &Asub );
        OutMsg.signal = (int) sHangUp;
        OutMsg.value = 0;
        OutMsg.sender = 0;
        sendMessage ( &(queue [Asub]), OutMsg );          // send message to A subscriber 
        break;
      default:
        break;
    }
  }

  return ( NULL );
}

*/









