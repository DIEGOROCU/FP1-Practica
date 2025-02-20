/**************************************************
*                                                 *
*         SIMULACI�N DE CARRERA DE COCHES         *
*      Autores: Pau Frangi y Diego Rodr�guez      *
*                                                 *
***************************************************/



// Incluimos las librer�as necesarias
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Colores para la consola
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"


// Constantes
const int LONG_CARRETERA = 20;
const int MAX_PASOS = 3;
const int TIEMPO_PARADO = 3;
const int POS_COCHE = 0;
const int NUM_CARRILES = 3;
const int MAX_CARRERAS = 3;

// Caracteres para dibujar la carretera
const char CHAR_LINEA_HORIZONTAL = char(205);
const char CHAR_ESQUINA_SUPERIOR_IZQUIERDA = char(201);
const char CHAR_ESQUINA_SUPERIOR_DERECHA = char(187);
const char CHAR_ESQUINA_INFERIOR_IZQUIERDA = char(200);
const char CHAR_ESQUINA_INFERIOR_DERECHA = char(188);
const char CHAR_LINEA_VERTICAL = char(186);
const char CHAR_COCHE = char(254);
const char CHAR_CLAVO = char(193);
const char CHAR_SORPRESA = char(63);
const char CHAR_NORMAL = char(32);

// Enumerado para tipos de posici�n
enum tTipoPosicion {
    NORMAL,
    CLAVO,
    SORPRESA
};

// Estructura para el coche
struct tCoche {
    int pos;
    int tiempoParado;
};

// Estructura para el carril
struct tCarril {

    tCoche coche;
    tTipoPosicion posiciones[LONG_CARRETERA];
};

// Estructura para la clasificaci�n
struct tClasificacion {

    std::string idCarretera;
    int clasificacion[NUM_CARRILES];
    int cont;
};

// Estructura para la lista de clasificaciones
struct tListaClasificacion {

    tClasificacion lista[MAX_CARRERAS];
    int cont;
};

// Alias de tipo para la carretera
using tCarretera = tCarril[NUM_CARRILES];

// Funciones para generar n�meros aleatorios
int aleatorio(int max) {
    return std::rand() % max;
}

// Funci�n para mostrar el t�tulo del programa
void titulo() {


    std::cout << "***************************************************" << "\n";
    std::cout << "*                                                 *" << "\n";
    std::cout << "*         SIMULACION DE CARRERA DE COCHES         *" << "\n";
    std::cout << "*      Autores: Pau Frangi y Diego Rodriguez      *" << "\n";
    std::cout << "*                                                 *" << "\n";
    std::cout << "***************************************************" << "\n";
    std::cout << "\n";

}

// Funci�n para preguntar al usuario si quiere hacer otra simulaci�n
bool seguirSimulacion() {

    std::cout << "QUIERES HACER OTRA SIMULACION? (S/N)" << "\n";
    char aux;
    std::cin >> aux;

    // Si el usuario introduce 'S' o 's', hacer otra simulaci�n
    return aux == 'S' || aux == 's';

}

// Funciones para inicializar el coche y la carretera
void iniciaCoche(tCoche& coche) {

    coche.pos = 0;
    coche.tiempoParado = 0;
}

// Funci�n para inicializar la carretera
void iniciaCarril(tCarril& carril) {

    // Inicializar la carretera con posiciones normales
    for (int j = 0; j < LONG_CARRETERA; j++) {
        carril.posiciones[j] = NORMAL;
    }
}

// Funci�n para leer la configuraci�n de un carril desde un archivo
void leeCarril(std::ifstream& entrada, tCarril& carril) {

    std::string auxiliar;

    // Leer la configuraci�n desde el archivo hasta que se lee "XX"
    while (entrada >> auxiliar && auxiliar != "XX") {

       
            int numClavos;
            entrada >> numClavos;
            for (int i = 0; i < numClavos; i++) {
                int posClavos;
                entrada >> posClavos;
                carril.posiciones[posClavos] = stringtoEnum(auxiliar);
            }
    
    }

}

