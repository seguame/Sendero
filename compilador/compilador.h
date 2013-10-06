#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <QString>

using namespace std;

enum Estado
{
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
    e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
    ESTADOS, ACP, ERR
};

enum Entrada
{
    _0,_1_7, _8_9, A_D, E, F, G_W, X, Y_Z, ARIT_SIM, ARIT_COM, DELIM, PUNTO, COMILLAS,
    DOS_PUNTOS, IGUAL, MAYOR_QUE, MENOR_QUE, NEGACION, OR, AND, GUION_BAJO,
    ENTRADAS
};

class Compilador
{
    public:
        Compilador(QString rutaArchivo);

    private:
        static const Estado matriz_transiciones[ESTADOS][ENTRADAS];
};


#endif // COMPILADOR_H
