#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <fstream>
#include <stack>


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
        void realizarMagia( void );
        string getRutaAlArchivo( void );
        string getNombreArchivo( void );

    private:
        Entrada siguienteEntrada(char caracter);
        void separarNombreArchivo(const string& str);
        void leerLexema( void );
        string siguienteLexema(void);
        bool esPalabraReservada( string palabra );
        bool esConstanteLogica( string palabra );
        void hacerAnalisisSintactico( void );
        void saltarLineasEnBlanco( void );

        void escribirError(string error);
        void escribirLog( void );

        ////////////////////
        ifstream compilable;
        bool enComentarioMultilinea;
        bool finDeArchivo;
        bool existeFuncionPrincipal;
        string renglon;
        unsigned int _lineaActual;
        unsigned int _columnaActual;
        string token;
        string _rutaCompletaArchivo;
        string _rutaAlArchivo;
        string _nombreArchivo;


        ofstream salidaInformacion;
        ofstream salidaErrores;

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
        ///////////////////////

        // Analizador Sintactico

        string lexico;

        void programa(void);
        bool importar(void);
        bool funcion(void);
        void params(void);
        bool pars();
        bool tipo(string lex);
        void bloque(bool avanzar);
        void vars (void);
        void estatutos(void);
        void comando(void);
        void asigna(bool checarIdentificador);
        bool dimension(void);
        void expr(bool esOpcional);
        void opy(bool esOpcional);
        void opno(bool esOpcional);
        void oprel(bool esOpcional);
        void suma(bool esOpcional);
        void multi(bool esOpcional);
        void expo (bool esOpcional);
        void signo (bool esOpcional);
        void termino (bool esOpcional);
        bool constanteTipo(string tok);
        bool lFunc_1(void);
        void lFunc_2(void);
        void vparam(void);
        void si(void);
        void desde(void);
        void caso(void);
        void regresa (void);
        bool lee(void);
        bool imprime(void);
        bool constante(void);
};

#endif // COMPILADOR_H
