

#include    <stdio.h>
#include    <unistd.h>
#include    <string.h>
#include    <errno.h>
#include 	"phtrdsMsg.h"


static void *Maquina(void *arg_ptr);		
static void *Crm(void *arg_ptr);			
static void *Nube(void *arg_ptr);			
static void *Environment(void *arg); 	


int main ( void )
{
  pthread_t   env_tid;                    
  pthread_t   CRM_tid;               
  pthread_t   Nube_tid;                    

  /* Create queues */
  iniciarColas();

  /* Create threads */
  pthread_create ( &env_tid, NULL, Environment, NULL );
  pthread_create ( &CRM_tid, NULL, Crm, NULL );
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
static void *Environment (void *arg ){
    char  line [100];
    int   val,		/*dinero*/
  	    maq,		/*num maquina*/
  		tipo,		/*cliente o admin*/
  		prod,		/*numero del producto*/
  		decision,
		prodA�adir,
		prodSuplir,
		prodRemover;
    Smensaje OutMsg;  
    for( ; ; ){
	    printf ( "Seleccione una de las maquinas (0..3):\n"); 
        fflush ( stdout );
        fflush ( stdin );
        fgets ( line, sizeof (line), stdin );
        sscanf ( line, "%d", &maq );		/*choice*/ 
        printf ( "Es usted un administrador o un cliente? (admin=1 , cliente=0)\n" );
        fflush ( stdout );
        fflush ( stdin );
        fgets ( line, sizeof (line), stdin );
        sscanf ( line, "%d", &tipo );		/*tipo de persona*/
        switch(tipo){
            case 0:
    	        printf ( "Seleccione un producto (1...5):\n" );
	            fflush ( stdout );
	            fflush ( stdin );
	            fgets ( line, sizeof (line), stdin );
	            sscanf ( line, "%d", &prod );		/*choice*/
		        printf ( "Ingrese su dinero :\n" );
		        fflush ( stdout );
	            fflush ( stdin );
		        fgets ( line, sizeof (line), stdin );
		        sscanf ( line, "%d", &val);
		        OutMsg.maquina = maq;
		        OutMsg.noProducto =prod;
		        OutMsg.senal = (int)enterCoin;
		        OutMsg.valor = val;
		        enviarMensaje ( &(queque [maq]), OutMsg ); 
		        break;

	        case 1:
			    printf ( "1. A�adir producto\n" );
			    printf ( "2. Suplir producto\n" );	
			    printf ( "3. Remover producto\n" );	
			    printf ( "4. Obtener dinero\n" );	
		        fflush ( stdout );
		        fflush ( stdin );
		        fgets ( line, sizeof (line), stdin );
		        sscanf ( line, "%d", &decision );		/*choice*/
		        switch (decision){
		    	    case 1:
		    		    printf ( "Seleccione un producto 1..5 :\n");
			            fflush ( stdout );
			            fflush ( stdin );
			            fgets ( line, sizeof (line), stdin );
			            sscanf ( line, "%d", &prodA�adir );
			            OutMsg.maquina = maq;
			            OutMsg.noProducto =prodA�adir;
			            OutMsg.senal = (int)addProduct;
			            OutMsg.valor = 0;
			            enviarMensaje ( &(queque [maq]), OutMsg );
                        break;
			        case 2:
			    	    printf ( "Seleccione un producto a suplir entre 1..5 :\n");
			            fflush ( stdout );
			            fflush ( stdin );
			            fgets ( line, sizeof (line), stdin );
			            sscanf ( line, "%d", &prodSuplir );
		    		    OutMsg.maquina = maq;
			            OutMsg.noProducto = prodSuplir;
			            OutMsg.senal = (int)supplyProduct;
			            OutMsg.valor = 0;
		    		    enviarMensaje ( &(queque [maq]), OutMsg );
                        break;
		    	    case 3:
		    		    printf ( "Seleccione el producto que desea remover entre 1..5 :\n");
			            fflush ( stdout );
			            fflush ( stdin );
			            fgets ( line, sizeof (line), stdin );
			            sscanf ( line, "%d", &prodRemover );
			            OutMsg.maquina = maq;
			            OutMsg.noProducto = prodRemover;
			            OutMsg.senal = (int)RemoveProduct;
			            OutMsg.valor = 0;
			            enviarMensaje ( &(queque [maq]), OutMsg ); 
                        break;
			        case 4: 
			    	    printf ( "Devolviendo dinero acumulado... :\n" );
			    	    OutMsg.maquina = maq;
			            OutMsg.noProducto = 0;
			            OutMsg.senal = withdrawal;
			            OutMsg.valor = 0;
			            enviarMensaje ( &(queque [maq]), OutMsg ); 
			    	    break;	
			        default:
        			    break;
			    }
		}
	}
    return ( NULL );
}
  
 /*crm*/ 
static void *Crm(void *arg_ptr){
	
	ESTADOS_CRM       state,
                      state_next;
	Smensaje          InMsg,
	                  OutMsg;
	pthread_t         maquinas[NUM_MACH]; 
	int               index;	
	int               SenderQ;
	
	for ( index = 0; index < NUM_MACH; index++ ){	
	    pthread_create ( &maquinas[index], NULL, Maquina, (void *) &index );
	    sleep ( 1 );
	}
	
	printf ( "\t\t\t\tsReady\n" ); 
	fflush ( stdout );

    state_next = IdleC;		
	for ( ; ; ){	
	    state = state_next;
	    InMsg = recibirMensaje ( &(queque [CRM]) );	
	    printf ( "\tCRM recive se�al %d, con estado %d\n", InMsg.senal, state);
	    fflush ( stdout );	
	    switch (state){
            case IdleC:
	            switch ( InMsg.senal){
	                case deleteProduct:
	                    OutMsg.maquina = InMsg.maquina;
                        OutMsg.noProducto = InMsg.noProducto;
                        OutMsg.senal = (int)saveData;
	                    enviarMensaje ( &(queque [NUBE]), OutMsg );
	                    state_next = ResponseC;
	                    break;
                    case updateProduct:
                        OutMsg.maquina = InMsg.maquina;
                        OutMsg.noProducto = InMsg.noProducto;
                        OutMsg.valor = 0;
                        OutMsg.senal = (int)saveData; 
	                    enviarMensaje ( &(queque [NUBE]), OutMsg );  
                        state_next = ResponseC;
                        break;
                    case createProduct:
                        OutMsg.maquina = InMsg.maquina;                         
                        OutMsg.noProducto = InMsg.noProducto;                   
                        OutMsg.valor = 0;
                        OutMsg.senal = (int)saveData;
                        enviarMensaje ( &(queque [NUBE]), OutMsg );
                        state_next = ResponseC;
                        break;
                    case requestInformation:
                        OutMsg.maquina = InMsg.maquina;
                        OutMsg.noProducto = 0;
                        OutMsg.valor = 0;
                        OutMsg.senal = (int)saveData;
                        enviarMensaje ( &(queque [NUBE]), OutMsg );
                        state_next = ResponseC;
                        break;
                    case RegistrarCompra:
                        OutMsg.maquina = InMsg.maquina;
                        OutMsg.noProducto = InMsg.noProducto;
                        OutMsg.valor = InMsg.valor;
                        OutMsg.senal = (int)saveData;
                        enviarMensaje ( &(queque [NUBE]), OutMsg );
                        state_next = ResponseC;
                        break;
                    default:
                        break;
                }
            case ResponseC:
                switch(InMsg.senal){
                    case returnResponseNube:
                        OutMsg.maquina = InMsg.maquina;
                        OutMsg.noProducto = 0;
                        OutMsg.valor = 0;
                        OutMsg.senal = (int)returnResponseCrm; 
	                    enviarMensaje ( &(queque [NUBE]), OutMsg );   // send message to sender process 
                        state_next = IdleC;
                        break;
                }
            default:
                break;
        }
		printf ( "\tel siguiente estado del CRM es: %d\n", state_next );
		fflush ( stdout );
    }
    for ( index = 0; index < NUM_MACH; index++ ){             
        pthread_join ( maquinas[index], NULL );
    }
    return (NULL);
}
		



static void *Maquina( void *arg_ptr ){		/*faltan args que recibe*/
	
	ESTADOS_MAQUINA    state,
                  state_next;
    Smensaje InMsg, OutMsg;

    int             queueNo,
                  *data_ptr;

    int             calledNumber,   
                  remotePId,
                  whoami,
                  senderQ,
				  payed,		/*para saber si ya pago*/
				  selected;		/*para saber si selecciono*/

    data_ptr = (int *) arg_ptr;
    queueNo = whoami = *data_ptr;

  	printf ( "\t\t\t\testa es la maquina %d...\n", whoami );
  	fflush ( stdout );
  	state_next = Connected;      
  	for ( ; ; ){
    	state = state_next;
    	InMsg = recibirMensaje ( &(queque [queueNo]) );
    	switch(state){
    		case Connected:
    			switch ( InMsg.senal ){
    				case RemoveProduct:		
			            OutMsg.valor = 0;
			            OutMsg.maquina = whoami;
						OutMsg.noProducto = InMsg.noProducto;
						OutMsg.senal = (int)deleteProduct;	
			            sendMessage ( &(queque [CRM]), OutMsg );
			            state_next = Response;
			            break;
			        case addProduct:
			        	OutMsg.valor = 0;
			            OutMsg.maquina = whoami;
						OutMsg.noProducto = InMsg.noProducto;
						OutMsg.senal = (int)createProduct;	
			            enviarMensaje ( &(queque [CRM]), OutMsg ); 
			            state_next = Response;
			            break;
			            
			        case supplyProduct:
			        	OutMsg.valor = 0;
			            OutMsg.maquina = whoami;
						OutMsg.noProducto = InMsg.noProducto;
						OutMsg.senal = (int)updateProduct;
			            enviarMensaje ( &(queque [CRM]), OutMsg ); 
			            state_next = Response;
			            break;
			            
			        case withdrawal:
			            OutMsg.valor = 0;
			            OutMsg.maquina = whoami;
						OutMsg.noProducto = 0;
						OutMsg.senal = (int)requestInformation;
			            enviarMensaje ( &(queque [CRM]), OutMsg );              		
			            state_next = Response2;
			            break;
			        
			        case enterCoin:
			        	OutMsg.valor = InMsg.valor;
			            OutMsg.maquina = whoami;
						OutMsg.noProducto = InMsg.noProducto;
						OutMsg.senal = (int)RegistrarCompra;
			        	enviarMensaje ( &(queque [CRM]), OutMsg );
						state_next = Response;  
						break;
			        default:
            			break;  	
				}
    		    
		    case Response:
		    	switch ( InMsg.senal ){
			        case returnResponseCrm:
			            printf ( "\t\t\t\t%d confirmacion transaccion\n", InMsg.maquina );							
			            fflush ( stdout );
			            state_next = Connected;
			            break; 
			        default:
			            break;
		        }   
		    case Response2:	
		    	switch ( InMsg.senal ){
			        case returnResponseCrm:
			        	printf ( "\t\t\t\t%d confirmacion transaccion\n", InMsg.maquina );							
			            fflush ( stdout );
						printf ( "\t\t\t\tdevolviendo monedas \n");							
			            fflush ( stdout );
			            state_next = Connected;
			            break;        
			        default:
			            break;
		        }
		        break;
		}
	}	
	return ( NULL );	
}

static void *Nube( void *arg_ptr ){   /*faltan args que recibe*/
	ESTADOS_NUBE state,
                 state_next;
    Smensaje     InMsg,
                 OutMsg;
    int          queueNo,
                 *data_ptr;
                 
    data_ptr = (int *) arg_ptr;
    queueNo = *data_ptr;
    
    state_next = IdleN;       /*idleL de la nube que en el SDL es Idle*/
  	
  	for ( ; ; ){
  		
    	state = state_next;
    	InMsg = receiveMessage ( &(queue [queueNo]) );
    	
    	switch(state){
    		switch ( InMsg.signal ){
	    		case IdleN:
		    		case saveData:
		    			//OutMsg.signal = (int) returnResponseNube;  /*cambie sCnxReq por returnResponseNube*/
		    			//puede ir un print aca
		    			state_next = IdleN;
		    			break;
		    		
		    		case requestData:
		    			//OutMsg.signal = (int) returnData;  /*cambie sCnxReq por returnData*/
		    			//puede ir un print aca
		    			state_next = IdleN;
		    			break;
		    		
		    		default:
		    			break;
		    	default:
		    		break;
    		}
    	}
	}
	return ( NULL );
}





