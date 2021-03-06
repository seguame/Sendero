#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <fstream>
#include "enumerators.h"
#include "tablasimbolos.h"

using namespace std;

class TablaSimbolos;

class Compilador
{
    public:
        Compilador(string rutaArchivo);
        ~Compilador();
        void realizarMagia( void );
        string getRutaAlArchivo( void );
        string getNombreArchivo( void );
        void escribirError(string error);

        Tipo determinarTipo(std::string t)
        {
            if(t.compare(REAL) == 0) return T_REAL;
            if(t.compare(ENTERO) == 0) return T_ENTERO;
            if(t.compare(HEXADECIMAL) == 0) return T_ENTERO;
            if(t.compare(OCTAL) == 0) return T_ENTERO;
            if(t.compare(CONST_LOGICA) == 0) return T_BOOLEANO;
            if(t.compare(ALFABETICO) == 0) return T_CADENA;
            if(t.compare(CARACTER) == 0) return T_CARACTER;

            return T_INVALIDO;
        }

    private:
        Entrada siguienteEntrada(char caracter);
        void separarNombreArchivo(const string& str);
        void leerLexema( void );
        string siguienteLexema(void);
        bool esPalabraReservada( string palabra );
        bool esConstanteLogica( string palabra );
        void hacerAnalisisSintactico( void );
        void saltarLineasEnBlanco( void );

        void escribirLog( void );

        ////////////////////
        ifstream compilable;
        bool enComentarioMultilinea;
        bool finDeArchivo;
        bool existeFuncionPrincipal;

        string token;
        string _rutaCompletaArchivo;
        string _rutaAlArchivo;
        string _nombreArchivo;

        string renglon;
        unsigned int _lineaActual;
        unsigned int _columnaActual;

        //Bandera de control para las 2 librerias realmente requeridas
        bool fmtDefinido;
        bool conDefinido;

        //Esta bandera controla el modo de declaracion de variables,
        //las globales y constantes se acomodan distinto y por tanto
        //no deben ser parseadas al jvm del mismo modo
        bool evaluarDespues;


        ofstream salidaInformacion;

        static const Estado matriz_transiciones[ESTADOS][ENTRADAS];
        static const string palabras_reservadas[PALABRAS_RESERVADAS];
        static const Tipo operacionesSuma[CANT_TIPOS][CANT_TIPOS];
        static const Tipo operacionMulti[CANT_TIPOS][CANT_TIPOS];
        static const Tipo operacionPotencia[CANT_TIPOS][CANT_TIPOS];
        static const bool operacionAsignacion[CANT_TIPOS][CANT_TIPOS];
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
        void bloque();
        bool vars (bool darAvanceAlFinal, bool global);
        void estatutos(void);
        bool comando(void);
        void asigna(void);
        void dimension(Simbolo* simb, bool verificarDimension);
        void expr(bool esOpcional);
        void opy(bool esOpcional);
        void opno(bool esOpcional);
        void oprel(bool esOpcional);
        void suma(bool esOpcional);
        void multi(bool esOpcional);
        void expo (bool esOpcional);
        void signo (bool esOpcional);
        void termino (bool esOpcional, bool invertirValor);
        bool constanteTipo(string tok);
        string lFunc_1(void);
        string lFunc_2(void);
        string vparam(void);
        bool si(void);
        bool desde(void);
        bool caso(void);
        bool regresa (void);
        bool lee(void);
        bool imprime(void);
        bool constante(void);



        ////////////////////////
        //Semantico
        TablaSimbolos* tablaDeSimbolos;

        bool obtenerTipoValorConstante(Simbolo* simb);
        string obtenerStringFirma(const string& firma);

};

#endif // COMPILADOR_H