// Funci�n para cargar la configuraci�n de la carretera desde un archivo
bool cargaCarretera(tCarretera& carriles, std::string nombreArchivo) {

    bool ok = true;

    // Crear un objeto de tipo ifstream para leer el archivo
    std::ifstream entrada;
    std::string auxiliar;

    //Abrir el archivo
    entrada.open(nombreArchivo + ".txt");

    // Si se puede abrir el archivo, leer la configuraci�n de la carretera
    if (entrada.is_open()) {

        // Leer la configuraci�n de cada carril e inicializarlo
        for (int i = 0; i < NUM_CARRILES; i++) {

            iniciaCarril(carriles[i]);
            iniciaCoche(carriles[i].coche);
            leeCarril(entrada, carriles[i]);
        }


    }
    else {

        // Si no se puede abrir el archivo, mostrar un mensaje de error
        ok = false;
        std::cout << "NO SE HA PODIDO ABRIR EL ARCHIVO. INTENTA DE NUEVO." << "\n";

        cargaCarretera(carriles, nombreArchivo);
    }

    // Cerrar el archivo
    entrada.close();

    return ok;
}

// Funci�n para dibujar la l�nea horizontal superior
void dibujaLineaHorizontalSuperior() {

    std::cout << CHAR_ESQUINA_SUPERIOR_IZQUIERDA << std::setw(LONG_CARRETERA) << std::setfill(CHAR_LINEA_HORIZONTAL) << CHAR_LINEA_HORIZONTAL << CHAR_ESQUINA_SUPERIOR_DERECHA << "\n";
}

// Funci�n para dibujar la l�nea discontinua
void dibujaLineaDiscontinua() {

    std::cout << CHAR_LINEA_VERTICAL;

    for (int i = 0; i < LONG_CARRETERA / 2; i++) {

        std::cout << CHAR_LINEA_HORIZONTAL << " ";
    }

    // Si la longitud de la carretera es impar, dibujar un caracter m�s
    if (LONG_CARRETERA % 2 != 0) {

        std::cout << CHAR_LINEA_HORIZONTAL;
    }

    std::cout << CHAR_LINEA_VERTICAL << "\n";
}

// Funci�n para dibujar la l�nea horizontal inferior
void dibujaLineaHorizontalInferior() {

    std::cout << CHAR_ESQUINA_INFERIOR_IZQUIERDA << std::setw(LONG_CARRETERA) << std::setfill(CHAR_LINEA_HORIZONTAL) << CHAR_LINEA_HORIZONTAL << CHAR_ESQUINA_INFERIOR_DERECHA << "\n";
}

void dibujaCarril(const tCarril& carril) {

    std::cout << CHAR_LINEA_VERTICAL;

    for (int i = 0; i < LONG_CARRETERA; i++) {

        // Dibujar el tipo de posici�n en cada celda
        if (carril.posiciones[i] == NORMAL) {
            std::cout << CHAR_NORMAL;
        }
        else if (carril.posiciones[i] == CLAVO) {

            std::cout << RED;
            std::cout << CHAR_CLAVO;
            std::cout << RESET;
        }
        else if (carril.posiciones[i] == SORPRESA) {

            std::cout << BLUE;
            std::cout << CHAR_SORPRESA;
            std::cout << RESET;
        }
    }

    std::cout << CHAR_LINEA_VERTICAL << "\n";

    std::cout << CHAR_LINEA_VERTICAL;

    // Dibujar el coche en la posici�n actual
    std::cout << std::setw(carril.coche.pos + 1) << std::setfill(CHAR_NORMAL) << CHAR_COCHE << std::setw(LONG_CARRETERA - carril.coche.pos) << std::setfill(CHAR_NORMAL);

    std::cout << CHAR_LINEA_VERTICAL << "\n";

}

// Funci�n para dibujar la carretera
void dibujaCarretera(const tCarretera carriles) {

    // Dibujar la carretera
    dibujaLineaHorizontalSuperior();

    // Dibujar los carriles y las l�neas discontinuas
    for (int i = 0; i < NUM_CARRILES - 1; i++) {

        dibujaCarril(carriles[i]);
        dibujaLineaDiscontinua();
    }

    // Dibujar el �ltimo carril y la l�nea horizontal inferior
    dibujaCarril(carriles[NUM_CARRILES - 1]);
    dibujaLineaHorizontalInferior();


}


