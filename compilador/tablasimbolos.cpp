#include "tablasimbolos.h"

TablaSimbolos::TablaSimbolos()
{
    qDebug() << "Creando tabla de simbolos";
    simbolos = new vector< map <string, Simbolo*>* >();
}

TablaSimbolos::~TablaSimbolos()
{
    qDebug() << "Borrando Tabla de simbolos";
    delete simbolos;
}

/**
 * @brief TablaSimbolos::insertarSimbolo
 *        Se mete el simbolo en el nivel que se tenga actualmente, siendo siempre
 *        el nivel 0 el scope global
 * @param x, el simbolo a meter
 * @return si se insertó con exito, esto falla si el simbolo ya existia
 */
bool TablaSimbolos::insertarSimbolo(Simbolo* x)
{
    qDebug() << "Insertando simbolo en tabla: " << x->getIdentificador().c_str();
    map <string, Simbolo*>* temp = simbolos->back();

    if(!existeSimbolo(x->getIdentificador(), temp))
    {
        pair<string, Simbolo*> par(x->getIdentificador(), x);
        temp->insert(par);
        return true;
    }

    return false;
}

/**
 * @brief TablaSimbolos::buscarSimbolo
 *        Se busca el valor del simbolo desde el scope mas profundo hasta el global
 * @param x, identificador a buscar
 * @return NULL si no está definido, el simbolo en caso contrario
 */
Simbolo* TablaSimbolos::buscarSimbolo(string x)
{
    qDebug() << "Buscando simbolo " << x.c_str() << "en la tabla";
    for(vector<map<string, Simbolo*>*>::reverse_iterator mapa = simbolos->rbegin(); mapa != simbolos->rend(); ++mapa)
    {
        map<string, Simbolo*>* m = *mapa;
        map<string, Simbolo*>::const_iterator cit = m->find(x);

        if(cit != m->end())
        {
            pair<string, Simbolo*> par = *cit;
            return par.second;
        }
    }

    return NULL;
}

/**
 * @brief TablaSimbolos::removerSimbolo
 *        Eliminar el simbolo del scope actual
 * @param x, identificador a eliminar
 * @return true si se elimino con exito
 */
bool TablaSimbolos::removerSimbolo(string x)
{
    qDebug() << "Borrando simbolo del scope actual";
    map<string, Simbolo*>* mapa = simbolos->back();

    //TODO: Eliminar referencia de simbolo

    return mapa->erase(x) == 1;
}

/**
 * @brief TablaSimbolos::nuevoScope
 *        Crea un nuevo nivel de scope
 */
void TablaSimbolos::nuevoScope(void)
{
    qDebug() << "Creando nuevo scope";
    simbolos->push_back(new map <string, Simbolo*>());
}

/**
 * @brief TablaSimbolos::borrarScope
 *        El scope actual mas bajo es eliminado
 */
void TablaSimbolos::borrarScope(void)
{
    qDebug() << "Borrando scope, esto puede dar un bug";

    map <string, Simbolo*>* mapa = simbolos->back();

    for(map <string, Simbolo*>::iterator it = mapa->begin(); it != mapa->end(); )
    {
        //borrando los simbolos
        pair<string, Simbolo*> par = *it;
        delete par.second;
        mapa->erase(it++);
    }
    delete simbolos->back();
    simbolos->erase(simbolos->begin() + (simbolos->size() - 1));
}

/**
 * @brief TablaSimbolos::purgarTabla
 *        Reinicia todo
 */
void TablaSimbolos::purgarTabla(void)
{
    qDebug() << "Purgando tabla";
    simbolos->clear();
}

bool TablaSimbolos::existeSimbolo(const string identificador, map <string, Simbolo*>* temp)
{
    qDebug() << "Checando existencia de simbolo";
    return temp->find(identificador) != temp->end();
}


void TablaSimbolos::prepararPila(void)
{
    qDebug() << "Preparando apilamiento";
    delete pila;
    pila = new stack<Simbolo*>();
}

void TablaSimbolos::apilarSimbolo(string s)
{
    qDebug() << "Apilando " << s.c_str();
    pila->push(new Simbolo(s));
}

void TablaSimbolos::almacenarPila(Tipo tipo)
{
    qDebug() << "Almacenando contenido de la pila";
    while(!pila->empty())
    {
        if(!this->insertarSimbolo(pila->top()->setTipo(tipo)))
        {
            //TODO: Añadir reporte de este error
            qDebug() << pila->top()->getIdentificador().c_str() << " ya está definido";
        }
        pila->pop();
    }
}

void TablaSimbolos::purgarPila(void)
{
    qDebug() << "Purgando la pila";
    while(!pila->empty())
    {
        delete pila->top();
        pila->pop();
    }
}
