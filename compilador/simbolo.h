#ifndef SIMBOLO_H
#define SIMBOLO_H

#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include <sstream>
#include "enumerators.h"

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
        const std::string _identificador;
        Tipo _tipo;
        bool _constante;
        unsigned int _cantDimensiones;
        bool _inicializado;
        Tipo _retorno; //usado en funciones


    public:
        //template <typename T> Simbolo(std::string const& identificador, T const& valor);
        Simbolo(std::string const& identificador):
            _ptr(NULL),
            _identificador(identificador),
            _tipo(T_INVALIDO),
            _constante(false),
            _cantDimensiones(0),
            _inicializado(false),
            _retorno(T_INVALIDO)
            {
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
            if(!esConstante())
            {
                if(!_inicializado)
                {
                    _inicializado = true;
                }

                /*if(_ptr != NULL)
                {
                    delete _ptr;
                    _ptr = NULL;
                }
                _ptr = new dato<T>(valor);*/

                if(_ptr != NULL)
                {
                    if(strcmp(typeid(valor).name(), (typeid(std::string*).name())) == 0)
                    {
                        qDebug() << "Cambio de cadena";
                        delete _ptr;
                        _ptr = NULL;
                        _ptr = new dato<T>(valor);
                    }
                    else
                    {
                        qDebug() << "reasignacion";
                        dynamic_cast<dato<T>&>(*this->_ptr)._valor = valor;

                    }
                }
                else
                {
                    qDebug() << "Nuevo valor de tipo " << typeid(valor).name();
                    _ptr = new dato<T>(valor);
                }
            }
            else
            {
                qDebug() << "No se debe cambiar un valor constante!!!";
            }
            return this;
        }
        std::string getStringTipo(void) const;
        Tipo getTipo(void) const;
        Simbolo* setTipo(Tipo tipo);
        std::string getIdentificador() const;
        Simbolo* setCantidadDimensiones(unsigned int cantidad);
        unsigned int getCantidadDimensiones(void) const;
        Simbolo* setConstante(void);
        bool esConstante(void) const;
        std::string toString(void) const;
        Simbolo* setInicializado(void);
        bool estaInicializado(void) const;
        Simbolo* setTipoRetorno(Tipo t);
        Tipo getTipoRetorno(void) const;
};

#endif // SIMBOLO_H
