#ifndef MANEJADOR_CLASS_H
#define MANEJADOR_CLASS_H

#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "simbolo.h"
#include "etiqueta.h"

#include <QDebug>

using namespace std;

class ManejadorClass
{
public:
    void resetearContadorLocalidades(void);
    void escribirCabeceraClase(void);
    void escribirCabeceraMetodo(Simbolo* funcion);
    void escribirConstructorEstatico(void);
    void escribirFinMetodo(Simbolo* funcion);

    void prepararImpresionPantalla(void);
    void escribirImpresionLNPantalla(const string& texto);
    void escribirImpresionPantalla(Simbolo* s, Tipo t);

    void escribirLeerDato(Simbolo* simbolo);

    void registrarVariableLocal(Simbolo* simbolo, int profundidad);
    void deregistrarVariableLocal(Simbolo* simbolo);
    void escribirDeclararVariableGlobal(Simbolo* simbolo);
    void escribirDeclararConstante(Simbolo* simbolo);
    void escribirDeclararArray(Simbolo* simbolo, bool global);

    void escribirEtiqueta(const Etiqueta& etq);
    void escribirSaltoEtiqueta(const Etiqueta& etq);

    void inicializarCaso(void);
    void aniadirCaso(Simbolo* simb,const Etiqueta& etq);
    void escribirCaso(void);

    void escribirLlamadaFuncion(Simbolo* simbolo);
    void escribirLlamadaVarGlobal(Simbolo* simbolo, bool almacenar);
    void escribirLlamadaVariable(Simbolo* simbolo, bool almacenar);

    void escribirValorConstante(Simbolo* simbolo);
    void escribirGuardarEnReferencia(Tipo t);
    void escribirCargarReferencia(Tipo t);
    void escribirSuma(Tipo t);
    void escribirResta(Tipo t);
    void escribirMultiplicacion(Tipo t);
    void escribirDivision(Tipo t);
    void escribirModulo(Tipo t);
    void escribirPotencia(void);
    void escribirInvertirValor(Tipo t);
    void escribirComparacion(string operando, const Etiqueta& etiqueta);
    void escribirEnteroConstante(int i);
    void escribirRealConstante(double d);
    void escribirStringConstante(const string& s);
    void escribirCastingDouble(void);


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
    void escribirLlamadaVariableEntero(int posStack, bool almacenar);
    void escribirLlamadaVariableReal(int posStack, bool almacenar);
    void escribirLlamadaVariableAlfabetico(int posStack, bool almacenar);

    string obtenerDescriptorFirma(const string& firma, Tipo retorno);
    string obtenerTipoRetorno(Tipo retorno);

    string obtenerTipo(char t);
    string obtenerTipo(Tipo t);

    int obtenerPosLocal(const string& alias);

    static ManejadorClass* instancia;

    ofstream archivo;
    string nombreClase;
    vector< pair<string, string> > instrucciones;
    vector< Simbolo* > globales;
    map <string, pair<int, Simbolo*> > locales;
    unsigned int localidadesActuales;

    vector<pair<string, string> > casos;
};

#endif // MANEJADOR_CLASS_H
