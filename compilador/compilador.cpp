#include <iostream>
#include <fstream>
#include <QDebug>

#include "compilador.h"

const Estado Compilador::matriz_transiciones[ESTADOS][ENTRADAS] =
{
    //      0   1–7 8-9 a–d     e   f   g–w x   y–z +-  */%^    ;,()[]{}    .   “   :   =   >   <   !   |   &   _
    /*e0*/ {e1, e6, e6, e14,    e14,e14,e14,e14,e14,e15,e15,    e12,        e12,e16,e18,e20,e27,e21,e22,e24,e25,e13},
    /*e1*/ {e2, e3, ERR,ERR,    ERR,ERR,ERR,e4, ERR,ERR,ERR,    ERR,        e7, ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e2*/ {e2, e3, ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e3*/ {e3, e3, ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e4*/ {e5, e5, e5, e5,     e5, e5, ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e5*/ {e5, e5, e5, e5,     e5, e5, ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e6*/ {e6, e6, e6, ACP,    e9, ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        e7, ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e7*/ {e8, e8, e8, ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e8*/ {e8, e8, e8, ACP,    e9, ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e9*/ {e11,e11,e11,ERR,    ERR,ERR,ERR,ERR,ERR,e10,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e10*/{e11,e11,e11,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e11*/{e11,e11,e11,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e12*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e13*/{e14,e14,e14,e14,    e14,e14,e14,e14,e14,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,e14},
    /*e14*/{e14,e14,e14,e14,    e14,e14,e14,e14,e14,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,e14},
    /*e15*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e16*/{e16,e16,e16,e16,    e16,e16,e16,e16,e16,e16,e16,    e16,        e16,e17,e16,e16,e16,e16,e16,e16,e16,e16},
    /*e17*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e18*/{ERR,ERR,ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,e19,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e19*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e20*/{ERR,ERR,ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,e23,ERR,ERR,ERR,ERR,ERR,ERR},
    /*e21*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e23,ACP,e28,ACP,ACP,ACP,ACP},
    /*e22*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e23,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e23*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e24*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,e26,ACP,ACP},
    /*e25*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,e26,ACP},
    /*e26*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*e27*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e23,e28,ACP,ACP,ACP,ACP,ACP},
    /*e28*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP}
};

Compilador::Compilador(QString rutaArchivo)
{
    qDebug() << rutaArchivo;
}
