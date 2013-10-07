#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <fstream>

#include <QString>

using namespace std;

enum Estado
{
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
    e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29,
    e30, e31, e32, e33, e34, e35, e36,
    ESTADOS, ACP, ERR = -1
};

enum Entrada
{
    _0,_1_7, _8_9, A_D, E, F, G_W, X, Y_Z, ARIT_SIM, ARIT_COM, DELIM, PUNTO, COMILLAS,
    DOS_PUNTOS, IGUAL, MAYOR_QUE, MENOR_QUE, NEGACION, OR, AND, GUION_BAJO, COMILLA, DIVISOR,
    ASTERISCO, ESPACIO, OTRO,
    ENTRADAS
};

enum
{
    INTERRUMPE, VALOR, CANAL, CONTS, CONTINUA, DEFECTO, DIFER, SINO, DESDE,
    FUNCION, IR, IR_A, SI, IMPORTAR, INTERFAZ, MAPA, PAQUETE, RANGO, REGRESA,
    SELECCIONA, ESTRUCTURA, CASO, TIPO, ENTERO, VAR, DIV, MOD, LOGICO,
    CARACTER, ALFABETICO, REAL,
    PALABRAS_RESERVADAS
};

class Compilador
{
    public:
        Compilador(string rutaArchivo);
        void realizarMagia();

    private:
        Entrada siguienteEntrada(char caracter);
        void separarNombreArchivo(const string& str);
        string lexico( string renglon );
        bool esPalabraReservada( string palabra );
        bool esConstanteLogica( string palabra );
        void hacerAnalisisSintactico( void );

    public:
        string _rutaAlArchivo;
        string _nombreArchivo;

    private:
        bool enComentarioMultilinea;
        string renglon;
        unsigned int _lineaActual;
        unsigned int _columnaActual;
        string token;
        string _rutaCompletaArchivo;


        ofstream salidaInformacion;
        ofstream salidaErrores;

    private:
        static const Estado matriz_transiciones[ESTADOS][ENTRADAS];
        static const string palabras_reservadas[PALABRAS_RESERVADAS];
        const string IDENTIFICADOR;
        const string ENTERO;
        const string HEXADECIMAL;
        const string OCTAL;
        const string REAL;
        const string DELIMITADOR;
        const string OPER_ARITMETICA;
        const string ALFABETICO;
        const string OPER_BINARIO;
        const string OPER_LOGICO;
        const string COMPARACION;
        const string ASIGNACION;
        const string RESERVADA;
        const string CONST_LOGICA;
        const string CARACTER;
        const string COMENTARIO;
        const string ERROR;


};


#endif // COMPILADOR_H
