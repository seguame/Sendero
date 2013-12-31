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
    void escribirCabeceraClase(void);
    void escribirCabeceraMetodo(Simbolo* funcion);
    void escribirConstructorEstatico(void);
    void escribirFinMetodo(Simbolo* funcion);

    void prepararImpresionPantalla(void);
    void escribirImpresionLNPantalla(const string& texto);
    void escribirImpresionPantalla(const string& texto);

    void registrarVariableLocal(Simbolo* simbolo, int profundidad);
    void deregistrarVariableLocal(Simbolo* simbolo);
    void escribirDeclararVariableGlobal(Simbolo* simbolo);
    void escribirDeclararConstante(Simbolo* simbolo);

    void escribirLlamadaFuncion(Simbolo* simbolo);
    void escribirValorConstante(Simbolo* simbolo);
    void escribirSuma(Tipo t);
    void escribirResta(Tipo t);
    void escribirEnteroConstante(int i);

    void aniadirInstruccion(const string& operacion,const string& parametro);

    void escribirArchivoParaEnsamblar(const string& ruta);

    void setNombreClase(const string& nombre);

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

    string obtenerTipo(char t);
    string obtenerTipo(Tipo t);

    static ManejadorClass* instancia;

    ofstream archivo;
    string nombreClase;
    vector< pair<string, string> > instrucciones;
    vector< Simbolo* > globales;
    map <string, pair<int, Simbolo*> > locales;
};

#endif // MANEJADOR_CLASS_H
