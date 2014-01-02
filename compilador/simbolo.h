#ifndef SIMBOLO_H
#define SIMBOLO_H

#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include <sstream>
#include "enumerators.h"

using namespace std;

#include <QDebug>

class Simbolo
{
    private:
        struct base
        {
            virtual ~base() {}
            virtual base* clone() const = 0;
        };

        template <typename T>
        struct dato: base
        {
            dato(T const& valor):
                _valor ( valor )
            {
            }


            base* clone() const
            {
                return new dato<T>(*this);
            }

            T _valor;
        };

        base* _ptr;
        const string _identificador;
        string _alias;
        Tipo _tipo;
        bool _constante;
        unsigned int _cantDimensiones;
        vector<int>* _tamDimension;
        bool _inicializado;
        bool _temporal;

        //usados en funciones
        Tipo _retorno;
        unsigned int _cantArgumentos;
        std::string _firma;


    public:
        //template <typename T> Simbolo(std::string const& identificador, T const& valor);
        Simbolo(string const& identificador):
            _ptr(NULL),
            _identificador(identificador),
            _alias(""),
            _tipo(T_INVALIDO),
            _constante(false),
            _cantDimensiones(0),
            _tamDimension(NULL),
            _inicializado(false),
            _temporal(false),
            _retorno(T_INVALIDO),
            _cantArgumentos(0),
            _firma("")
            {
                _tamDimension = new vector<int>();
            }
        Simbolo(Simbolo const& otro);
        Simbolo& operator= (Simbolo const& otro);
        ~Simbolo();
        void swap(Simbolo& otro);
        template <typename T> T& getValor(void) const
        {
            return dynamic_cast<dato<T>&>(*this->_ptr)._valor;
        }

        template <typename T> Simbolo* setValor(T const& valor)
        {

            if(!_inicializado)
            {
                _inicializado = true;
            }

            if(_ptr != NULL)
            {
                if(strcmp(typeid(valor).name(), (typeid(string*).name())) == 0)
                {
                    qDebug() << "Cambio de cadena";
                    delete _ptr;
                    _ptr = NULL;
                    _ptr = new dato<T>(valor);
                }
                else
                {

                    qDebug() << "reasignacion";
                    try
                    {
                        dynamic_cast<dato<T>&>(*this->_ptr)._valor = valor;
                    }
                    catch (bad_alloc ba)
                    {
                        qDebug() << "Cambio total de dato, regenerando";
                        delete _ptr;
                        _ptr = NULL;
                        _ptr = new dato<T>(valor);
                    }
                }

                //TODO: Cambiar el tipo de dato segun se asigne
            }
            else
            {
                qDebug() << "Nuevo valor de tipo " << typeid(valor).name();
                _ptr = new dato<T>(valor);
            }
            return this;
        }
        string getStringTipo(void) const;
        Tipo getTipo(void) const;
        Simbolo* setTipo(Tipo tipo);
        string getIdentificador() const;
        Simbolo* setCantidadDimensiones(unsigned int cantidad);
        unsigned int getCantidadDimensiones(void) const;
        Simbolo* setConstante(void);
        bool esConstante(void) const;
        string toString(void) const;
        Simbolo* setInicializado(void);
        bool estaInicializado(void) const;
        Simbolo* setTipoRetorno(Tipo t);
        Tipo getTipoRetorno(void) const;
        Simbolo* setTemporal(void);
        bool esTemporal(void) const;
        Simbolo* setFirmaFuncion(const string& s);
        string getFirmaFuncion(void) const;
        Simbolo* addTamanioDimension(int tam);
        int getTamanioDimension(int pos) const;
        Simbolo* setAlias(const string& alias);
        string getAlias(void) const;
};

#endif // SIMBOLO_H
