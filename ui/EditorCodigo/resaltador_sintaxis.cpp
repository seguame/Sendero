#include <QtGui>

#include "resaltador_sintaxis.h"

ResaltadorSintaxis::ResaltadorSintaxis(QTextDocument *padre)
    : QSyntaxHighlighter(padre)
{
    ReglaDeResaltado regla;

    formatoPalabraReservada.setForeground(Qt::darkMagenta);
    formatoPalabraReservada.setFontWeight(QFont::Bold);

    QStringList patronPalabraReservada;
    patronPalabraReservada  << "\\binterrumpe\\b" << "\\bvalor\\b" << "\\bcanal\\b"
                            << "\\bconst\\b" << "\\bcontinua\\b" << "\\bdefecto\\b"
                            << "\\bdifer\\b" << "\\bsino\\b" << "\\bdesde\\b"
                            << "\\bfuncion\\b" << "\\bir\\b" << "\\bir_a\\b"
                            << "\\bsi\\b" << "\\bimportar\\b" << "\\binterfaz\\b"
                            << "\\bmapa\\b" << "\\bpaquete\\b" << "\\brango\\b"
                            << "\\bregresa\\b" << "\\bselecciona\\b" << "\\bestructura\\b"
                            << "\\bcaso\\b" << "\\btipo\\b" << "\\bentero\\b"
                            << "\\bvar\\b" << "\\bdiv\\b" << "\\bmod\\b"
                            << "\\blogico\\b" << "\\bcaracter\\b" << "\\balfabetico\\b"
                            << "\\breal\\b" << "\\bVerdadero" << "\\bFalso\\b";

    foreach (const QString &patron, patronPalabraReservada)
    {
        regla.patron = QRegExp(patron);
        regla.formato = formatoPalabraReservada;
        reglasDeResaltado.append(regla);
    }


    formatoClase.setFontWeight(QFont::Bold);
    formatoClase.setForeground(Qt::darkBlue);
    regla.patron = QRegExp("\\bQ[A-Za-z]+\\b");
    regla.formato = formatoClase;
    reglasDeResaltado.append(regla);


    formatoComentarioUnaLinea.setForeground(Qt::darkRed);
    regla.patron = QRegExp("//[^\n]*");
    regla.formato = formatoComentarioUnaLinea;
    reglasDeResaltado.append(regla);

    formatoComentarioMultiLinea.setForeground(Qt::red);


    formatoEncomillado.setForeground(Qt::darkGreen);
    regla.patron = QRegExp("\".*\"");
    regla.formato = formatoEncomillado;
    reglasDeResaltado.append(regla);


    formatoFuncion.setFontItalic(true);
    formatoFuncion.setForeground(Qt::blue);
    regla.patron = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    regla.formato = formatoFuncion;
    reglasDeResaltado.append(regla);


    expresionInicioComentario = QRegExp("/\\*");
    expresionFinComentario = QRegExp("\\*/");
}


void ResaltadorSintaxis::highlightBlock(const QString &texto)
{
    foreach (const ReglaDeResaltado &regla, reglasDeResaltado)
    {
        QRegExp expresion(regla.patron);
        int indice = expresion.indexIn(texto);

        while (indice >= 0)
        {
            int longitud = expresion.matchedLength();
            setFormat(indice, longitud, regla.formato);
            indice = expresion.indexIn(texto, indice + longitud);
        }
    }

    setCurrentBlockState(0);



    int indiceDeInicio = 0;

    if (previousBlockState() != 1)
    {
        indiceDeInicio = expresionInicioComentario.indexIn(texto);
    }


    while (indiceDeInicio >= 0)
    {

        int indiceFinal = expresionFinComentario.indexIn(texto, indiceDeInicio);
        int tamanioComentario;

        if (indiceFinal == -1)
        {
            setCurrentBlockState(1);
            tamanioComentario = texto.length() - indiceDeInicio;
        }
        else
        {
            tamanioComentario = indiceFinal - indiceDeInicio
                                + expresionFinComentario.matchedLength();
        }

        setFormat(indiceDeInicio, tamanioComentario, formatoComentarioMultiLinea);
        indiceDeInicio = expresionInicioComentario.indexIn(texto, indiceDeInicio + tamanioComentario);
    }
}

