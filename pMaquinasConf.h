//DEFINICIONES BASICAS DEL SISTEMA

//Constantes/variables del sistema-------------------------------------------------------
#define BUFSIZE 8
#define NUM_COLAS 6
#define NUM_MACH 4
#define Maq1 0
#define Maq2 1
#define Maq3 2
#define Maq4 3
#define CRM 4
#define NUBE 5

//Estructura de Mensaje--------------------------------------------------------
typedef struct{
    int valor;
    int maquina;
    int senal;
    int noProducto;
} Smensaje;


//Senales del Sistema----------------------------------------------------------
typedef enum{
    updateProduct,
    returnResponseNube,
    createProduct,
    deleteProduct,
    requestInformation,
    RegistrarCompra
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
