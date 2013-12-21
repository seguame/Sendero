#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#include <vector>
#include <string>
#include <map>  //cambiar a unordered_map para eficiencia una vez se determine
                //que podemos compilar en windows tambien

#include "enumerators.h"
#include "simbolo.h"

using namespace std;

class TablaSimbolos
{
public:
    TablaSimbolos();
    ~TablaSimbolos();

    bool insertarSimbolo(Simbolo x);
    Simbolo buscarSimbolo(string x);
    bool removerSimbolo(string x);
    void nuevoScope();
    void borrarScope();
    void purgarTabla();

private:
    vector< map <string, Simbolo> > *simbolos;
};

#endif // TABLASIMBOLOS_H
