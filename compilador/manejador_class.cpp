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
    aniadirInstruccion(".class public", nombre);
    aniadirInstruccion(".super", "java/lang/Object");

    //se añade una instancia estatica del Scanner de java
    aniadirInstruccion(".field static public", "lector Ljava/util/Scanner;");

    //Se añade un constructor default vacio
    aniadirInstruccion(".method public <init> :", "()V");
    aniadirInstruccion("    .limit stack", "1");
    aniadirInstruccion("    .limit locals", "1");
    aniadirInstruccion("    aload_0", "");
    aniadirInstruccion("    invokespecial", "java/lang/Object <init> ()V");
    aniadirInstruccion("    return", "");
    aniadirInstruccion(".end", "method");

    //se añade el constructor estatico para instancia el Scanner
    aniadirInstruccion(".method static <clinit> :", "()V");
    aniadirInstruccion("    .limit stack", "3");
    aniadirInstruccion("    .limit locals", "0");
    aniadirInstruccion("    new", "java/util/Scanner");
    aniadirInstruccion("    dup", "");
    aniadirInstruccion("    getstatic", "java/lang/System in Ljava/io/InputStream;");
    aniadirInstruccion("    invokespecial", "java/util/Scanner <init> (Ljava/io/InputStream;)V");
    aniadirInstruccion("    putstatic", nombre + " lector Ljava/util/Scanner;");
    aniadirInstruccion("    return", "");
    aniadirInstruccion(".end", "method");
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
    aniadirInstruccion(".limit stack", "40");
    aniadirInstruccion(".limit locals", "100");
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

void ManejadorClass::escribirImpresionLNPantalla(const string& texto)
{
    aniadirInstruccion("    getstatic", "java/lang/System out Ljava/io/PrintStream;");
    aniadirInstruccion("    ldc", texto);
    aniadirInstruccion("    invokevirtual", "java/io/PrintStream println (Ljava/lang/Object;)V");
}

void ManejadorClass::escribirImpresionPantalla(const string& texto)
{
    aniadirInstruccion("    getstatic", "java/lang/System out Ljava/io/PrintStream;");
    aniadirInstruccion("    ldc", texto);
    aniadirInstruccion("    invokevirtual", "java/io/PrintStream print (Ljava/lang/Object;)V");
}

void ManejadorClass::escribirDeclararVariableGlobal(Simbolo* simbolo)
{

}

void ManejadorClass::escribirDeclararConstante(Simbolo* simbolo)
{
    stringstream operando;
    operando << simbolo->getIdentificador();
    operando << " " << obtenerTipo(simbolo->getTipo());
    operando << " = ";
    switch(simbolo->getTipo())
    {
        case T_ENTERO:
            operando << simbolo->getValor<long>();
            break;
        case T_REAL:
            operando << simbolo->getValor<double>();
            break;
        case T_BOOLEANO:
            operando << simbolo->getValor<bool>();
            break;
        case T_CADENA:
            operando << (simbolo->getValor<string>());
            break;
        case T_CARACTER:
            operando << simbolo->getValor<char>();
            break;
        default:
            operando << "ERROR VALOR CONSTANTE";
            break;
    }


    aniadirInstruccion(".field static public", operando.str());
}

void ManejadorClass::escribirEnteroConstante(long i)
{
    if(i == -1)
        aniadirInstruccion("    iconst_m1", "");
    else
    {
        stringstream operador;
        stringstream operando;

        operador << "    ";
        operando << "";

        if(i >= 0 && i <= 5)
        {
            operador << "iconst_";
            operador << i;
        }
        else if(i >= -128 && i <= 127)
        {
            operador << "bipush";
            operando << i;
        }
        else if(i >= -32768 && i <= 32767)
        {
            operador << "sipush";
            operando << i;
        }
        else
        {
            operador << "ERRORENTERO";
            qDebug() << "Valor entero muy grande, no implementado";
        }

        aniadirInstruccion(operador.str(), operando.str());
    }

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
        flujo << obtenerTipo(firma[i]);
        if(firma[i] == '3' || firma[i] == '9') flujo << ";";
    }

    flujo << ')';

    flujo << obtenerTipo(retorno);

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

string ManejadorClass::obtenerTipo(char t)
{
    switch(t)
    {
    case '0': return "I";
    case '1': return "D";
    case '2': return "C";
    case '3': return "Ljava/lang/String;";
    case '4': return "Z";
    case '6': return "V";
    case '9': return "[Ljava/lang/String;";
    default:  return "ERROR_SIMBOLO_NO_ADMITIDO";
    }
}

string ManejadorClass::obtenerTipo(Tipo t)
{
    switch(t)
    {
    case T_ENTERO: return "I";
    case T_REAL: return "D";
    case T_CARACTER: return "C";
    case T_CADENA: return "Ljava/lang/String;";
    case T_BOOLEANO: return "Z";
    case T_INVALIDO: return "V";
    default: return "ERROR_SIMBOLO_NO_ADMITIDO";
    }
}
