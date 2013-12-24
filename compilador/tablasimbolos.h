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


    void prepararPila(void);
    void apilarSimbolo(string s);
    void almacenarPila(Tipo tipo);
    void purgarPila(void);


private:
    bool existeSimbolo(Simbolo* buscable, map <string, Simbolo*>* temp) const;

private:
    Compilador* refCompilador;
    vector< map <string, Simbolo*>* > *simbolos;
    stack<Simbolo*> *pila;
};

#endif // TABLASIMBOLOS_H
