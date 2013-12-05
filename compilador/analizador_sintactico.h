#ifndef ANALIZADOR_SINTACTICO_H
#define ANALIZADOR_SINTACTICO_H

class AnalizadorSintactico
{
    public:
        void programa( void );
        void importar( void );
        void funcion( void );
        void params( void );
        void pars ( void );
        void tipo( void );
        void bloque ( void );
        void vars ( void );
        void estatutos( void );
        void comando ( void );
        void asigna (void);
        void dimension (void);
        void expr( void );
        void opy( void );
        void opno( void );
        void oprel( void );
        void suma( void );
        void multi(void);
        void expo (void);
        void signo (void);
        void termino (void);
        void constanteTipo(void);
        void lFunc_1(void);
        void lFunc_2(void);
        void vparam(void);
        void si(void);
        void desde(void);
        void caso(void);
        void regresa (void);
        void lee(void);
        void imprime(void);
        void constante(void);
};

#endif // ANALIZADORSINTACTICO_H
