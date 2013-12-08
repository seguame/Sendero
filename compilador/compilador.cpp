#include <iostream>
#include <sstream>

#include <QDebug>

#include "compilador.h"

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
    /*e36*/{ACP,ACP,ACP,ACP,    ACP,ACP,ACP,ACP,ACP,ACP,ACP,    ACP,        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP,   ACP,    ACP,    ACP,    ACP}

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
    ALFABETICO("alfabetico"),
    OPER_BINARIO("binaria"),
    OPER_LOGICO("logico"),
    COMPARACION("comparacion"),
    ASIGNACION("asignacion"),
    RESERVADA("reservada"),
    CONST_LOGICA("logico"),
    CARACTER("caracter"),
    COMENTARIO("comentario"),
    ERROR("error")
{
    enComentarioMultilinea = false;
    _lineaActual = -1;
    _columnaActual = 0;
    _rutaCompletaArchivo = rutaArchivo;
    renglon = "";
    finDeArchivo = false;
    existeFuncionPrincipal = false;

    separarNombreArchivo(_rutaCompletaArchivo);

    //qDebug() << QString(_rutaAlArchivo.c_str());
    //qDebug() << QString(_nombreArchivo.c_str());
}


void Compilador::realizarMagia(void)
{
    salidaInformacion.open((_rutaAlArchivo+"/"+_nombreArchivo+".lexemas").c_str());
    salidaErrores.open((_rutaAlArchivo+"/"+_nombreArchivo+".errores").c_str());

    compilable.open(_rutaCompletaArchivo.c_str());

    if(!compilable.is_open())
    {
        qDebug() << "No se pudo abrir el archivo a compilar";
        return;
    }


    ///////////////////////////////

    hacerAnalisisSintactico();


    //////////////////////////////

    if(enComentarioMultilinea) //se acabo el archivo y no se cerro el comentario
    {
        stringstream ss;
        ss << _lineaActual;
        salidaErrores << ss.str() << ",,," << 0 << ",,," << lexico << ",,,Fin de archivo inesperado,,," << renglon << endl;
    }

    compilable.close();
    salidaInformacion.close();
    salidaErrores.close();
}

void Compilador::hacerAnalisisSintactico(void)
{
    programa();
}

void Compilador::saltarLineasEnBlanco(void)
{
    while(renglon.compare("") == 0)
    {
        if(getline(compilable, renglon))
        {
            _lineaActual++;
            _columnaActual = 0;
        }
        else
        {
            finDeArchivo = true;
            break;
        }
    }
}

void Compilador::escribirError(string error)
{
    stringstream linea;
    stringstream colum;

    linea << (_lineaActual + 1);
    salidaErrores << linea.str() << ",,,";
    colum << _columnaActual;
    salidaErrores << colum.str() << ",,,";
    salidaErrores << lexico << ",,,";

    salidaErrores << error << ",,," << renglon << endl;
}

void Compilador::escribirLog(void)
{

    if(lexico.compare("") != 0 && !finDeArchivo)
        salidaInformacion << (token + ",,," +  lexico) << endl;
}

void Compilador::leerLexema(void)
{
    do
    {
        lexico = siguienteLexema();
        escribirLog();
    }while(token.compare(COMENTARIO) == 0);

}