// Funci�n para preguntar al usuario si quiere entrar en modo debug
bool modo() {

    // Variable para controlar el modo debug
    bool modo = false;
    char aux;
    std::cout << "QUIERES ENTRAR EN EL MODO DEBUG? (S/N)" << "\n";
    std::cin >> aux;

    // Si el usuario introduce 'S' o 's', entrar en modo debug
    if (aux == 'S' || aux == 's') {
        modo = true;
    }

    std::cin.ignore(); // Limpiar cualquier caracter de nueva l�nea restante

    return modo;
}


// Funciones para verificar la presencia de sorpresas o clavos en una posici�n
bool esSorpresa(const tTipoPosicion pos[], int p) {

    return pos[p] == SORPRESA;
}

// Funci�n para verificar si hay un clavo en una posici�n
bool esClavo(const tTipoPosicion pos[], int p) {

    return pos[p] == CLAVO;
}

// Funci�n para verificar si una posici�n est� dentro de la carretera
bool enCarretera(int n) {

    return n >= 0 && n < LONG_CARRETERA;
}

// Funci�n para buscar la posici�n de una sorpresa en una direcci�n dada
int buscaPosicionSorpresa(const tCarril& carril, int incr) {

    int cont;

    if (incr == 1) {

        // Si el incremento es 1, buscar hacia la derecha hasta encontrar una sorpresa
        cont = carril.coche.pos + 1;
        while (cont < LONG_CARRETERA && carril.posiciones[cont] != SORPRESA) {
            cont++;
        }

        if (cont == LONG_CARRETERA) {

            cont = 0;
        }

    }
    else if (incr == -1) {

        // Si el incremento es -1, buscar hacia la izquierda hasta encontrar una sorpresa
        cont = carril.coche.pos - 1;
        while (cont >= 0 && carril.posiciones[cont] != SORPRESA) {
            cont--;
        }
    }

    return cont;
}

// Funci�n para avanzar el coche (con o sin modo debug)
int avanza(int& posCoche, int i, bool DEBUG) {

    // Si el modo debug est� activado, pedir el n�mero de pasos que quiere avanzar el usuario
    if (DEBUG) {

        std::cout << "INTRODUCE EL NUMERO DE PASOS QUE QUIERES AVANZAR (COCHE " << i << ")" << "\n";
        int avanzar;
        std::cin >> avanzar;
        posCoche += avanzar;

        // Si el modo debug no est� activado, avanzar un n�mero aleatorio de pasos
    }
    else {

        std::cout << "PULSA ENTER PARA AVANZAR (COCHE " << i << ")" << "\n";
        std::string dummy;
        std::getline(std::cin, dummy); // Leer la l�nea completa (incluido el salto de l�nea)
        int avanza = aleatorio(MAX_PASOS + 1);
        posCoche += avanza;

        std::cout << "EL COCHE " << i << " AVANZA " << avanza << " PASOS" << "\n";
    }

    // Si el coche se sale de la carretera, colocarlo en la posici�n 0
    if (posCoche < 0) {

        posCoche = 0;
    }

    return posCoche;
}

// Funci�n para verificar si el coche ha llegado al final de la carretera
bool haLlegado(int posCoche) {

    return posCoche >= LONG_CARRETERA;
}

// Funci�n para calcular la nueva posici�n del coche y realizar acciones correspondientes
bool calculaPosicion(tCarril& carril, int i) {

    // Si el coche est� en una posici�n con un clavo, pinchar la rueda
    if (esClavo(carril.posiciones, carril.coche.pos)) {

        // El coche se queda inmovilizado durante un n�mero de pasos
        carril.coche.tiempoParado = TIEMPO_PARADO;

        std::cout << "EL COCHE " << i << " SE HA PINCHADO. ESTARA INMOVILIZADO " << TIEMPO_PARADO << " PASOS" << "\n";

        // Si el coche est� en una posici�n con una sorpresa, avanzar o retroceder
    }
    else if (esSorpresa(carril.posiciones, carril.coche.pos)) {

        std::cout << "EL COCHE " << i << " ESTA EN UNA POSICION SORPRESA" << "\n";
        int incr = aleatorio(2);

        // Si el n�mero aleatorio es 0, el coche avanza. Si es 1, retrocede
        if (incr == 0) {
            incr = 1;
        }
        else {
            incr = -1;
        }

        //Actalizar la posici�n del coche
        int posAnterior = carril.coche.pos;
        carril.coche.pos = buscaPosicionSorpresa(carril, incr);

        // Mostrar el incremento en la posici�n del coche
        if (incr == 1) {

            if (carril.coche.pos == 0) {

                std::cout << "EL COCHE " << i << " RETROCEDE " << posAnterior - carril.coche.pos << " PASOS" << "\n";
            }
            else {

                std::cout << "EL COCHE " << i << " AVANZA " << carril.coche.pos - posAnterior << " PASOS" << "\n";
            }

        }
        else {
            std::cout << "EL COCHE " << i << " RETROCEDE " << posAnterior - carril.coche.pos << " PASOS" << "\n";
        }

        return true;

    }

    return false;

}

