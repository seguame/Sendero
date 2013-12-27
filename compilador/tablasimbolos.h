#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <map>  //cambiar a unordered_map para eficiencia una vez se determine
                //que podemos compilar en windows tambien

#include "enumerators.h"
#include "simbolo.h"
#include "utils/reportadorerrores.h"
#include "compilador.h"

#include <QDebug>

using namespace std;

class Compilador;

class TablaSimbolos
{
public:
    TablaSimbolos(Compilador* c);
    ~TablaSimbolos();

    bool insertarSimbolo(Simbolo* x);
    Simbolo* buscarSimbolo(string x) const;
    bool removerSimbolo(string x);
    void nuevoScope(void);
    void borrarScope(void);
    void purgarTabla(void);


    void prepararPilas(void);
    Simbolo* apilarSimbolo(string identificador, bool estaInicializado);
    void apilarValor(Simbolo* s);
    Simbolo* desapilarValor(void);
    string almacenarPilaSimbolos(Tipo tipo);
    void apilarTipo(Tipo tipo);
    Tipo desapilarTipo(void);
    void noEsEvaluable(void); //marca en caso de que se esté en asignacion, que no todos los valores son constantes
    void checarValidezDeOperaciones(void);
    void purgarPilas(void);

    void entrarContextoFuncion(Simbolo* funcion);
    void setFirmaFuncion(const string& s);
    void setTipoRetornoFuncion(Tipo t);
    Tipo getTipoRetornoFuncion(void);
    void salirContextoFuncion(void);


private:
    bool existeSimbolo(Simbolo* buscable, map <string, Simbolo*>* temp) const;

private:
    Compilador* refCompilador;
    vector< map <string, Simbolo*>* > *simbolos;
    stack<Simbolo*> *pilaSimbolos;
    stack<Tipo> *pilaTipos;
    stack<Simbolo*> *pilaValores;

    Simbolo* contextoFuncion;
    bool retornoValidado;
};

#endif // TABLASIMBOLOS_H
