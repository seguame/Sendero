#ifndef MANEJADOR_CLASS_H
#define MANEJADOR_CLASS_H

#include <fstream>
#include <sstream>
#include <vector>

#include "simbolo.h"

#include <QDebug>

using namespace std;

class ManejadorClass
{
public:
    void escribirCabeceraClase(const string& nombre);
    void escribirCabeceraMetodo(Simbolo* funcion);
    void escribirFinMetodo(Simbolo* funcion);

    void aniadirInstruccion(const string& operacion,const string& parametro);
    void escribirArchivoParaEnsamblar(const string& ruta);

    static void Inicializar(void);
    static ManejadorClass* ObtenerInstancia(void);
    static void Terminar(void);
    static int Ensamblar(const string& ruta, const string& archivo);

private:
    ManejadorClass();

    void escribirCabeceraMetodo(const string& nombre, const string& firma, Tipo retorno);
    void escribirMain(void);

    string obtenerDescriptorFirma(const string& firma, Tipo retorno);
    string obtenerTipoRetorno(Tipo retorno);

    static ManejadorClass* instancia;

    ofstream archivo;
    vector< pair<string, string> > instrucciones;
};

#endif // MANEJADOR_CLASS_H