// Funci�n para avanzar el coche en el carril y dibujar la carretera
bool avanzaCarril(tCarretera carretera, int i, bool DEBUG) {

    // Si el coche no est� pinchado, avanzar el coche
    if (carretera[i].coche.tiempoParado == 0) {

        avanza(carretera[i].coche.pos, i, DEBUG);
    }
    else {

        // Si el coche est� pinchado, reducir el tiempo que le queda para moverse
        carretera[i].coche.tiempoParado--;

        // Si el coche ya ha reparado la rueda, avanzar el coche
        if (carretera[i].coche.tiempoParado == 0) {

            std::cout << "EL COCHE " << i << " HA REANUDADO LA MARCHA" << "\n";
            avanza(carretera[i].coche.pos, i, DEBUG);

            // Si el coche no ha reparado la rueda, mostrar un mensaje
        }
        else {

            std::cout << "EL COCHE " << i << " ESTA PINCHADO. LE QUEDAN " << carretera[i].coche.tiempoParado << " TURNOS PARA MOVERSE" << "\n";
        }
    }

    // Si el coche ha llegado al final de la carretera, mostrar un mensaje, colocarlo en la posici�n final y devolver true
    if (haLlegado(carretera[i].coche.pos)) {

        carretera[i].coche.pos = LONG_CARRETERA;
        dibujaCarretera(carretera);

        return true;

        // Si el coche no ha llegado al final de la carretera, calcular la nueva posici�n del coche y dibujar la carretera
    }
    else if (carretera[i].coche.tiempoParado == 0) {

        dibujaCarretera(carretera);
        bool enSorpresa = calculaPosicion(carretera[i], i);

        // Si el coche est� en una posici�n sorpresa, dibujar la carretera
        if (enSorpresa) {

            dibujaCarretera(carretera);
        }
    }

    return false;

}

// Funci�n para avanzar todos los carriles
void avanzaCarriles(tCarretera carretera, tClasificacion& clasificacion, bool DEBUG) {

    for (int i = 0; i < NUM_CARRILES; i++) {

        // Si el coche no ha llegado al final de la carretera, avanzar el carril (se asegura de que no se clasifique varias veces si ya ha llegado)
        if (!haLlegado(carretera[i].coche.pos)) {

            avanzaCarril(carretera, i, DEBUG);

            // Si el coche ha llegado al final de la carretera, a�adirlo a la clasificaci�n
            if (haLlegado(carretera[i].coche.pos)) {

                clasificacion.clasificacion[clasificacion.cont] = i;
                clasificacion.cont++;

            }
        }

    }

}

void mostrarClasificacion(const tClasificacion& clasificacion) {

    std::cout << "\n" << "-------------------------------------" << "\n";
    std::cout << "CLASIFICACION DE LA CARRERA" << "\n" << "\n";

    for (int i = 0; i < NUM_CARRILES; i++) {

        std::cout << "  PUESTO " << i + 1 << ": COCHE EN EL CARRIL " << clasificacion.clasificacion[i] << "\n";
    }

    std::cout << "-------------------------------------" << "\n" << "\n";
}

