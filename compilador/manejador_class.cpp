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


void ManejadorClass::escribirCabecera(const string& nombre)
{
    string operando = ".class public";
    string operador = nombre;

    aniadirInstruccion(operando, operador);

    operando = ".super";
    operador = "java/lang/Object";
    aniadirInstruccion(operando, operador);
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

    comando2 << "cp ";
    comando2 << "\"/home/seguame/Documentos/Taller Compiladores/EditorSendero/compilador/Krakatau/";
    comando2 << archivo;
    comando2 << ".class\" ";
    comando2 << ruta;
    comando2 << "/";
    comando2 << archivo;
    comando2 << ".class\"";

    system(comando2.str().c_str());


    return system("xterm");
}
