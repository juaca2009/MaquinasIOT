//DEFINICIONES BASICAS DEL SISTEMA

//Constantes/variables del sistema-------------------------------------------------------
#define BUFSIZE 8
int NUM_COLAS = 6;
#define CRM 0
#define NUBE 1

//Estructura de Mensaje--------------------------------------------------------
typedef struct{
    int valor;
    int maquina;
    int senal;
} Smensaje;


//Senales del Sistema----------------------------------------------------------
typedef enum{
    sNumMach,
    updateProduct,
    returnResponseNube,
    createProduct,
    deleteProduct,
    requestInformation,
    activateMachine
} TO_CRM;


typedef enum{
    saveData
} TO_NUBE;


typedef enum{
    RemoveProduct,
    returnResponseCrm,
    enterCoin,
    selectSlot,
    addProduct,
    supplyProduct,
    withdrawal,
    configMach
} TO_MAQUINA;


//Estados del Sistema----------------------------------------------------------
typedef enum{
    IdleC,
    ResponseC
} ESTADOS_CRM;


typedef enum{
    Connected,
    Connecting,
    GettingId,
    Response,
    Response2,
    Response3
} ESTADOS_MAQUINA;

typedef enum{
    IdleN
} ESTADOS_NUBE;
