#include <iostream>
#include <sstream>



#include <QDebug>

#include "compilador.h"
#include "analizador_sintactico.h"

const Estado Compilador::matriz_transiciones[ESTADOS][ENTRADAS] =
{
    //      0   1–7 8-9 a–d     e   f   g–w x   y–z +-  %^      ;,()[]{}    .   “   :   =   >   <   !   |   &   _    '      /       *       ESPACIO OTRO
    /*e0*/{e1,  e6, e6, e14,    e14,e14,e14,e14,e14,e15,e15,    e12,        e12,e16,e18,e20,e27,e21,e22,e24,e25,e13, e29,   e32,    e15,    ERR,    ERR},
    /*e1*/{e2,  e3, ERR,ERR,    ERR,ERR,ERR,e4, ERR,ACP,ACP,    ACP,        e7, ERR,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ERR, ERR,   ACP,    ACP,    ACP,    ERR},
    /*e2*/{e2,  e3, ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ACP,ACP,    ACP,        e7, ERR,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ERR, ERR,   ACP,    ACP,    ACP,    ERR},
    /*e3*/{e3,  e3, ERR,ERR,    ERR,ERR,ERR,ERR,ACP,ACP,ACP,    ACP,        ERR,ERR,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ERR, ERR,   ACP,    ACP,    ACP,    ERR},
    /*e4*/{e5,  e5, e5, e5,     e5, e5, ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR, ERR,   ERR,    ERR,    ERR,    ERR},
    /*e5*/{e5,  e5, e5, e5,     e5, e5, ERR,ERR,ERR,ACP,ACP,    ACP,        ERR,ERR,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ERR, ERR,   ACP,    ACP,    ACP,    ERR},
    /*e6*/{e6,  e6, e6, ACP,    e9, ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        e7, ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e7*/{e8,  e8, e8, ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR, ERR,   ERR,    ERR,    ERR,    ERR},
    /*e8*/{e8,  e8, e8, ACP,    e9, ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e9*/{e11, e11,e11,ERR,    ERR,ERR,ERR,ERR,ERR,e10,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR, ERR,   ERR,    ERR,    ERR,    ERR},
    /*e10*/{e11,e11,e11,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR, ERR,   ERR,    ERR,    ERR,    ERR},
    /*e11*/{e11,e11,e11,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e12*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e13*/{e14,e14,e14,e14,    e14,e14,e14,e14,e14,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,e14, ERR,   ERR,    ERR,    ERR,    ERR},
    /*e14*/{e14,e14,e14,e14,    e14,e14,e14,e14,e14,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,e14, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e15*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e16*/{e16,e16,e16,e16,    e16,e16,e16,e16,e16,e16,e16,    e16,        e16,e17,e16,e16,e16,e16,e16,e16,e16,e16, e16,   e16,    e16,    e16,    e16},
    /*e17*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e18*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e19,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e19*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e20*/{ERR,ERR,ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,e23,ERR,ERR,ERR,ERR,ERR,ERR, ERR,   ERR,    ERR,    ERR,    ERR},
    /*e21*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e23,ACP,e28,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e22*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e23,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e23*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e24*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,e26,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e25*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,e26,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e26*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e27*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,e23,e28,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e28*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e29*/{e30,e30,e30,e30,    e30,e30,e30,e30,e30,e30,e30,    e30,        e30,e30,e30,e30,e30,e30,e30,e30,e30,e30, ERR,   e30,    e30,    e30,    e30},
    /*e30*/{ERR,ERR,ERR,ERR,    ERR,ERR,ERR,ERR,ERR,ERR,ERR,    ERR,        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR, e31,   ERR,    ERR,    ERR,    ERR},
    /*e31*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP},
    /*e32*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   e33,    e34,    ACP,    ACP},
    /*e33*/{e33,e33,e33,e33,    e33,e33,e33,e33,e33,e33,e33,    e33,        e33,e33,e33,e33,e33,e33,e33,e33,e33,e33, e33,   e33,    e33,    e33,    e33},
    /*e34*/{e34,e34,e34,e34,    e34,e34,e34,e34,e34,e34,e34,    e34,        e34,e34,e34,e34,e34,e34,e34,e34,e34,e34, e34,   e34,    e35,    e34,    e34},
    /*e35*/{e34,e34,e34,e34,    e34,e34,e34,e34,e34,e34,e34,    e34,        e34,e34,e34,e34,e34,e34,e34,e34,e34,e34, e34,   e36,    e35,    e34,    e34},
    /*e36*/{e36,e36,e36,e36,    e36,e36,e36,e36,e36,e36,e36,    e36,        e36,e36,e36,e36,e36,e36,e36,e36,e36,e36, e36,   e36,    e36,    e36,    e36}

};

const string Compilador::palabras_reservadas[PALABRAS_RESERVADAS] =
{
    "interrumpe", "valor", "canal", "const", "continua", "defecto", "difer", "sino", "desde",
    "funcion", "ir", "ir_a", "si", "importar", "interfaz", "mapa", "paquete", "rango", "regresa",
    "selecciona", "estructura", "caso", "tipo", "entero", "var", "div", "mod", "logico",
    "caracter", "alfabetico", "real"
};

Compilador::Compilador(string rutaArchivo) :
    IDENTIFICADOR("identificador"),
    ENTERO("entero"),
    HEXADECIMAL("hexadecimal"),
    OCTAL("octal"),
    REAL("real"),
    DELIMITADOR("delimitador"),
    OPER_ARITMETICA("aritmetica"),
    ALFABETICO("alfanumerica"),
    OPER_BINARIO("binaria"),
    OPER_LOGICO("logico"),
    COMPARACION("comparacion"),
    ASIGNACION("asignacion"),
    RESERVADA("reservada"),
    CONST_LOGICA("constante logica"),
    CARACTER("caracter"),
    COMENTARIO("comentario"),
    ERROR("error")
{
    enComentarioMultilinea = false;
    _lineaActual = 0;
    _columnaActual = 0;
    _rutaCompletaArchivo = rutaArchivo;

    separarNombreArchivo(_rutaCompletaArchivo);

    //qDebug() << QString(_rutaAlArchivo.c_str());
    //qDebug() << QString(_nombreArchivo.c_str());
}


void Compilador::realizarMagia( void )
{
    hacerAnalisisSintactico();
}

void Compilador::hacerAnalisisSintactico()
{
    ifstream compilable;
    salidaInformacion.open((_rutaAlArchivo+"/"+_nombreArchivo+".lexemas").c_str());
    salidaErrores.open((_rutaAlArchivo+"/"+_nombreArchivo+".errores").c_str());

    compilable.open(_rutaCompletaArchivo.c_str());

    if(!compilable.is_open())
    {
        qDebug() << "No se pudo abrir el archivo a compilar";
        return;
    }

    //salidaInformacion << "-----------------------------------------" << endl;
    //salidaInformacion << "TOKEN \t LEXEMA" << endl;
    //salidaInformacion << "-----------------------------------------" << endl;

    //salidaErrores << "------------------------------------------------------------------------------------------------" << endl;
    //salidaErrores << "LINEA \t COL \t ERROR \t\t DESCRIPCCION \t\t\t LINEA ERROR" << endl;
    //salidaErrores << "------------------------------------------------------------------------------------------------" << endl;

    string linea;
    string lex;
    while(getline(compilable, linea))
    {
        //stringstream ss;
        //ss << (_lineaActual + 1);
        //qDebug() << QString((ss.str() + " " + linea).c_str());

        while(_columnaActual < linea.size())
        {
            lex = siguienteLexema(linea);
            //qDebug() << QString((token + "\t" + lex).c_str());
            if(lex.compare("") != 0)
            {
                salidaInformacion << (token + ",,," +  lex) << endl;
            }
        }
        _lineaActual++;
        _columnaActual = 0;
    }

    if(enComentarioMultilinea) //se acabo el archivo y no se cerro el comentario
    {
        stringstream ss;
        ss << _lineaActual;
        salidaErrores << ss.str() << ",,," << 0 << ",,," << lex << ",,,Fin de archivo inesperado,,," << linea << endl;
    }

    //qDebug() << "Terminamos";
    compilable.close();
    salidaInformacion.close();
    salidaErrores.close();
}


string Compilador::siguienteLexema(string renglon)
{
    Entrada entrada;
    Estado estado;
    Estado estadoAnterior;
    string lexema = "";
    char c;
    size_t tamanio_linea = renglon.size();
    token = ERROR;

    if(enComentarioMultilinea)
    {
        //el salto de linea elimina el * que pudiera estar en el estado 35
        estadoAnterior = estado = e34;
    }
    else
    {
        estadoAnterior = estado = e0;
    }


    while (estado != ERR && estado != ACP && _columnaActual < tamanio_linea)
    {
        c = renglon.at(_columnaActual++);

        if(estado == e0 || estado == e34)
        {
            while(c == ' ' || c == '\t' || c == '\n')
            {
                if((_columnaActual + 1) > tamanio_linea)
                {
                    token = "";
                    return "";
                }
                else
                {
                    c = renglon.at(_columnaActual++);
                }

            }
        }

        entrada = siguienteEntrada(c);

        if(estado != ERR && estado != ACP)
        {
            estadoAnterior = estado;
            estado = matriz_transiciones[estado][entrada];
            /*int f = estado;
            stringstream ss;
            ss << f;
            qDebug() << c << QString((ss.str().c_str()));*/
            if(estado != ERR && estado != ACP)
            {
                lexema += c;
            }
            else
            {
                _columnaActual--;
            }
        }

        if(entrada == OTRO && estado != e0 && estado != e16 && estado != e33 && estado != e34)
        {
            estadoAnterior = estado;
            estado = ACP;
        }

        /*if(entrada == ESPACIO && estado != e16 && estado != e33 && estado != e34 && estado != e35)
        {
            estado = ACP;
        }*/

        if(_columnaActual == tamanio_linea)
        {
            estadoAnterior = estado;
            estado = ACP;
        }

    }

    if(estado == ACP)
    {

        switch(estadoAnterior)
        {
            case e1: case e2: case e6:
                token = ENTERO;
                break;
            case e3:
                token = OCTAL;
                break;
            case e5:
                token = HEXADECIMAL;
                break;
            case e8: case e11:
                token = REAL;
                break;
            case e12: case e18:
                token = DELIMITADOR;
                break;
            case e14:
                token = IDENTIFICADOR;
                if(esPalabraReservada(lexema))
                {
                    token = RESERVADA;
                }
                else if(esConstanteLogica(lexema))
                {
                    token = CONST_LOGICA;
                }
                break;
            case e15: case e32:
                token = OPER_ARITMETICA;
                break;
            case e17:
                token = ALFABETICO;
                break;
            case e19:
                token = ASIGNACION;
                break;
            case e22: case e26:
                token = OPER_LOGICO;
                break;
            case e24: case e25: case e28:
                token = OPER_BINARIO;
                break;
            case e23: case e21: case e27:
                token = COMPARACION;
                break;
            case e31:
                token = CARACTER;
                break;
            case e33:
                token = COMENTARIO;
                break;

            // casos especiales en el comentario multilinea
            case e34: case e35:
                enComentarioMultilinea = true;
                token = COMENTARIO;
                break;

            case e36:
                token = COMENTARIO;
                enComentarioMultilinea = false;
                break;

            default:
                //token = ERROR;
                break;
        }

        if(estado == e36) //llegó fin de linea y no logramos ponerlo en anterior
        {
            token = COMENTARIO;
            enComentarioMultilinea = false;
        }
    }
    else if(estado == ERR)
    {
        stringstream linea;
        stringstream colum;

        linea << (_lineaActual + 1);
        salidaErrores << linea.str() << ",,,";
        colum << _columnaActual;
        salidaErrores << colum.str() << ",,,";
        salidaErrores << lexema << ",,,";

        switch(estadoAnterior)
        {
            case e4: case e5:
                salidaErrores << "Constante hexadecimal incompleta. Se esperaba 0-9|A-F";
                break;

            case e1:
                salidaErrores << "Valor Octal|Hexadecimal|Decimal invalido. Se esperaba 0-7|.|X";
                break;
            case e2: case e3:
                salidaErrores << "Valor Octal invalido. Se esperaba 0-7";
                break;
            case e7:
                salidaErrores << "Falta parte decimal. Se esperaba digito";
                break;

            case e9:
                salidaErrores << "Falta parte exponencial. Se esperaba +|-|digito";
                break;

            case e10:
                salidaErrores << "Falta parte exponencial. Se esperaba digito";
                break;

            case e13:
                salidaErrores << "_ invalido como identificador. Se espera _+|[letra|digito]+";
                break;

            case e16:
                salidaErrores << "Fin de cadena no encontrado. Se esperaba \"";
                break;

            case e20:
                salidaErrores << "Se esperaba =|: despues de =";
                break;

            case e29:
                salidaErrores << "se esperaba un caracter cualquiera excepto \"'\"";
                break;

            case e30:
                salidaErrores << "se esperaba\"'\"";
                break;

            default:
                salidaErrores << "DEBUG";
                break;
        }

        //Remover los espacios en blanco laterales del renglon
        //No se hace antes para no moficiar el conteo de columnas
        remove(renglon.begin(), renglon.end(), ' ');

        salidaErrores << " y llego: " << c << ",,," << renglon << endl;
    }
    else
    {
        throw "No se que ocurrio aqui";
    }

    return lexema;
}


Entrada Compilador::siguienteEntrada(char caracter)
{
    //stringstream ss;
    //ss << _columnaActual;
    //qDebug() << QString(((ss.str()+ " ")+=caracter).c_str());
    char c = tolower(caracter);
    switch(c)
    {
        case '0':
            return _0;

        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7':
            return _1_7;

        case '8': case '9':
            return _8_9;

        case 'a': case 'b': case 'c': case 'd':
            return A_D;

        case 'e':
            return E;

        case 'f':
            return F;

        case 'g': case 'h': case 'i': case 'j': case 'k':
        case 'l': case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w':
            return G_W;

        case 'x':
            return X;

        case 'y': case 'z':
            return Y_Z;

        case '+': case '-':
            return ARIT_SIM;

        case '%': case '^':
            return ARIT_COM;

        case ',': case '{': case '}': case ';':
        case '[': case ']': case '(': case ')':
            return DELIM;

        case '.':
            return PUNTO;

        case '"':
            return COMILLAS;

        case ':':
            return DOS_PUNTOS;

        case '=':
            return IGUAL;

        case '>':
            return MAYOR_QUE;

        case '<':
            return MENOR_QUE;

        case '!':
            return NEGACION;

        case '|':
            return OR;

        case '&':
            return AND;

        case '_':
            return GUION_BAJO;

        case '\'':
            return COMILLA;

        case '/':
            return DIVISOR;

        case '*':
            return ASTERISCO;

        case ' ':
            return ESPACIO;

        default:
            //int f = c;
            //stringstream ss;
            //ss << f;
            //qDebug() << QString((string("Caracter no reconocido: ")+=c).c_str());
            //qDebug() << QString((ss.str().c_str()));
            return OTRO;
    }
}

bool Compilador::esPalabraReservada( string palabra )
{
    for(int i = 0; i < PALABRAS_RESERVADAS; i++)
    {
        if (palabra.compare(palabras_reservadas[i]) == 0)
        {
            return true;
        }
    }

    return false;
}

bool Compilador::esConstanteLogica( string palabra )
{
    return ((palabra.compare("Verdadero") == 0 ) || palabra.compare("Falso") == 0);
}

void Compilador::separarNombreArchivo (const string& str)
{
    size_t pos1 = str.find_last_of("/\\");
    size_t pos2 = str.find_last_of(".");

    _rutaAlArchivo = str.substr(0 ,pos1);
    _nombreArchivo = str.substr(pos1 + 1, pos2 - pos1 - 1);
}

string Compilador::getRutaAlArchivo( void )
{
    return _rutaAlArchivo;
}

string Compilador::getNombreArchivo( void )
{
    return _nombreArchivo;
}
