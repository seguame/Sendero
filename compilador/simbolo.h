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
        bool _dimensionado;
        int _cantDimensiones;


    public:
        template <typename T> Simbolo(std::string const& identificador, T const& valor);
        Simbolo(std::string const& identificador);
        Simbolo(Simbolo const& otro);
        Simbolo& operator= (Simbolo const& otro);
        ~Simbolo();
        void swap(Simbolo& otro);
        template <typename T> T& getValor(void);
        template <typename T> void setValor(T const& valor);
        Tipo getTipo(void) const;
        Simbolo* setTipo(Tipo tipo);
        std::string getIdentificador() const;
        Simbolo* setEsDimensionado(void);
        bool esDimensionado(void) const;
        void setCantidadDimensiones(int cantidad);
        int getCantidadDimensiones(void) const;
        Simbolo* setConstante(void);
        bool esConstante(void) const;
        std::string toString(void) const;
};

#endif // SIMBOLO_H
