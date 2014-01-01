#include <QDebug>

#include "compilador.h"
#include "utils/conversor.h"
#include "utils/reportadorerrores.h"
#include "manejador_class.h"

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

const Tipo Compilador::operacionesSuma[CANT_TIPOS][CANT_TIPOS] =
{
                //Entero    Real        Caracter    Cadena      Booleano
    /*Entero*/  {T_ENTERO,  T_REAL,     T_ENTERO,   T_INVALIDO, T_INVALIDO},
    /*Real*/    {T_REAL,    T_REAL,     T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Caracter*/{T_INVALIDO,T_INVALIDO, T_CARACTER, T_CADENA,   T_INVALIDO},
    /*Cadena*/  {T_INVALIDO,T_INVALIDO, T_CADENA,   T_CADENA,   T_INVALIDO},
    /*Booleano*/{T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO}
};

const Tipo Compilador::operacionMulti[CANT_TIPOS][CANT_TIPOS] =
{
                //Entero    Real        Caracter    Cadena      Booleano
    /*Entero*/  {T_ENTERO,  T_REAL,     T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Real*/    {T_REAL,    T_REAL,     T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Caracter*/{T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Cadena*/  {T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Booleano*/{T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO}
};

const Tipo Compilador::operacionPotencia[CANT_TIPOS][CANT_TIPOS] =
{
                //Entero    Real        Caracter    Cadena      Booleano
    /*Entero*/  {T_REAL,    T_REAL,     T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Real*/    {T_REAL,    T_REAL,     T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Caracter*/{T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Cadena*/  {T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO},
    /*Booleano*/{T_INVALIDO,T_INVALIDO, T_INVALIDO, T_INVALIDO, T_INVALIDO}
};

const bool Compilador::operacionAsignacion[CANT_TIPOS][CANT_TIPOS] =
{
                //Entero    Real        Caracter    Cadena      Booleano
    /*Entero*/  {true,      false,      false,      false,      false},
    /*Real*/    {true,      true,       false,      false,      false},
    /*Caracter*/{false,     false,      true,       false,      false},
    /*Cadena*/  {false,     false,      false,      true,       false},
    /*Booleano*/{false,     false,      false,      false,      true}
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
    OPER_LOGICO("operador logico"),
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

    fmtDefinido = false;
    conDefinido = false;
    evaluarDespues = false;

    separarNombreArchivo(_rutaCompletaArchivo);
    tablaDeSimbolos = new TablaSimbolos(this);
}

Compilador::~Compilador()
{
    delete tablaDeSimbolos;
}


void Compilador::realizarMagia(void)
{
    ReportadorErrores::ObtenerInstancia()->SetRuta(_rutaAlArchivo+"/"+_nombreArchivo);

    salidaInformacion.open((_rutaAlArchivo+"/"+_nombreArchivo+".lexemas").c_str());
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
        escribirError("Fin de archivo inesperado");
    }

    compilable.close();
    salidaInformacion.close();
}


void Compilador::hacerAnalisisSintactico(void)
{
    //Escribir cabecera del intermedario de bytecode
    ManejadorClass::ObtenerInstancia()->setNombreClase(_nombreArchivo);
    ManejadorClass::ObtenerInstancia()->escribirCabeceraClase();

    programa();


    ManejadorClass::ObtenerInstancia()->escribirArchivoParaEnsamblar(_rutaAlArchivo+"/"+_nombreArchivo);


    ManejadorClass::Ensamblar(_rutaAlArchivo, _nombreArchivo);
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
    ReportadorErrores::ObtenerInstancia()->escribirError(_lineaActual, _columnaActual, lexico, error, renglon);
}

void Compilador::escribirLog(void)
{

    if(lexico.compare("") != 0 && !finDeArchivo && !token.compare(COMENTARIO) == 0)
        salidaInformacion << (token + ",,," +  lexico) << endl;
}

void Compilador::leerLexema(void)
{
    do
    {
        lexico = siguienteLexema();
        escribirLog();
    }while(((token.compare(COMENTARIO) == 0) || (lexico.compare("") == 0)) && !finDeArchivo);

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
        string error;

        switch(estadoAnterior)
        {
            case e4: case e5:
                error = "Constante hexadecimal incompleta. Se esperaba 0-9|A-F";
                break;

            case e1:
                error = "Valor Octal|Hexadecimal|Decimal invalido. Se esperaba 0-7|.|X";
                break;
            case e2: case e3:
                error = "Valor Octal invalido. Se esperaba 0-7";
                break;
            case e7:
                error = "Falta parte decimal. Se esperaba digito";
                break;

            case e9:
                error = "Falta parte exponencial. Se esperaba +|-|digito";
                break;

            case e10:
                error = "Falta parte exponencial. Se esperaba digito";
                break;

            case e13:
                error = "_ invalido como identificador. Se espera _+|[letra|digito]+";
                break;

            case e16:
                error = "Fin de cadena no encontrado. Se esperaba \"";
                break;

            case e20:
                error = "Se esperaba = | : despues de =";
                break;

            case e29:
                error = "se esperaba un caracter cualquiera excepto \"'\"";
                break;

            case e30:
                error = "se esperaba\"'\"";
                break;

            default:
                error = "DEBUG";
                break;
        }

        error = error + " y llego: " + c;

        escribirError(error);
    }
    else
    {
        throw "No se que ocurrio aqui";
    }

    return lexema;
}


Entrada Compilador::siguienteEntrada(char caracter)
{
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

    tablaDeSimbolos->nuevoScope(); // el Scope Global

    do
    {
        leerLexema();

        if(!importar() &&
           !funcion() &&
           !constante() &&
           !vars(false, true) &&
           !finDeArchivo)
        {
            escribirError("Se esperaba definicion de constantes, importaciones, o funciones");
        }

    }while(!finDeArchivo);

    //Antes de que se borre el scope global, guardar las dimensionadas globales
    ManejadorClass::ObtenerInstancia()->escribirConstructorEstatico();

    tablaDeSimbolos->borrarScope();

    if(!existeFuncionPrincipal)
        escribirError("No se encontro la funcion \"principal\"");
}

bool Compilador::importar(void)
{
    if(lexico.compare("importar") != 0)
        return false;


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

            if(lexico.compare("\"fmt\"") == 0)
            {
                fmtDefinido = true;
            }
            if(lexico.compare("\"con\"") == 0)
            {
                conDefinido = true;
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
    else
    {
        if(lexico.compare("\"fmt\"") == 0)
        {
            fmtDefinido = true;
        }
        if(lexico.compare("\"con\"") == 0)
        {
            conDefinido = true;
        }
    }

    return true;
}


//TODO: chequeo de que si la funcion retorna tipo, este sea valido
bool Compilador::funcion(void)
{
    if(lexico.compare("funcion") != 0)
        return false;

    Simbolo* func = NULL;
    Tipo tipoRetorno = T_INVALIDO;

    leerLexema();

    if(token.compare(IDENTIFICADOR) != 0)
        escribirError("Se esperaba identificador de funcion");
    else
    {
        if(lexico.compare("principal") == 0)
        {
            if(!existeFuncionPrincipal)
            {
                existeFuncionPrincipal = true;
            }
        }

        func = new Simbolo(lexico);
        func->setTipo(T_FUNCION);

        if(!tablaDeSimbolos->insertarSimbolo(func))
        {
            delete func;
            func = NULL;
        }
    }


    tablaDeSimbolos->entrarContextoFuncion(func);

    params();


    leerLexema();

    //si lo consecuente es un "tipo" avanzar otro token dentro del bloque
    //si no lo era, validar en bloque que lo que sigue es un {
    if(tipo(lexico))
    {
        tipoRetorno = determinarTipo(lexico);

        //setear que la funcion retorna algo
        tablaDeSimbolos->setTipoRetornoFuncion(tipoRetorno);

        leerLexema();
    }

    //Escribir la firma del metodo en el intermediario de bytecode
    ManejadorClass::ObtenerInstancia()->escribirCabeceraMetodo(func);

    bloque();



    tablaDeSimbolos->salirContextoFuncion();

    return true;
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
    string firmaFuncion = "";
    leerLexema();

    if(lexico.compare(")") == 0)
    {
        tablaDeSimbolos->setFirmaFuncion("6"); //6 es el tipo Void
        return false; //funcion sin parametros, valido, se retorna que no habia
    }

    // la funcion tiene parametros

    //preparando el apilamiento de variables para ser almacenadas
    //en la tabla de simbolos
    tablaDeSimbolos->prepararPilas();

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
            {
                escribirError("Se esperaba identificador de variable");
            }
            else
            {
                //lexico contiene el nombre del identificador
                //como son parametros de una funcion se sobreentiende que
                //van con un valor asociado, asi que se marcan como
                //que fueron inicializadas
                tablaDeSimbolos->apilarSimbolo(lexico, true);
            }


            if(primeraVuelta)
                primeraVuelta = false;

            leerLexema();

        }while(lexico.compare(",") == 0);

        if(!tipo(lexico))
        {
            tablaDeSimbolos->purgarPilas();
            escribirError("Se esperaba el tipo de variable");
        }
        else
        {
            //lexico contiene el tipo de dato a ser apilado
            firmaFuncion = firmaFuncion + tablaDeSimbolos->almacenarPilaSimbolos(determinarTipo(lexico), false);
        }

        leerLexema();

    }while(lexico.compare(",") == 0);

    //almacenar la firma de la funcion
    tablaDeSimbolos->setFirmaFuncion(firmaFuncion);

    //por si quedo algun elemento volando
    tablaDeSimbolos->purgarPilas();

    return true;
}

bool Compilador::tipo( string lex )
{
    return (lex.compare(REAL) == 0 ||
            lex.compare(ENTERO) == 0 ||
            lex.compare(CONST_LOGICA) == 0 ||
            lex.compare(CARACTER) == 0 ||
            lex.compare(ALFABETICO) == 0);
}

void Compilador::bloque ()
{
    qDebug() << "bloque";

    if(lexico.compare("{") != 0)
        escribirError("Es esperaba apertura de bloque {");

    // Se crea un scope interno
    tablaDeSimbolos->nuevoScope();

    leerLexema();

    estatutos();

    if(lexico.compare("}") != 0)
        escribirError("Se esperaba cierre de bloque }");

    // se elimina todo lo contenido dentro del scope
    tablaDeSimbolos->borrarScope();
}

bool Compilador::vars (bool darAvanceAlFinal, bool global)
{
    Simbolo* temp = NULL;

    qDebug() << "vars";
    if(lexico.compare("var") != 0)
        return false; //los caminos de la vida, no son lo que yo esperaba (8)

    //evaluar despues las dimensiones globales, se encarga el bytecode
    if(global)
        evaluarDespues = true;

    //preparando el apilamiento de variables para ser almacenadas
    //en la tabla de simbolos
    tablaDeSimbolos->prepararPilas();

    leerLexema();

    if(token.compare(IDENTIFICADOR) == 0)
    {
        temp = tablaDeSimbolos->apilarSimbolo(lexico, false);

        leerLexema();
        dimension(temp, false);

        if(!tipo(lexico))
        {
            tablaDeSimbolos->purgarPilas();
            escribirError("Se esperaba definicion de tipo");
        }
        else
        {
            //el lexico contiene el tipo de variable a almacenar
            tablaDeSimbolos->almacenarPilaSimbolos(determinarTipo(lexico), global);
        }
    }
    else if(lexico.compare("(") == 0)
    {
        leerLexema();
        bool primeraVuelta = true;


        /*
         * Se iran apilando los distintos identificadores
         * para al final determinar su tipo y por ultimo almacenarlos
         * en la tabla de simbolos si son validos [se le da su tipo]
         */
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

                if(primeraVuelta)
                    primeraVuelta = false;

                if(token.compare(IDENTIFICADOR) != 0)
                {
                    escribirError("Se esperaba identificador de variable");
                }
                else
                {
                    //lexico contiene el nombre del identificador
                    temp = tablaDeSimbolos->apilarSimbolo(lexico, false);
                }

                leerLexema();
                dimension(temp, false);

            }while(lexico.compare(",") == 0);

            if(!tipo(lexico))
            {
                tablaDeSimbolos->purgarPilas();
                escribirError("Se esperaba el tipo de variable");
            }
            else
            {
                //lexico contiene el tipo de dato a ser apilado
                tablaDeSimbolos->almacenarPilaSimbolos(determinarTipo(lexico), global);
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

    if(darAvanceAlFinal)
        leerLexema();

    //por si quedo algun elemento volando
    tablaDeSimbolos->purgarPilas();

    //fuera del ambito global, no importa
    evaluarDespues = false;

    return true;
}

void Compilador::estatutos(void)
{
    qDebug() << "Estatutos";

    bool requiereSeparador;

    do
    {
        if(lexico.compare(";") == 0)
        {
            leerLexema();
        }

        requiereSeparador = comando();


    }while(lexico.compare(";") == 0 || !requiereSeparador);
}


bool Compilador::comando (void)
{
    qDebug() << "comando";

    bool requiereSeparador = true;

    if(lexico.compare("var") == 0)
    {
        vars(true, false);
        requiereSeparador = false;
    }
    else if(token.compare(IDENTIFICADOR) == 0)
    {
        qDebug() << "inicia con identificador";

        if(lexico.compare("con") == 0)
        {
            if(!conDefinido)
            {
                escribirError("paquete \"con\" no fue importado");
            }
            lee();
        }
        else if(lexico.compare("fmt") == 0)
        {
            if(!fmtDefinido)
            {
                escribirError("paquete \"fmt\" no fue importado");
            }
            imprime();
        }
        else
        {
            //Antes no hago la lectura para checar si el identificador
            //que llegó es "con" o "fmt"

            Simbolo* temp = tablaDeSimbolos->buscarSimbolo(lexico);

            if(temp == NULL)
            {
                 asigna(); //para que siga el flujo normar de lectura, dentro se reporta el error
            }
            else
            {
                if(temp->getTipo() == T_FUNCION)
                {
                    leerLexema();

                    if(lexico.compare("(") != 0)
                    {
                        escribirError("Se esperaba apertura de parentesis despues de la funcion " + temp->getIdentificador());
                    }

                    string paramsObtenidos = lFunc_1();
                    string paramsEsperados = temp->getFirmaFuncion();

                    if(paramsEsperados.compare(paramsObtenidos) != 0)
                    {
                        escribirError("Error de parametros, la funcion espera " + obtenerStringFirma(paramsEsperados) +
                                      " y se está enviando " + obtenerStringFirma(paramsObtenidos));
                    }

                    ManejadorClass::ObtenerInstancia()->escribirLlamadaFuncion(temp);

                    leerLexema();
                }
                else
                {
                    asigna();
                }
            }
        }
    }
    else
    {

        if(lexico.compare("si") == 0)
        {
            si();
            requiereSeparador = false;
        }
        else if(lexico.compare("desde") == 0)
        {
            desde();
            requiereSeparador = false;
        }
        else if(lexico.compare("caso") == 0)
        {
            caso();
            requiereSeparador = false;
        }
        else if(lexico.compare("regresa") == 0)
        {
            regresa();
        }
        else if(lexico.compare("interrumpe") == 0)
        {
            leerLexema();
        }
        else if(lexico.compare("continua") == 0)
        {
            leerLexema();
        }
    }

    return requiereSeparador;
}

void Compilador::asigna (void)
{
    Simbolo* temp = NULL;
    Tipo buscado = T_INVALIDO;
    Tipo obtenido = T_INVALIDO;

    //Limpieza de la pila de tipos, apilaremos el tipo de la variable
    //destino para comparar al final que sea algo valido
    tablaDeSimbolos->prepararPilas();
    qDebug() << "asigna";

    if(token.compare(IDENTIFICADOR) != 0)
    {
        escribirError("Se espera un identificador para la asignacion");
    }
    else
    {
        temp = tablaDeSimbolos->buscarSimbolo(lexico);
        if(temp == NULL)
        {
            escribirError(lexico + " no esta definido");
        }
        else
        {
            if(temp->esConstante())
            {
                escribirError("Las constantes no pueden modificarse");
            }

            buscado = temp->getTipo();
        }
    }

    leerLexema();

    if(temp != NULL && temp->getCantidadDimensiones() > 0)
    {
        //se manda a llamar a la intruccion que cargue la referencia al array para almacenarle valor
        ManejadorClass::ObtenerInstancia()->escribirLlamadaVariable(temp, false);
    }

    //Dimension verificará que la variable encontrada cumpla
    //con lo especificado en ella, tambien apilara las dimensiones segun se requiera
    dimension(temp, true);

    if(lexico.compare(":=") != 0)
    {
            escribirError("Se esperaba operador de asignacion \":=\"");
    }
    leerLexema();

    //no es opcional el termino
    expr(false);

    //revisar que lo obtenido al final de la expresion sea del tipo
    //de dato al que se le va a asignar, como tambien se apilo desapilarlo
    obtenido = tablaDeSimbolos->desapilarTipo();

    if(temp != NULL && operacionAsignacion[buscado][obtenido])
    {
        //variables unidimensionadas se ponen despues de toda la carga de la expr
        //las dimensionadas, antes
        if(temp->getCantidadDimensiones() == 0)
        {
            ManejadorClass::ObtenerInstancia()->escribirLlamadaVariable(temp, true);
        }
        /*Simbolo* varTemp = tablaDeSimbolos->desapilarValor();

        if(varTemp == NULL)
        {
            qDebug() << "Intentado asignar, pero no hay valor en pila";
        }
        else if(varTemp->getIdentificador().compare("HOLDER") == 0)
        {
            switch(obtenido)
            {
            case T_ENTERO:
                ManejadorClass::ObtenerInstancia()->escribirEnteroConstante(varTemp->getValor<long>());
                break;
            }
        }
        else
        {
            //
        }*/

        temp->setInicializado();



    }
    else
    {
        escribirError("Asignacion de tipos invalida");
    }
}

void Compilador::dimension (Simbolo* simb, bool verificarDimensiones)
{
    qDebug() << "dimension";

    unsigned int contDimension = 0;

    if(lexico.compare("[") != 0)
    {
        if(verificarDimensiones)
        {
            if(simb != NULL && simb->getCantidadDimensiones() != 0)
            {
                ReportadorErrores::ObtenerInstancia()->escribirError(_lineaActual,
                                                                     _columnaActual,
                                                                     simb->getIdentificador(),
                                                                     "Variable debe tener dimension",
                                                                     renglon);
            }
        }

        return ; //las dimensiones de la vida, no son lo que yo esperaba (8)
    }

    if(verificarDimensiones)
    {
        if(simb != NULL && simb->getCantidadDimensiones() == 0)
        {
            ReportadorErrores::ObtenerInstancia()->escribirError(_lineaActual,
                                                                 _columnaActual,
                                                                 simb->getIdentificador(),
                                                                 "Variable no dimensionada",
                                                                 renglon);
        }
    }

    do
    {
        leerLexema();
        expr(false);

        //Validar que lo obtenido sea de tipo logico
        if(tablaDeSimbolos->desapilarTipo() != T_ENTERO)
        {
            escribirError("Conflicto en tipos en  dimension (solo admite entero)");
        }
        else
        {
            if(simb != NULL)
            {
                //cuando se verifica dimension no se especifica tamanio de simbolo, solo cuando se esta en expr
                if(!verificarDimensiones)
                {
                    Simbolo* simbTam = tablaDeSimbolos->desapilarValor();

                    if(!simbTam->esConstante() && simbTam->getIdentificador().compare("HOLDER") != 0)
                    {
                        escribirError("Las dimensiones deben definirse con valores constantes");
                    }
                    else
                    {

                        int valor = simbTam->getValor<int>();
                        simb->addTamanioDimension(valor);

                        if(simbTam->getIdentificador().compare("HOLDER") == 0)
                            delete simbTam;
                    }
                }
                else
                {
                    if(simb->getCantidadDimensiones()-1 > contDimension )
                    {
                        ManejadorClass::ObtenerInstancia()->aniadirInstruccion("    aaload","");
                    }
                }
            }
        }

        if(lexico.compare("]") != 0)
        {
            escribirError("Se esperaba cierre de corchetes");
        }
        else
        {
            ++contDimension;
        }

        leerLexema();
    }while(lexico.compare("[") == 0);

    if(verificarDimensiones)
    {
        if(simb != NULL && (simb->getCantidadDimensiones() != contDimension) && (simb->getCantidadDimensiones() != 0))
        {

            stringstream obtenidos;
            stringstream requeridos;

            obtenidos << contDimension;
            requeridos << simb->getCantidadDimensiones();

            ReportadorErrores::ObtenerInstancia()->escribirError(_lineaActual,
                                                                 _columnaActual,
                                                                 simb->getIdentificador(),
                                                                 "Variable incongruene en dimensiones, declarada con " + requeridos.str() + " y se le da " + obtenidos.str(),
                                                                 renglon);
        }
    }
    else
    {
        if(simb != NULL)
        {
            simb->setCantidadDimensiones(contDimension);
        }
    }

}

string Compilador::lFunc_1(void)
{
    qDebug() << "lFunc_1";

    string params = vparam();


    if(lexico.compare(")") != 0)
    {
        escribirError("Se esperaban parametros o cierre de parentesis de funcion");
    }

    return params;
}

string Compilador::lFunc_2(void)
{
    qDebug() << "lFunc_2";
    string params = vparam();

    if(lexico.compare(")") != 0)
        escribirError("Se esperaba ) para cerrar la expresion");

    leerLexema();

    return params;
}

string Compilador::vparam(void)
{
    stringstream paramObtenidos;
    bool opcional = true;

    do
    {
        if(lexico.compare(",") == 0)
        {
            opcional = false;
        }

        leerLexema();
        expr(opcional);

        paramObtenidos << tablaDeSimbolos->desapilarTipo();

    }while(lexico.compare(",") == 0);

    return paramObtenidos.str();
}

bool Compilador::si(void)
{
    qDebug() << "si";

    leerLexema();
    expr(false);

    //Validar que lo obtenido sea de tipo logico
    if(tablaDeSimbolos->desapilarTipo() != T_BOOLEANO)
    {
        escribirError("La expresion dada al \"si\" no es de tipo logica");
    }

    bloque();

    leerLexema();

    if(lexico.compare("sino") == 0)
    {
        leerLexema();
        bloque();
        leerLexema();
    }

    return true;
}

bool Compilador::desde(void)
{
    qDebug() << "desde";

    leerLexema();

    do
    {
        if(lexico.compare(",") == 0)
            leerLexema();

        //por si llega estando el cuerpo vacio
        if(lexico.compare(";") == 0)
            break;

        asigna();

    }while(lexico.compare(",") == 0);



    if(lexico.compare(";") != 0)
        escribirError("Se esperaba asignaciones separadas por coma o un delimitador de zona de asignacion");

    leerLexema();
    expr(true);


    if(lexico.compare(";") != 0)
        escribirError("Se esperaba delimitador de zona de condicion");

    leerLexema();
    do
    {
        if(lexico.compare(",") == 0)
            leerLexema();

        //por si llega estando el cuerpo vacio
        if(lexico.compare("{") == 0)
            break;

        asigna();

    }while(lexico.compare(",") == 0);

    //if(lexico.compare(";") != 0)
    //    escribirError("Se esperaba delimitador de zona de incrementos");

    bloque();
    leerLexema();

    return true;
}


bool Compilador::caso(void)
{
    qDebug() << "caso";

    bool existeUnDefault = false;

    leerLexema();

    if(token.compare(IDENTIFICADOR) != 0)
    {
        escribirError("Se esperaba la variable de \"caso\"");
    }
    else
    {
        Simbolo* temp = tablaDeSimbolos->buscarSimbolo(lexico);

        if(temp == NULL)
        {
            escribirError(lexico + " no esta definido");
        }
        else
        {
            if(temp->getTipo() != T_ENTERO)
            {
                escribirError("Para la variable de \"caso\" solo se admiten tipos enteros y se da un " + temp->getStringTipo());
            }
        }
    }

    leerLexema();

    if(lexico.compare("{") != 0)
        escribirError("Se esperaba apertura de bloque de \"caso\"");

    leerLexema();

    do
    {
        if(lexico.compare("defecto") == 0)
        {
            if(!existeUnDefault)
            {
                existeUnDefault = true;
            }
            else
            {
                escribirError("Multiplies etiquetas \"defecto\" en el \"caso\"");
            }

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
        escribirError("Se esperaba cierre de bloque de casos");
    }

    return true;
}

bool Compilador::regresa (void)
{
    qDebug() << "regresa";

    leerLexema();
    expr(true);

    Tipo retornoFuncion = tablaDeSimbolos->getTipoRetornoFuncion();
    Tipo retornado = tablaDeSimbolos->desapilarTipo();

    if(retornado != retornoFuncion)
    {
        escribirError("Conflicto en tipos en valor de retorno");
    }

    return true;
}

bool Compilador::lee(void)
{
    Simbolo* temp = NULL;
    qDebug() << "lee";

    leerLexema();

    if(lexico.compare(".") != 0)
        escribirError("Se esperaba accesor \".\"");

    leerLexema();

    if(lexico.compare("Lee") != 0)
        escribirError("metodo de \"con\" desconocido");

    leerLexema();

    if(lexico.compare("(") != 0)
        escribirError("Se esperaba apertura de parentesis");

    leerLexema();

    if(token.compare(IDENTIFICADOR) != 0)
    {
        escribirError("Se esperaba un identificador");
    }
    else
    {
        temp = tablaDeSimbolos->buscarSimbolo(lexico);
        if(temp == NULL)
        {
            escribirError(lexico + " no esta definido");
        }
        else
        {
            temp->setInicializado();
        }
    }

    leerLexema();
    dimension(temp, true);

    if(lexico.compare(")") != 0)
        escribirError("Se esperaba cierre de parentesis");

    leerLexema();

    if(temp != NULL)
    {
        ManejadorClass::ObtenerInstancia()->escribirLeerDato(temp);
    }

    return true;
}

bool Compilador::imprime(void)
{
    qDebug() << "imprime";
    bool conSalto = false;

    leerLexema();

    if(lexico.compare(".") != 0)
        escribirError("Se esperaba accesor \".\"");

    leerLexema();

    if(lexico.compare("Imprime") == 0)
    {
    }
    else if(lexico.compare("Imprimenl") == 0)
    {
        conSalto = true;
    }
    else
    {
        escribirError("metodo de \"fmt\" desconocido");
    }

    leerLexema();

    if(lexico.compare("(") != 0)
        escribirError("Se esperaba apertura de parentesis");

    ManejadorClass::ObtenerInstancia()->prepararImpresionPantalla();
    bool esConstante = false;
    do
    {
        leerLexema();
        expr(false);

        Simbolo* sActual = tablaDeSimbolos->desapilarValor();
        Tipo tActual = tablaDeSimbolos->desapilarTipo();
        stringstream imprimible;

        if(sActual->esTemporal())
        {
            esConstante = true;

            switch(tActual)
            {
                case T_ENTERO:
                    imprimible << sActual->getValor<int>();
                    break;
                case T_REAL:
                    imprimible << sActual->getValor<double>();
                    break;
                case T_BOOLEANO:

                    if(sActual->getValor<bool>())
                    {
                        imprimible << "\"Verdadero\"";
                    }
                    else
                    {
                        imprimible << "\"Falso\"";
                    }
                    break;
                case T_CADENA:
                    imprimible << (sActual->getValor<string>());
                    break;
                case T_CARACTER:
                    imprimible << sActual->getValor<char>();
                    break;
                default:
                    imprimible << "\"NULO\"";
                    break;
            }
        }

        ManejadorClass::ObtenerInstancia()->escribirImpresionPantalla(imprimible.str(), sActual, tActual, esConstante);

        if(sActual->esTemporal()) delete sActual;

        ManejadorClass::ObtenerInstancia()->prepararImpresionPantalla();

    }while(lexico.compare(",") == 0);

    if(conSalto)
    {
        ManejadorClass::ObtenerInstancia()->escribirImpresionLNPantalla("\"\"");
    }

    if(lexico.compare(")") != 0)
        escribirError("Se esperaba cierre de parentesis");

    leerLexema();
    return true;

}

bool Compilador::constanteTipo(string tok)
{
    return (tok.compare(REAL) == 0 ||
            tok.compare(ENTERO) == 0 ||
            tok.compare(HEXADECIMAL) == 0 ||
            tok.compare(OCTAL) == 0 ||
            tok.compare(CONST_LOGICA) == 0 ||
            tok.compare(CARACTER) == 0||
            tok.compare(ALFABETICO) == 0);
}

bool Compilador::constante(void)
{
    qDebug() << "constante";

    if(lexico.compare("const") != 0)
        return false;


    leerLexema();

    if(token.compare(IDENTIFICADOR) == 0)
    {
        Simbolo* simb = new Simbolo(lexico);

        leerLexema();

        if(token.compare(ASIGNACION) != 0)
            escribirError("Se esperaba asignacion despues de identificador");

        leerLexema();

        if(obtenerTipoValorConstante(simb))
        {
            tablaDeSimbolos->insertarSimbolo(simb);

            ManejadorClass::ObtenerInstancia()->escribirDeclararConstante(simb);
        }
        else
        {
            escribirError("Se esperaba un valor constante");
        }
    }
    else if(lexico.compare("(") == 0)
    {
        Simbolo* simb;
        do
        {
            leerLexema();
            if(token.compare(IDENTIFICADOR) != 0)
            {
                escribirError("Se esperaba identificador");
            }

            simb = new Simbolo(lexico);


            leerLexema();

            if(token.compare(ASIGNACION) != 0)
                escribirError("Se esperaba asignacion despues de identificador");

            leerLexema();

            if(obtenerTipoValorConstante(simb))
            {
                tablaDeSimbolos->insertarSimbolo(simb);

                ManejadorClass::ObtenerInstancia()->escribirDeclararConstante(simb);
            }
            else
            {
                escribirError("Se esperaba un valor constante");
            }

            leerLexema();
        }while(lexico.compare(",") == 0);

        if(lexico.compare(")") != 0)
            escribirError("Se esperaba cierre de parentesis en la definicion de constantes");
    }
    else
    {
        escribirError("Se esperaba identificador o apertura de parentesis despues de const");
    }


    return true;
}


void Compilador::expr(bool terminoOpcional)
{
    qDebug() << "expr";

    bool hayOperacion = false;
    bool primeraPasada = true;
    string actual;

    do
    {
        if(lexico.compare("||") == 0)
        {
            if(primeraPasada)
                escribirError("Se esperaba un termino antes del simbolo");

            actual = lexico;
            hayOperacion = true;
            leerLexema();
        }

        opy(terminoOpcional);
        primeraPasada = false;


        if(hayOperacion)
        {
           hayOperacion = false;

           Tipo derecho = tablaDeSimbolos->desapilarTipo();
           Tipo izquierdo = tablaDeSimbolos->desapilarTipo();

           if(derecho != T_BOOLEANO)
               escribirError("El valor a la derecha del || no es de tipo Logico");

           if(izquierdo != T_BOOLEANO)
               escribirError("El valor a la izquierda del || no es de tipo Logico");

           tablaDeSimbolos->apilarTipo(T_BOOLEANO);
        }

    }while(lexico.compare("||") == 0);
}

void Compilador::opy(bool terminoOpcional)
{
    qDebug() << "opy";

    string actual;
    bool hayOperacion = false;
    bool primeraPasada = true;

    do
    {
        if(lexico.compare("&&") == 0)
        {
            if(primeraPasada)
                escribirError("Se esperaba un termino antes del simbolo");

            actual = lexico;
            hayOperacion = true;
            leerLexema();
        }

        opno(terminoOpcional);
        primeraPasada = false;

        if(hayOperacion)
        {
           hayOperacion = false;

           Tipo derecho = tablaDeSimbolos->desapilarTipo();
           Tipo izquierdo = tablaDeSimbolos->desapilarTipo();

           if(derecho != T_BOOLEANO)
               escribirError("El valor a la derecha del && no es de tipo Logico");

           if(izquierdo != T_BOOLEANO)
               escribirError("El valor a la izquierda del && no es de tipo Logico");

           tablaDeSimbolos->apilarTipo(T_BOOLEANO);
        }

    }while(lexico.compare("&&") == 0);
}

void Compilador::opno(bool terminoOpcional)
{
    qDebug() << "opno";
    bool hayOperacion = false;
    string actual;

    if(lexico.compare("!") == 0)
    {
        actual = lexico;
        hayOperacion = true;
        leerLexema();
    }

    oprel(terminoOpcional);

    if(hayOperacion)
    {
       hayOperacion = false;

       Tipo derecho = tablaDeSimbolos->desapilarTipo();

       if(derecho != T_BOOLEANO)
           escribirError("El valor a la derecha del ! no es de tipo Logico");

       tablaDeSimbolos->apilarTipo(T_BOOLEANO);
    }
}

void Compilador::oprel(bool terminoOpcional)
{
    qDebug() << "oprel";

    string actual;
    bool hayOperacion = false;
    bool primeraPasada = true;

    do
    {
        if(token.compare(COMPARACION) == 0)
        {
            qDebug() << "Es comparacion";

            if(primeraPasada)
                escribirError("Se esperaba un termino antes del simbolo");

            actual = lexico;
            hayOperacion = true;
            leerLexema();
            terminoOpcional = false;
        }

        suma(terminoOpcional);
        primeraPasada = false;

        if(hayOperacion)
        {
            hayOperacion = false;

            Tipo derecho = tablaDeSimbolos->desapilarTipo();
            Tipo izquierdo = tablaDeSimbolos->desapilarTipo();

            //TODO: En operaciones del tipo == si se pueden admitir booleanos, validar eso

            if(derecho != T_ENTERO && derecho != T_REAL && derecho != T_CARACTER)
            {
                escribirError("El valor a la derecha del " + actual + " no es de tipo Entero, Real o Caracter");
            }

            if(izquierdo != T_ENTERO && izquierdo != T_REAL && izquierdo != T_CARACTER)
            {
                escribirError("El valor a la izquierda del " + actual + " no es de tipo Entero, Real o Caracter");
            }

            //las comparaciones dan de valor izquierdo un booleano
            tablaDeSimbolos->apilarTipo(T_BOOLEANO);
        }


    }while(token.compare(COMPARACION) == 0);
}

void Compilador::suma(bool terminoOpcional)
{
    qDebug() << "suma";

    string actual;
    bool hayOperacion = false;
    bool primeraPasada = true;

    do
    {
        if(lexico.compare("+") == 0 || lexico.compare("-") == 0)
        {
            if(primeraPasada)
                escribirError("Se esperaba un termino antes del simbolo");

            actual = lexico;
            hayOperacion = true;
            leerLexema();
        }

        multi(terminoOpcional);
        primeraPasada = false;

        if(hayOperacion)
        {
            hayOperacion = false;

            Tipo NuevoIzquierdo = T_INVALIDO;
            Tipo derecho = tablaDeSimbolos->desapilarTipo();
            Tipo izquierdo = tablaDeSimbolos->desapilarTipo();

            if(derecho != T_ENTERO && derecho != T_REAL && derecho != T_CARACTER && derecho != T_CADENA)
                escribirError("El valor a la derecha del " + actual + " no es de tipo Entero, Real, Alfabetico o Caracter");

            if(izquierdo != T_ENTERO && izquierdo != T_REAL && izquierdo != T_CARACTER && izquierdo != T_CADENA)
                escribirError("El valor a la izquierda del " + actual + " no es de tipo Entero, Real, Alfabetico o Caracter");


            //Obtener el tipo de valor izquierdo y apilarlo
            NuevoIzquierdo = operacionesSuma[derecho][izquierdo];
            tablaDeSimbolos->apilarTipo(NuevoIzquierdo);



            if(actual.compare("+") == 0)
                ManejadorClass::ObtenerInstancia()->escribirSuma(NuevoIzquierdo);
            else
                ManejadorClass::ObtenerInstancia()->escribirResta(NuevoIzquierdo);
        }

    }while(lexico.compare("+") == 0 || lexico.compare("-") == 0);
}

void Compilador::multi(bool terminoOpcional)
{
    qDebug() << "multi";

    string actual;
    bool hayOperacion = false;
    bool primeraPasada = true;

    do
    {
        if(lexico.compare("*") == 0 || lexico.compare("/") == 0 || lexico.compare("%") == 0)
        {
            if(primeraPasada)
                escribirError("Se esperaba un termino antes del simbolo");

            actual = lexico;
            hayOperacion = true;
            leerLexema();
        }

        expo(terminoOpcional);
        primeraPasada = false;

        if(hayOperacion)
        {
            hayOperacion = false;

            Tipo NuevoIzquierdo = T_INVALIDO;
            Tipo derecho = tablaDeSimbolos->desapilarTipo();
            Tipo izquierdo = tablaDeSimbolos->desapilarTipo();

            if(derecho != T_ENTERO && derecho != T_REAL)
                escribirError("El valor a la derecha del " + actual + " no es de tipo Entero o Real");

            if(izquierdo != T_ENTERO && izquierdo != T_REAL)
                escribirError("El valor a la izquierda del " + actual + " no es de tipo Entero o Real");


            //Obtener el tipo de valor izquierdo y apilarlo
            NuevoIzquierdo = operacionMulti[derecho][izquierdo];
            tablaDeSimbolos->apilarTipo(NuevoIzquierdo);
        }

    }while(lexico.compare("*") == 0 || lexico.compare("/") == 0 || lexico.compare("%") == 0);
}

void Compilador::expo (bool terminoOpcional)
{
    qDebug() << "expo";

    string actual;
    bool hayOperacion = false;
    bool primeraPasada = true;

    do
    {
        if(lexico.compare("^") == 0)
        {
            if(primeraPasada)
                escribirError("Se esperaba un termino antes del simbolo");

            actual = lexico;
            hayOperacion = true;
            leerLexema();
        }

        signo(terminoOpcional);
        primeraPasada = false;

        if(hayOperacion)
        {
            hayOperacion = false;

            Tipo NuevoIzquierdo = T_INVALIDO;
            Tipo derecho = tablaDeSimbolos->desapilarTipo();
            Tipo izquierdo = tablaDeSimbolos->desapilarTipo();

            if(derecho != T_ENTERO && derecho != T_REAL)
                escribirError("El valor a la derecha del " + actual + " no es de tipo Entero o Real");

            if(izquierdo != T_ENTERO && izquierdo != T_REAL)
                escribirError("El valor a la izquierda del " + actual + " no es de tipo Entero o Real");


            //Obtener el tipo de valor izquierdo y apilarlo
            NuevoIzquierdo = operacionPotencia[derecho][izquierdo];
            tablaDeSimbolos->apilarTipo(NuevoIzquierdo);
        }

    }while(lexico.compare("^") == 0);
}

void Compilador::signo (bool terminoOpcional)
{
    qDebug() << "signo";

    string actual;
    bool hayOperacion = false;
    bool invertirValor = false;

    if(lexico.compare("-") == 0)
    {
        actual = lexico;
        hayOperacion = true;
        invertirValor = true;
        leerLexema();
    }

    termino(terminoOpcional, invertirValor);

    if(hayOperacion)
    {
       hayOperacion = false;

       Tipo derecho = tablaDeSimbolos->desapilarTipo();

       if(derecho != T_ENTERO && derecho != T_REAL)
           escribirError("El valor a la derecha del - no es de tipo Entero o Real");

       //su tipo no cambia, se reapila
       tablaDeSimbolos->apilarTipo(derecho);
    }
}

void Compilador::termino (bool terminoOpcional, bool invertirValor)
{
    qDebug() << "termino";
    if(constanteTipo(token))
    {
        //Llegó un dato constante, determinar su tipo, valor y apilarlos

        Simbolo* almacenadoTemporal = new Simbolo("HOLDER");
        almacenadoTemporal->setTemporal();

        if(!obtenerTipoValorConstante(almacenadoTemporal))
        {
            escribirError("Tu compu no sirve #posMeQuejo");
        }
        else if(invertirValor)
        {
            switch(almacenadoTemporal->getTipo())
            {
            case T_ENTERO:
                almacenadoTemporal->setValor( -1 * (almacenadoTemporal->getValor<int>() ));
                break;
            case T_REAL:
                almacenadoTemporal->setValor(-1 * (almacenadoTemporal->getValor<double>() ));
                break;
            default:
                escribirError("Aplicando valor inverso a tipo de dato no compatible");
                break;
            }
        }


        tablaDeSimbolos->apilarValor(almacenadoTemporal);

        if(!evaluarDespues)
        {
            //cargar valor en pila del jvm
            ManejadorClass::ObtenerInstancia()->escribirValorConstante(almacenadoTemporal);
        }

        tablaDeSimbolos->apilarTipo(almacenadoTemporal->getTipo());

        leerLexema();

        return;
    }
    else if(token.compare(IDENTIFICADOR) == 0)
    {
        //TODO, invertir valores de las variables


        Simbolo* temp = tablaDeSimbolos->buscarSimbolo(lexico);


        //Como se estan usando datos variables, la expresion no
        //puede ser evaluada en tiempo de compilacion
        //Abandonado este aproach
        //tablaDeSimbolos->noEsEvaluable();

        //Se apilan los simbolos asociados
        //para mantener coherencia entre las pilas de Valor y de Tipo

        if(temp == NULL)
        {
            escribirError(lexico + " no esta definido");
        }
        else
        {
            if(temp->getTipo() != T_FUNCION)
            {
                //Llego una variable definida que no es funcion
                //apilar su tipo de dato


                if(!temp->estaInicializado())
                {
                    escribirError("La variable \"" + temp->getIdentificador() + "\" no esta inicializada");
                }

                tablaDeSimbolos->apilarValor(temp);
                tablaDeSimbolos->apilarTipo(temp->getTipo());

            }
            else //pues quesque es funcion
            {

                Tipo retorno = temp->getTipoRetorno();

                if(retorno != T_INVALIDO)
                {
                    tablaDeSimbolos->apilarValor(temp);
                }
                else
                {
                    escribirError("La funcion no retorna valor");
                }

                //Apilar el tipo de retorno de la funcion
                tablaDeSimbolos->apilarTipo(retorno);
            }
        }

        leerLexema();
        if(lexico.compare("(") == 0)
        {
            if(temp != NULL && temp->getTipo() != T_FUNCION)
            {
                escribirError("Identificador \"" + temp->getIdentificador() + "\"no está definido como funcion, pero se usa como una");
            }

            string paramsObtenidos = lFunc_2();
            string paramsEsperados = (temp != NULL) ? temp->getFirmaFuncion() : "";

            if(paramsEsperados.compare(paramsObtenidos) != 0)
            {
                escribirError("Error de parametros, la funcion espera " + obtenerStringFirma(paramsEsperados) +
                              " y se está enviando " + obtenerStringFirma(paramsObtenidos));
            }

            ManejadorClass::ObtenerInstancia()->escribirLlamadaFuncion(temp);

            return;
        }
        else
        {
            if(temp != NULL && temp->getTipo() == T_FUNCION)
            {
                escribirError("Identificador \"" + temp->getIdentificador() + "\" es una funcion, pero es usada como variable");
            }

            if(!evaluarDespues)
            {
                //verificar que cumpla con su especificacion de dimension
                dimension(temp, true);


                //poner identificador para obtener su valor asociado
                ManejadorClass::ObtenerInstancia()->escribirLlamadaVariable(temp, false);
            }

            return;
        }
    }
    else if(lexico.compare("(") == 0)
    {
        leerLexema();
        expr(false);

        if(lexico.compare(")") != 0)
            escribirError("Se esperaba ) para cerrar la expresion");

        leerLexema();

        return;
    }
    else if(!terminoOpcional)
    {
        escribirError("Se esperaba un término");
        return;
    }


    tablaDeSimbolos->apilarTipo(T_INVALIDO); //dummy, si no hay terminos, se apila un invalido
}

bool Compilador::obtenerTipoValorConstante(Simbolo* simb)
{
    bool exitoParsing = true;

    if(token.compare(REAL) == 0)
    {
        double valor;

        Conversor::cadena2Real(valor, lexico.c_str());
        simb->setValor(valor)->setConstante()->setTipo(T_REAL);

    }
    else if(token.compare(ENTERO) == 0 ||
            token.compare(HEXADECIMAL) == 0 ||
            token.compare(OCTAL) == 0)
    {
        int valor;

        Conversor::cadena2Entero(valor, lexico.c_str());
        simb->setValor(valor)->setConstante()->setTipo(T_ENTERO);
    }
    else if(token.compare(CONST_LOGICA) == 0)
    {
        bool valor;

        Conversor::cadena2Booleano(valor, lexico.c_str());
        simb->setValor(valor)->setConstante()->setTipo(T_BOOLEANO);
    }
    else if(token.compare(ALFABETICO) == 0)
    {
        simb->setValor(lexico)->setConstante()->setTipo(T_CADENA);
    }
    else if(token.compare(CARACTER) == 0)
    {
        simb->setValor(lexico[1])->setConstante()->setTipo(T_CARACTER);
    }
    else
    {
        exitoParsing = false;
    }

    return exitoParsing;
}

string Compilador::obtenerStringFirma(const string& firma)
{
    stringstream flujo;
    int longitud = firma.length();

    for(int i = 0; i < longitud; ++i)
    {
        switch(firma[i])
        {
        case '0':
            flujo << "Entero";
            break;
        case '1':
            flujo << "Real";
            break;
        case '2':
            flujo << "Caracter";
            break;
        case '3':
            flujo << "Alfabetico";
            break;
        case '4':
            flujo << "Logico";
            break;
        default:
            flujo << "Void";
            break;
        }

        if(i != longitud -1) flujo << "-";
    }

    return flujo.str();
}
