#include "manejador_class.h"

ManejadorClass* ManejadorClass::instancia = NULL;

ManejadorClass::ManejadorClass(){}

void ManejadorClass::Inicializar(void)
{
    Terminar();

    instancia = new ManejadorClass();
}

ManejadorClass* ManejadorClass::ObtenerInstancia(void)
{
    if(instancia == NULL)
    {
        instancia = new ManejadorClass();
    }

    return instancia;
}

void ManejadorClass::Terminar(void)
{
    if(instancia != NULL)
    {
        delete instancia;
        instancia = NULL;
    }
}


void ManejadorClass::escribirCabeceraClase(const string& nombre)
{
    string operando = ".class public";
    string operador = nombre;

    aniadirInstruccion(operando, operador);

    operando = ".super";
    operador = "java/lang/Object";
    aniadirInstruccion(operando, operador);
}

void ManejadorClass::escribirMain(void)
{
    string metodo  = "main";
    string firma   = "9";

    escribirCabeceraMetodo(metodo, firma, T_INVALIDO);
}

void ManejadorClass::escribirCabeceraMetodo(Simbolo* funcion)
{
    if(funcion->getTipo() == T_FUNCION)
    {
        if(funcion->getIdentificador().compare("principal") == 0)
        {
            escribirMain();
        }
        else
        {
            escribirCabeceraMetodo(funcion->getIdentificador(), funcion->getFirmaFuncion(), funcion->getTipoRetorno());
        }
    }
    else
    {
        qDebug() << "Intentando escribir cabecera de metodo con un simbolo que no es funcion";
    }
}

void ManejadorClass::escribirCabeceraMetodo(const string& nombre, const string& firma, Tipo retorno)
{
    stringstream metodo;
    string firmaCompleta;

    metodo << ".method public static ";
    metodo << nombre;
    metodo << " :";

    firmaCompleta = obtenerDescriptorFirma(firma, retorno);

    aniadirInstruccion(metodo.str(), firmaCompleta);
    aniadirInstruccion(".limit stack", "10");
    aniadirInstruccion(".limit locals", "10");
}

void ManejadorClass::escribirFinMetodo(Simbolo* funcion)
{
    string retorno;

    retorno = obtenerTipoRetorno(funcion->getTipoRetorno());

    aniadirInstruccion(retorno, "");
    aniadirInstruccion(".end", "method");
}

void ManejadorClass::aniadirInstruccion(const string& operacion,const string& parametro)
{
    instrucciones.push_back(make_pair(operacion, parametro));
}


void ManejadorClass::escribirArchivoParaEnsamblar(const string& ruta)
{
    archivo.open((ruta+".bytecode").c_str());

    if(!archivo.is_open())
    {
        qDebug() << "No se pudo abrir el archivo de bytecode";
        return;
    }

    for(vector<pair<string,string> >::size_type i = 0; i != instrucciones.size(); ++i)
    {
        archivo << instrucciones[i].first << " " << instrucciones[i].second << std::endl;
    }

    qDebug() << "Archivo bytecode escrito";
}

int ManejadorClass::Ensamblar(const string& ruta, const string& archivo)
{
    stringstream comando;
    stringstream comando2;

    comando << "python ";
    comando << "\"/home/seguame/Documentos/Taller Compiladores/EditorSendero/compilador/Krakatau/assemble.py\" \"";
    comando << ruta;
    comando << "/";
    comando << archivo;
    comando << ".bytecode\"";

    qDebug() << comando.str().c_str();

    system(comando.str().c_str());

    comando2 << "mv ";
    comando2 << "\"/home/seguame/Documentos/Taller Compiladores/EditorSendero/compilador/Krakatau/";
    comando2 << archivo;
    comando2 << ".class\" \"";
    comando2 << ruta;
    comando2 << "/";
    comando2 << archivo;
    comando2 << ".class\"";

    qDebug() << comando2.str().c_str();

    system(comando2.str().c_str());


    return system(("java " + archivo).c_str());
}

string ManejadorClass::obtenerDescriptorFirma(const string& firma, Tipo retorno)
{
    stringstream flujo;
    int longitud = firma.length();

    flujo << '(';

    for(int i = 0; i < longitud; ++i)
    {
        switch(firma[i])
        {
        case '0':
            flujo << 'I';
            break;
        case '1':
            flujo << 'D';
            break;
        case '2':
            flujo << 'C';
            break;
        case '3':
            flujo << "Ljava/lang/String";
            break;
        case '4':
            flujo << 'Z';
            break;
        case '6':
            flujo << 'V';
            break;
        case '9': //especial
            flujo << "[Ljava/lang/String";
            break;
        default:
            flujo << "ERROR_SIMBOLO_NO_ADMITIDO";
            break;
        }

        flujo << ";";
    }

    flujo << ')';

    switch(retorno)
    {
    case T_ENTERO:
        flujo << 'I';
        break;
    case T_REAL:
        flujo << 'D';
        break;
    case T_CARACTER:
        flujo << 'C';
        break;
    case T_CADENA:
        flujo << "Ljava/lang/String";
        break;
    case T_BOOLEANO:
        flujo << 'Z';
        break;
    case T_INVALIDO:
        flujo << 'V';
        break;
    default:
        flujo << "ERROR_SIMBOLO_NO_ADMITIDO";
        break;
    }


    return flujo.str();
}

string ManejadorClass::obtenerTipoRetorno(Tipo retorno)
{
    switch(retorno)
    {
    case T_CARACTER:
    case T_ENTERO:
        return "    ireturn";
    case T_REAL:
        return "    dreturn";
    default:
        return "    return";
    }
}
