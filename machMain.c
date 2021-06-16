

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
		prodAnadir,
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
			    printf ( "1. Anadir producto\n" );
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
			            sscanf ( line, "%d", &prodAnadir );
			            OutMsg.maquina = maq;
			            OutMsg.noProducto =prodAnadir;
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
	
	for ( index = 0; index < NUM_MACH; index++ ){	
	    pthread_create ( &maquinas[index], NULL, Maquina, (void *) &index );
	    sleep ( 1 );
	}
	
	printf ( "\t\t\t\t\t\tCRM listo\n" ); 
	fflush ( stdout );

    state_next = IdleC;		
	for ( ; ; ){	
	    state = state_next;
	    InMsg = recibirMensaje ( &(queque [CRM]) );	
	    printf ( "\t\t\t\t\t\tCRM recive senal %d, de la maquina %d con estado %d\n", InMsg.senal, InMsg.maquina, state);
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
	                    enviarMensaje ( &(queque [InMsg.maquina]), OutMsg );   // send message to sender process 
                        state_next = IdleC;
                        break;
                }
            default:
                break;
        }
		printf ( "\t\t\t\t\t\tel siguiente estado del CRM es: %d\n", state_next );
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

    int  whoami;

    data_ptr = (int *) arg_ptr;
    queueNo = whoami = *data_ptr;

  	printf ( "\tla maquina %d ya se encuentra lista\n", whoami );
  	fflush ( stdout );
  	state_next = Connected;      
  	for ( ; ; ){
    	state = state_next;
    	InMsg = recibirMensaje ( &(queque [queueNo]) ); 
	    printf ( "\tMaquina recive senal %d \n", InMsg.senal);
        fflush(stdout);
    	switch(state){
    		case Connected:
    			switch ( InMsg.senal ){
    				case RemoveProduct:		
			            OutMsg.valor = 0;
			            OutMsg.maquina = whoami;
						OutMsg.noProducto = InMsg.noProducto;
						OutMsg.senal = (int)deleteProduct;	
			            enviarMensaje ( &(queque [CRM]), OutMsg );
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
			            printf ( "\tmaquina %d envia senal al ambiente: confirmacion transaccion\n", InMsg.maquina );							
			            fflush ( stdout );
			            state_next = Connected;
			            break; 
			        default:
			            break;
		        }   
		    case Response2:	
		    	switch ( InMsg.senal ){
			        case returnResponseCrm:
			        	printf ( "\tmaquina %d envia senal al ambiente: confirmacion transaccion\n", InMsg.maquina );							
			            fflush ( stdout );
						printf ( "\tdevolviendo monedas y entregando compra \n");							
			            fflush ( stdout );
			            state_next = Connected;
			            break;        
			        default:
			            break;
		        }
		        break;
		}
		printf ( "\tel siguiente estado de la maquina es: %d\n", state_next );
		fflush ( stdout );
	}	
	return ( NULL );	
}

static void *Nube( void *arg_ptr ){   
	ESTADOS_NUBE state,
                 state_next;
    Smensaje     InMsg,
                 OutMsg;
    
    state_next = IdleN; 
    printf("\t\t\t\t\t\t\t\tNube lista \n");
    fflush(stdout);
  	for ( ; ; ){
    	state = state_next;
    	InMsg = recibirMensaje ( &(queque [NUBE]) );
	    printf ( "\t\t\t\t\t\t\t\tnube recive senal %d \n", InMsg.senal);
        fflush(stdout);

    	switch(state){
			case IdleN:
    			switch ( InMsg.senal ){
		    		case saveData:
		    			OutMsg.maquina = InMsg.maquina;
                        OutMsg.noProducto = 0;
                        OutMsg.valor = 0;
						OutMsg.senal = returnResponseCrm;
						enviarMensaje ( &(queque [CRM]), OutMsg );
		    			state_next = IdleN;
		    			break;
		    		default:
		    			break;
				}
		    default:
		    	break;
    	} 
	}
	return ( NULL );
}





