

#include    <stdio.h>
#include    <unistd.h>
#include    <string.h>
#include    <errno.h>
#include 	"phtrdsMsg.h"

#define 	NUM_MACH 4


static void *Maquina();		/*faltan args que recibe*/
static void *CRM();			/*faltan args que recibe*/
static void *Nube();		/*faltan args que recibe*/
static void *Environment(); /*faltan args que recibe*/


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
static void *Environment (  )			/*faltan args que recibe*/
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
static void *CRM (  ){			/*faltan args que recibe*/
	
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
	
	state_next = IdleC;			/*idleL del CRM */
	
	
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


static void *Maquina(){		/*faltan args que recibe*/
	
	ESTADOS_MAQUINA    state,
                  state_next;
    Smensaje          InMsg,
                  OutMsg;

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

  	printf ( "\t\t\t\tsubscriber %d is free...\n", whoami );
  	fflush ( stdout );

  	state_next = IdleL;       /*idleL de la maquina que en el SDL es connected*/
  	
  	for ( ; ; ){
  		
    	state = state_next;
    	InMsg = receiveMessage ( &(queue [queueNo]) );
    	
    	switch(state){
    		case IdleL:
    			switch ( InMsg.signal ){
    				case configMach:
    					calledNumber = InMsg.value;
			            OutMsg.signal = (int) sGetId;			/*verificar esta monda*/ /*sGetId cambiar a sNumMach*/
			            OutMsg.value = calledNumber;
			            OutMsg.sender = whoami;
			            sendMessage ( &(queue [CENTRAL_Q]), OutMsg ); /* send message to pCentral process */
			            state_next = GettingId;
			            break;
			        case addProduct:
			        	calledNumber = InMsg.value;
			            OutMsg.signal = (int) sGetId;			/*verificar esta monda*/ /*sGetId cambiar a sNumMach*/
			            OutMsg.value = calledNumber;
			            OutMsg.sender = whoami;	
			            //enviarMensaje ( &(queue [createProduct]), OutMsg ); 
			            //sendMessage ( &(queue [CENTRAL_Q]), OutMsg ); /* send message to pCentral process *///borrar
			            state_next = response2;
			            break;
			            
			        case supplyProduct:
			        	calledNumber = InMsg.value;
			            OutMsg.signal = (int) sGetId;			/*verificar esta monda*/ /*sGetId cambiar a sNumMach*/
			            OutMsg.value = calledNumber;
			            OutMsg.sender = whoami;	
			            //enviarMensaje ( &(queue [updateProduct]), OutMsg ); 
			            //sendMessage ( &(queue [CENTRAL_Q]), OutMsg ); /* send message to pCentral process *///borrar
			            state_next = response2;
			            break;
			        
			        case removeProduct:
			        	calledNumber = InMsg.value;
			            OutMsg.signal = (int) sGetId;			/*verificar esta monda*/ /*sGetId cambiar a sNumMach*/
			            OutMsg.value = calledNumber;
			            OutMsg.sender = whoami;	
			            //enviarMensaje ( &(queue [updateProduct]), OutMsg ); 
			            //sendMessage ( &(queue [CENTRAL_Q]), OutMsg ); /* send message to pCentral process *///borrar
			            state_next = response2;
			            break;
			        
			        case withdrawal:
			        	calledNumber = InMsg.value;
			            OutMsg.signal = (int) sGetId;			/*verificar esta monda*/ /*sGetId cambiar a sNumMach*/
			            OutMsg.value = calledNumber;
			            OutMsg.sender = whoami;	
			            //enviarMensaje ( &(queue [updateProduct]), OutMsg );              		/*ACA SEGUN EL SDL NO MANDA NINGUNA SEÑAL*/
			            //sendMessage ( &(queue [CENTRAL_Q]), OutMsg ); /* send message to pCentral process *///borrar
			            state_next = response3;
			            break;
			        
			        case enterCoin:
			        	payed = payed + 1;
			        	state_next = IdleL;
			        	break;
			        	
			        case selectSlot:
			        	selected=selected+1;
			        	
			        	if(payed==1 && selected==1){            /*True*/
			        		calledNumber = InMsg.value;
			            	OutMsg.signal = (int) sGetId;			/*verificar esta monda*/ /*sGetId cambiar a sNumMach*/
			            	OutMsg.value = calledNumber;
			            	//enviarMensaje ( &(queue [updateProduct]), OutMsg ); 
			            	//sendMessage ( &(queue [CENTRAL_Q]), OutMsg ); /* send message to pCentral process *///borrar
			            	payed=0;
			            	selected=0;
			            	state_next = response;
			            	break;
						}
						else{                                 /*False*/
							state_next = IdleL;
							break;
						}
					
					case returnResponseCRM:
						state_next = IdleL;
						break;
						
					case sId:
						remotePId= InMsg.value;
						state_next = IdleL;
						break;
			        	
			        default:
            			break;
			        	
				}
				break;
    		case GettingId:
    			switch ( InMsg.signal ){
    				
		          	case sId:
			            remotePId = InMsg.value;
			            //OutMsg.signal = (int) sCnxReq;  /*cambiar sCnxReq por activateMachine*/
			            OutMsg.signal = (int) sCnxReq;  /*cambiar sCnxReq por returnResponseCRM*/
			            OutMsg.value = 0;
			            OutMsg.sender = whoami;
			            sendMessage ( &(queue [remotePId]), OutMsg ); /* send message to pLocal process */
			            state_next = Connecting;
			            break;
			            
			        case sError:
			            state_next = IdleL;
			            break;
			            
			        default:
			            break;
		        }
		        break;
		        
		    case Connecting:

		        switch ( InMsg.signal ){
		        	
		          	case activateMachine:
			            printf ( "\t\t\t\tsCallConf: %d -> %d\n", whoami, InMsg.sender );			/*cambiar mensaje print*/
			                                                          /* "send" message to environment */
			            fflush ( stdout );
			            state_next = IdleL;   /*connected = IdleL*/
			            break;
			        case returnResponseCRM:
			            printf ( "\t\t\t\t%d sBusy\n", InMsg.sender );							/*cambiar mensaje print*/
			                                                          /* "send" message to environment */
			            fflush ( stdout );
			            state_next = IdleL;
			            break;
			        default:
			            break;
		        }
		        break;
		        
		    case response2:
		    	
		    	switch ( InMsg.signal ){
		        	
			        case returnResponseCRM:
			            printf ( "\t\t\t\t%d sBusy\n", InMsg.sender );							/*cambiar mensaje print*/
			                                                          /* "send" message to environment */
			            fflush ( stdout );
			            state_next = IdleL;
			            break;
			        
					case sID:
			            printf ( "\t\t\t\tsCallConf: %d -> %d\n", whoami, InMsg.sender );			/*cambiar mensaje print*/
			                                                          /* "send" message to environment */
			            fflush ( stdout );
			            state_next = IdleL;   /*connected = IdleL*/
			            break;    
			        
			        default:
			            break;
		        }
		        break;
		        
		    case response3:
		    	
		    	switch ( InMsg.signal ){
		        	
			        case returnResponseCRM:
			        	//OutMsg.signal = (int) returnMoney;  /*cambie sCnxReq por returnMoney*/
			            state_next = IdleL;
			            break;
			        
			        case sID:
			        	
			        	//OutMsg.signal = (int) requestInformation;  /*cambie sCnxReq por requestInformation*/
			        	//OutMsg.signal = (int) returnMoney;  /*cambie sCnxReq por returnMoney*/
			        	state_next = IdleL;
			            break;
			            
			        default:
			            break;
		        }
		        break;
		    case response:
		    	
		    	if (InMsg.signal==returnResponseCRM){
		    		//OutMsg.signal = (int) deliverProduct;  /*cambie sCnxReq por deliverProduct*/
			        //OutMsg.signal = (int) returnCoin;  /*cambie sCnxReq por returnCoin*/
			        state_next = IdleL;
			        break;
				}
		    	else{
		    		break;
				}
				break;
    		default:
				break;	
		}
	}	
	return ( NULL );	
}

static void *Nube(){   /*faltan args que recibe*/
	ESTADOS_NUBE state,
                 state_next;
    Smensaje     InMsg,
                 OutMsg;
    int             queueNo,
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





