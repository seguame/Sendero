#include "tablasimbolos.h"
#include "manejador_class.h"

TablaSimbolos::TablaSimbolos(Compilador* c):
    pilaSimbolos(NULL),
    pilaValores(NULL),
    pilaTipos(NULL),
    contextoFuncion(NULL)
{
    qDebug() << "Creando tabla de simbolos";
    simbolos = new vector< map <string, Simbolo*>* >();
    refCompilador = c;

    prepararPilas();
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

    if(!existeSimbolo(x, temp))
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
Simbolo* TablaSimbolos::buscarSimbolo(string x) const
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

        std::stringstream nivel;
        nivel << simbolos->size();
        qDebug() << "Nivel " << nivel.str().c_str() << " " << par.second->toString().c_str();

        if(simbolos->size() != 1)
        {
            //quitar de la tabla de locales
            ManejadorClass::ObtenerInstancia()->deregistrarVariableLocal(par.second);
        }

        delete par.second;
        mapa->erase(it++);
    }
    delete simbolos->back();
    simbolos->erase(simbolos->begin() + (simbolos->size() - 1));
}

/**
 * @brief TablaSimbolos::purgarTabla
 *        Elimina referencia de los simbolos, pero no libera memoria
 */
void TablaSimbolos::purgarTabla(void)
{
    qDebug() << "Purgando tabla";
    simbolos->clear();
}


/**
 * @brief TablaSimbolos::existeSimbolo busca en la tabla si existe el simbolo indicado
 * @param buscable
 * @param temp, mapa donde hacer la busqueda
 * @return true si encontrado, falso en contra.
 */
bool TablaSimbolos::existeSimbolo(Simbolo* buscable, map <string, Simbolo*>* temp) const
{
    qDebug() << "Checando existencia de simbolo";

    map<string, Simbolo*>::const_iterator otro = temp->find(buscable->getIdentificador());

    if(otro != temp->end())
    {
        pair<string, Simbolo*> par = *otro;
        string error = buscable->getIdentificador() + " ya fue definido como ";

        if(par.second->getTipo() != T_FUNCION)
        {
            if(par.second->esConstante())
            {
                error += "constante de tipo " + par.second->getStringTipo();
            }
            else
            {
                error += par.second->getStringTipo() + " y aqui se declara como " + buscable->getStringTipo();
            }

            refCompilador->escribirError(error);
        }


        return true;
    }

    return false;
}

/**
 * @brief TablaSimbolos::prepararPilas
 *        Inicializa el contenido de las pilas, si contenian algo
 *        son purgadas hasta quedar vacias
 */
void TablaSimbolos::prepararPilas(void)
{
    qDebug() << "Preparando apilamiento";
    if(pilaSimbolos == NULL)
    {
        pilaSimbolos = new stack<Simbolo*>();
    }

    if(pilaTipos == NULL)
    {
        pilaTipos = new stack<Tipo>();
    }

    if(pilaValores == NULL)
    {
        pilaValores = new stack<Simbolo*>();
    }

    purgarPilas();
}

/**
 * @brief TablaSimbolos::apilarSimbolo
 *          Almacena temporalmente un nuevo simbolo como candidato a la tabla de simbolos
 *          con el identificador dado
 * @param identificador
 * @param estaInicializado
 * @return Referencia al simbolo generado
 */
Simbolo* TablaSimbolos::apilarSimbolo(string identificador, bool estaInicializado)
{
    qDebug() << "Apilando " << identificador.c_str();

    Simbolo* s = new Simbolo(identificador);

    if(estaInicializado)
    {
        s->setInicializado();
    }

    pilaSimbolos->push(s);

    return s;
}

/**
 * @brief TablaSimbolos::apilarValor
 *          Apila temporalmente un simbolo existente para su posterior evaluacion
 *          en expresiones
 * @param s
 */
void TablaSimbolos::apilarValor(Simbolo* s)
{
    if(s == NULL)
    {
        qDebug() << "Intentando apilar un simbolo nulificado";
    }

    pilaValores->push(s);
}


/**
 * @brief TablaSimbolos::desapilarValor
 *          Extrae el simbolo en el tope de la pila de valores
 * @return Simbolo extraido
 */
Simbolo* TablaSimbolos::desapilarValor(void)
{
    if(pilaValores->empty())
    {
        qDebug() << "Desapilando valor de una pila vacia";
    }
    else
    {
        Simbolo* s = pilaValores->top();
        pilaValores->pop();
        return s;
    }

    return NULL;
}

/**
 * @brief TablaSimbolos::almacenarPilaSimbolos
 *          Se intentan guardar todos los simbolos apilados, aqui se chequea que no existan
 *          si alguno ya está en la tabla es ignorado y reportado como erroneo
 * @param tipo, Tipo de simbolo a almacenar
 * @param esGlobal
 * @return Un string que contiene una firma de los tipos almacenados
 */
string TablaSimbolos::almacenarPilaSimbolos(Tipo tipo, bool esGlobal)
{
    stringstream regresable;
    qDebug() << "Almacenando contenido de la pila";
    while(!pilaSimbolos->empty())
    {
        if(this->insertarSimbolo(pilaSimbolos->top()->setTipo(tipo)))
        {
            if(esGlobal)
            {
                ManejadorClass::ObtenerInstancia()->escribirDeclararVariableGlobal(pilaSimbolos->top());
            }
            else
            {
                //Registrarla como variable en nivel distinto, no se hace verificacion de sobreescritura pues
                //se valido por semantica que exista en ese nivel
                ManejadorClass::ObtenerInstancia()->registrarVariableLocal(pilaSimbolos->top(), simbolos->size());
                ManejadorClass::ObtenerInstancia()->escribirDeclararArray(pilaSimbolos->top(), false);
            }
        }
        pilaSimbolos->pop();
        regresable << tipo;
    }

    return regresable.str();
}