// Funci�n para simular la carrera hasta que el coche llegue al final de la carretera
tClasificacion simulaCarrera(tCarretera carretera, tClasificacion& clasificacion) {

    bool DEBUG = modo();

    // Pedir el identificador de la carretera
    std::cout << "INTRODUCE EL IDENTIFICADOR DE LA CARRERA" << "\n";
    std::cin >> clasificacion.idCarretera;
    std::cin.ignore(); // Limpiar cualquier caracter de nueva l�nea restante

    // Inicializar la clasificaci�n y el contador de coches que han llegado al final
    clasificacion.cont = 0;
    int aux = 0;

    // Mientras no hayan llegado todos los coches al final de la carretera, avanzar los carriles
    while (aux < NUM_CARRILES) {

        avanzaCarriles(carretera, clasificacion, DEBUG);
        aux = 0;

        // Contar los coches que han llegado al final de la carretera
        for (int i = 0; i < NUM_CARRILES; i++) {

            if (haLlegado(carretera[i].coche.pos)) {

                aux++;
            }

        }

    }

    // Mostrar la clasificaci�n
    mostrarClasificacion(clasificacion);

    // Devolver la clasificaci�n
    return clasificacion;


}

// Funci�n que inicializa la lista de clasificaciones
void iniciaListaClasificacion(tListaClasificacion& listaC) {

    listaC.cont = 0;

}


// Funci�n que elimina la clasificaci�n de la posici�n pos de la lista listaC, manteniendo el orden de las clasificaciones
void eliminaClasificacion(tListaClasificacion& listaC, int pos) {

    for (int i = pos; i < listaC.cont - 1; i++) {

        listaC.lista[i] = listaC.lista[i + 1];
    }

    listaC.cont--;

}

// Funcion que que inserta la clasificacion al final de la lista de clasificaciones 
void insertaClasificacion(tListaClasificacion& listaC, const tClasificacion& clasificacion) {

    // Si la lista est� llena, eliminar la clasificaci�n de la posici�n 0
    if (listaC.cont == MAX_CARRERAS) {

        eliminaClasificacion(listaC, 0);
    }

    // Insertar la clasificaci�n al final de la lista
    listaC.lista[listaC.cont] = clasificacion;
    listaC.cont++;

}

// Funci�n que guarda la lista de clasificaciones en el archivo "clasificacion.txt"
bool guardaListaClasificaci�n(const tListaClasificacion& listaC) {

    // Crear un objeto de tipo ofstream para escribir el archivo
    std::ofstream salida;

    // Abrir el archivo
    salida.open("clasificacion.txt");

    // Si se puede abrir el archivo, escribir la lista de clasificaciones
    if (salida.is_open()) {

        for (int i = 0; i < listaC.cont; i++) {

            salida << "CARRERA: " << listaC.lista[i].idCarretera << "\n";

            for (int j = 0; j < NUM_CARRILES; j++) {

                salida << "   PUESTO " << j + 1 << ": COCHE EN EL CARRIL " << listaC.lista[i].clasificacion[j] << "\n";
            }

            salida << "\n";
        }

        std::cout << "SE HA GUARDADO LA CLASIFICACION EN EL ARCHIVO 'clasificacion.txt'" << "\n";


    }
    else {

        // Si no se puede abrir el archivo, mostrar un mensaje de error
        std::cout << "NO SE HA PODIDO ABRIR EL ARCHIVO" << "\n";
        return false;
    }

    // Cerrar el archivo
    salida.close();

    return true;

}


// Funci�n principal
int main() {

    tCarretera carretera;
    tListaClasificacion listaClasificacion;

    titulo();
    iniciaListaClasificacion(listaClasificacion);

    // Abrir el archivo
    std::string nombreArchivo = "";
    std::cout << "INTRODUCE EL NOMBRE DEL ARCHIVO (sin .txt): " << "\n";
    std::cin >> nombreArchivo;

    do {// Mientras el usuario quiera hacer otra simulaci�n, simular la carrera

        tClasificacion clasificacion;
        cargaCarretera(carretera, nombreArchivo);  // funci�n booleana que devuelve true si se ha podido cargar la carretera (no se utiliza)
        dibujaCarretera(carretera);
        insertaClasificacion(listaClasificacion, simulaCarrera(carretera, clasificacion));

    } while (seguirSimulacion());

    // Guardar la lista de clasificaciones en el archivo "clasificacion.txt"
    guardaListaClasificaci�n(listaClasificacion); // funci�n booleana que devuelve true si se ha podido guardar la lista de clasificaciones (no se utiliza)

    return 0;
}
