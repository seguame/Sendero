#include "tablasimbolos.h"

TablaSimbolos::TablaSimbolos()
{
    simbolos = new vector< map <string, Simbolo> >();
}

TablaSimbolos::~TablaSimbolos()
{
    delete simbolos;
}


void TablanuevoScope();
void borrarScope();
void purgarTabla();