/**
 * @brief TablaSimbolos::apilarTipo
 *          Apilar temporalmente el tipo de algun simbolo
 * @param tipo
 */
void TablaSimbolos::apilarTipo(Tipo tipo)
{
    pilaTipos->push(tipo);
}

/**
 * @brief TablaSimbolos::desapilarTipo
 *          Extrae el tipo en el tope de la pila de tipos
 * @return
 */
Tipo TablaSimbolos::desapilarTipo(void)
{
    if(pilaTipos->empty())
    {
        qDebug() << "Desapilando tipo de una pila vacia";
    }
    else
    {
        Tipo t = pilaTipos->top();
        pilaTipos->pop();
        return t;
    }

    return T_INVALIDO;
}

/**
 * @brief TablaSimbolos::purgarPilas
 *          Libera la memoria de los simbolos asociados y vacía las pila
 */
void TablaSimbolos::purgarPilas(void)
{
    qDebug() << "Purgando las pilas";
    while(!pilaSimbolos->empty())
    {
        delete pilaSimbolos->top();
        pilaSimbolos->pop();
    }

    while(!pilaTipos->empty())
    {
        pilaTipos->pop();
    }

    while(!pilaValores->empty())
    {
        /*if(pilaValores->top()->getIdentificador().compare("HOLDER") == 0)
        {
            delete pilaValores->top();  //FIXME FUGA DE MEMORIA! Se esta liberando memoria ya liberada
        }*/
        pilaValores->pop();
    }

}


/**
 * @brief TablaSimbolos::entrarContextoFuncion
 *          Genera lo necesario para que una funcion tenga sus variables
 *          locales validas en el bytecode, inicializa el tipo de retorno
 *          como no definido
 * @param funcion
 */
void TablaSimbolos::entrarContextoFuncion(Simbolo* funcion)
{
    qDebug() << "Entrando en contexto funcion";

    //Un scope propio para las variables de la firma de funcion
    nuevoScope();

    //El contador de variables locales se reinicia a 0
    ManejadorClass::ObtenerInstancia()->resetearContadorLocalidades();

    retornoValidado = false;
    if(funcion != NULL)
    {
        contextoFuncion = funcion->setConstante();
    }
}


/**
 * @brief TablaSimbolos::setFirmaFuncion
 *          Al contexto de funcion actual le asigna la firma de metodo
 *          obtenida en forma de cadena
 * @param s
 */
void TablaSimbolos::setFirmaFuncion(const string& s)
{
    if(contextoFuncion != NULL)
    {
        contextoFuncion->setFirmaFuncion(s);
    }
    else
    {
        qDebug() << "Firma: No se ah puesto en contexto de funcion";
    }
}


/**
 * @brief TablaSimbolos::setTipoRetornoFuncion
 *          Especifica que valor retornara la funcion
 * @param t
 */
void TablaSimbolos::setTipoRetornoFuncion(Tipo t)
{
    if(contextoFuncion != NULL)
    {
        contextoFuncion->setTipoRetorno(t);
    }
    else
    {
        qDebug() << "Retorno: No se ah puesto en contexto de funcion";
    }
}

/**
 * @brief TablaSimbolos::getTipoRetornoFuncion
 *          Al mandarse llamar marca que la funcion tuvo almenos
 *          un retorno valido
 * @return
 */
Tipo TablaSimbolos::getTipoRetornoFuncion(void)
{
    retornoValidado = true;

    if(contextoFuncion != NULL)
    {
        return contextoFuncion->getTipoRetorno();
    }

    return T_INVALIDO;
}

/**
 * @brief TablaSimbolos::salirContextoFuncion
 *          Elimina los Simbolos generados para la funcion
 *          Verifica que la funcion retorne el valor que especifica
 *          o no retorne nada, tambien nulifica el contexto de funcion
 */
void TablaSimbolos::salirContextoFuncion(void)
{
    qDebug() << "saliendo scope de funcion";
    //aparte del scope propio de la funcion, tambien se eliminan las de la firma de funcion
    borrarScope();

    if(contextoFuncion != NULL)
    {

        if(!retornoValidado && contextoFuncion->getTipoRetorno() != T_INVALIDO)
        {
            refCompilador->escribirError("En funcion " + contextoFuncion->getIdentificador() + " no se dio ningun valor de retorno");
        }

        //Escribir el fin del metodo en el intermediario
        ManejadorClass::ObtenerInstancia()->escribirFinMetodo(contextoFuncion);

        contextoFuncion = NULL;
    }
    else
    {
        qDebug() << "Saliendo de un contexto de funcion nulo";
    }

}

string TablaSimbolos::generarEtiqueta(void)
{
    stringstream etq;

    etq << "ET_" << etiquetas.size();

    etiquetas.push_back(etq.str());
}

string TablaSimbolos::getEtiquetaActual(void)
{
    return etiquetas.at(etiquetas.size() - 1);
}

void TablaSimbolos::eliminarUltimaEtiqueta(void)
{
    etiquetas.pop_back();
}