string Compilador::siguienteLexema(void)
{
    token = ERROR;
    string lexema = "";

    if(_columnaActual  >= renglon.size())
    {
        renglon = "";
        saltarLineasEnBlanco();
    }

    if(finDeArchivo) return "";



    Entrada entrada;
    Estado estado;
    Estado estadoAnterior;
    char c = '\0';
    size_t tamanio_linea = renglon.size();


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

        if(_columnaActual >= tamanio_linea)
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
                salidaErrores << "Se esperaba = | : despues de =";
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
        //remove(renglon.begin(), renglon.end(), ' ');

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




//==============================================================
//
//                PARSER !!!!!!!!!!!!!!
//
//==============================================================



void Compilador::programa(void)
{
    leerLexema();

    if(lexico.compare("paquete") != 0)
    {
        escribirError("Se esperaba definicion de paquete");
    }

    leerLexema();

    if(lexico.compare("principal") != 0)
    {
        escribirError("Se esperaba definicion de \"principal\"");
    }

    do
    {
        leerLexema();

        if(lexico.compare("importar") == 0)
            importar();
        else if(lexico.compare("funcion") == 0)
            funcion();
        else if(!finDeArchivo)
            escribirError("Se espera funcion o importe de paquete");

    }while(!finDeArchivo);

    if(!existeFuncionPrincipal)
        escribirError("No se encontro la funcion \"principal\"");
}

void Compilador::importar(void)
{
    bool valido = false;
    leerLexema();

    if(lexico.compare("(") == 0)
    {
        do
        {
            leerLexema();

            //validar que no tenga parentesis vacios
            if(!valido && token.compare(ALFABETICO) == 0)
            {
                valido = true;
            }
        }while(token.compare(ALFABETICO) == 0);

        if(!valido)
        {
            escribirError("Faltan los paquetes a importar");
        }

        if(lexico.compare(")") != 0)
        {
            escribirError("Se esperaba constantes alfabetica o cierre de parentesis");
        }
    }
    else if(token.compare(ALFABETICO) != 0)
    {
        escribirError("Se esperaba constantes alfabetica");
    }
}


//TODO: chequeo de que si la funcion retorna tipo, este sea valido
void Compilador::funcion(void)
{
    leerLexema();

    if(token.compare(IDENTIFICADOR) != 0)
        escribirError("Se esperaba identificador de funcion");
    else
    {
        if(lexico.compare("principal") == 0)
        {
            if(!existeFuncionPrincipal)
                existeFuncionPrincipal = true;
            else
                escribirError("la funcion \"principal\" ya fue declarada");
        }
    }

    params();

    leerLexema();

    //si lo consecuente es un "tipo" avanzar otro token dentro del bloque
    //si no lo era, validar en bloque que lo que sigue es un {
    if(tipo(lexico))
    {
        bloque(true);
    }
    else
    {
        bloque(false);
    }
}

void Compilador::params(void)
{
    leerLexema();

    if(lexico.compare("(") != 0)
        escribirError("Se esperaba apertura de parentesis despues de nombre de funcion");


    if(pars()) //si habia parametros
    {
        if(lexico.compare(")") != 0)
            escribirError("Se esperaba cierre de parentesis");
    }
    //else, no se checa cierre de parentesis, se hizo en pars
}

bool Compilador::pars(void)
{
    leerLexema();

    if(lexico.compare(")") == 0)
        return false; //funcion sin parametros, valido, se retorna que no habia

    // la funcion tiene parametros

    bool primeraVuelta = true;

    do
    {
        do
        {
            if(!primeraVuelta)
            {
                if(lexico.compare(",") != 0)
                    escribirError("Se esperaba coma ','");
                else
                    leerLexema();
            }

            if(token.compare(IDENTIFICADOR) != 0)
                escribirError("Se esperaba identificador de variable");

            if(primeraVuelta)
                primeraVuelta = false;

            leerLexema();

        }while(lexico.compare(",") == 0);

        if(!tipo(lexico))
        {
            escribirError("Se esperaba el tipo de variable");
        }

        leerLexema();

    }while(lexico.compare(",") == 0);

    return true;
}

bool Compilador::tipo( string lex )
{
    return (lex.compare(REAL) == 0 ||
            lex.compare(ENTERO) == 0 ||
            lex.compare(CONST_LOGICA) == 0 ||
            lex.compare(ALFABETICO) == 0);
}

void Compilador::bloque (bool avanzar)
{
    qDebug() << "bloque";
    if(avanzar)
        leerLexema();

    if(lexico.compare("{") != 0)
        escribirError("Es esperaba apertura de bloque {");

    leerLexema();

    vars();
    estatutos();

    if(lexico.compare("}") != 0)
        escribirError("Se esperaba cierre de bloque }");
}

void Compilador::vars (void)
{
    qDebug() << "vars";
    if(lexico.compare("var") != 0)
        return; //los caminos de la vida, no son lo que yo esperaba (8)

    leerLexema();

    if(token.compare(IDENTIFICADOR) == 0)
    {
        leerLexema();

        if(!tipo(lexico))
            escribirError("Se esperaba definicion de tipo");
    }
    else if(lexico.compare("(") == 0)
    {
        leerLexema();
        bool primeraVuelta = true;

        do
        {
            do
            {
                if(!primeraVuelta)
                {
                    if(lexico.compare(",") != 0)
                        escribirError("Se esperaba coma ','");
                    else
                        leerLexema();
                }

                if(token.compare(IDENTIFICADOR) != 0)
                    escribirError("Se esperaba identificador de variable");

                if(primeraVuelta)
                    primeraVuelta = false;

                leerLexema();

            }while(lexico.compare(",") == 0);

            if(!tipo(lexico))
            {
                escribirError("Se esperaba el tipo de variable");
            }

            leerLexema();

        }while(lexico.compare(",") == 0);

        if(lexico.compare(")") != 0)
        {
            escribirError("Se esperaba cierre de parentesis");
        }
    }
    else
    {
        escribirError("Se esperaba un identificador o grupo de estos");
    }

    leerLexema();
}

void Compilador::estatutos(void)
{
    qDebug() << "Estatutos";
    comando();
}

//TODO, que retorne true si existio comando
void Compilador::comando (void)
{
    qDebug() << "comando";
    if(token.compare(IDENTIFICADOR) == 0)
    {
        leerLexema();
        //separados del resto porque ambos inician con un identificador
        if(!lFunc_1())
        {
            asigna();
        }
        leerLexema();
    }
    else
    {
        si();
        desde();
        caso();
        regresa();
        lee();
        imprime();

        if(lexico.compare("interrumpe") == 0)
        {

        }
        else if(lexico.compare("continua") == 0)
        {

        }

    }

}

void Compilador::asigna (void)
{
    qDebug() << "asigna";
    if(dimension())
    {
        leerLexema();
    }

    if(lexico.compare(":=") != 0)
    {
            escribirError("Se esperaba operador de asignacion \":=\"");
    }
    leerLexema();
    expr();
}

bool Compilador::dimension (void)
{
    qDebug() << "dimension";
    if(lexico.compare("[") != 0)
        return false; //las dimensiones de la vida, no son lo que yo esperaba (8)
    return false;
}

void Compilador::expr(void)
{
    qDebug() << "expr";
}

void Compilador::opy(void)
{

}

void Compilador::opno( void )
{

}

void Compilador::oprel( void )
{

}

void Compilador::suma( void )
{

   /* bool hay_operacion = false;
    string siguiente;
    lexico = siguienteLexema();

    do
    {
        if((lexico.compare("+") == 0 || lexico.compare("-") == 0))
        {
            siguiente = lexico;
            hay_operacion = true;
            lex = lexico();
        }

        multiplicacionDivisionModulo();

        if (hay_operacion)
        {
            hay_operacion = false;

            hacerMagiaDeTresDirecciones(siguiente);

            operador_derecho = pila_valores->top();
            pila_valores->pop();

            operador_izquierdo = pila_valores->top();
            pila_valores->pop();

            if(siguiente.compare("+") == 0)
            {
                resultado = operador_izquierdo + operador_derecho;
            }
            else if (siguiente.compare("-") == 0)
            {
                resultado = operador_izquierdo - operador_derecho;
            }
            else
            {
                cerr << "Algo fue mal en el analisis: Evaluador::sumaResta" << endl;
                exit(1);
            }

            pila_valores->push(resultado);

        }
    } while (lex.compare("+") == 0 || lex.compare("-") == 0);*/
}

void Compilador::multi(void)
{

}

void Compilador::expo (void)
{

}

void Compilador::signo (void)
{

}

void Compilador::termino (void)
{

}

void Compilador::constanteTipo(void)
{

}

bool Compilador::lFunc_1(void)
{
    qDebug() << "lFunc_1";
    if(lexico.compare("(") != 0)
        return false;

    expr();

    leerLexema();

    if(lexico.compare(")") != 0)
    {
        escribirError("Se esperaba cierre de parentesis");
    }

    return true;
}

void Compilador::lFunc_2(void)
{

}

void Compilador::vparam(void)
{

}

void Compilador::si(void)
{
    qDebug() << "si";
    if(lexico.compare("si") != 0)
        return;

    expr();

    bloque(true);

    leerLexema();

    if(lexico.compare("sino") == 0)
    {
        bloque(true);
        leerLexema();
    }
}

void Compilador::desde(void)
{
    qDebug() << "desde";
    if(lexico.compare("desde") != 0)
        return;

    //asigna();

    leerLexema();

    if(lexico.compare(";") != 0)
        escribirError("Se esperaba delimitador de zona de asignacion");

    //expr();
    leerLexema();

    if(lexico.compare(";") != 0)
        escribirError("Se esperaba delimitador de zona de condicion");

    //asigna();
    leerLexema();

    if(lexico.compare(";") != 0)
        escribirError("Se esperaba delimitador de zona de incrementos");

    bloque(true);
    leerLexema();
}


void Compilador::caso(void)
{
    qDebug() << "caso";
    if(lexico.compare("caso") != 0)
        return;

    leerLexema();

    if(token.compare(IDENTIFICADOR) != 0)
        escribirError("Se esperaba la variable de \"caso\"");

    leerLexema();

    if(lexico.compare("{") != 0)
        escribirError("Se esperaba apertura de bloque de \"caso\"");

    leerLexema();

    do
    {
        if(lexico.compare("defecto") == 0)
        {
            leerLexema();
            if(lexico.compare(":") != 0)
            {
                escribirError("Se esperaba \":\"");
            }

            leerLexema();
            estatutos();

        }
        else if(lexico.compare("valor") == 0)
        {
            leerLexema();
            if(token.compare(ENTERO) != 0)
            {
                escribirError("Solo se admiten constantes enteras como casos");
            }

            leerLexema();
            if(lexico.compare(":") != 0)
            {
                escribirError("Se esperaba \":\"");
            }

            leerLexema();
            estatutos();
        }
        else
        {
            escribirError("Se esperaba \"valor\" | \"defecto\" en el bloque de caso");
        }
    }while(lexico.compare("valor") == 0 || lexico.compare("defecto") == 0);

    leerLexema();
    if(lexico.compare("}") != 0)
    {
        escribirError("Se esperaba cierre de bloque");
    }
}

void Compilador::regresa (void)
{

}

void Compilador::lee(void)
{

}

void Compilador::imprime(void)
{

}

void Compilador::constante(void)
{

}

