#ifndef SIMBOLO_H
#define SIMBOLO_H

#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include "enumerators.h"

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
        const std::string _simbolo;
        Tipo _tipo;
        bool _dimensionado;
        unsigned int _cantDimensiones;


    public:
        template <typename T> Simbolo(std::string const simbolo, T const& valor);
        Simbolo(std::string const simbolo);
        Simbolo(Simbolo const& otro);
        Simbolo& operator= (Simbolo const& otro);
        ~Simbolo();
        void swap(Simbolo& otro);
        template <typename T> T& getValor();
        template <typename T> void setValor(T const& valor);
        Tipo getTipo() const;
        void setTipo(Tipo tipo);
        std::string getSimbolo() const;
        void setEsDimensionado(bool b);
        bool esDimensionado() const;
        void setCantidadDimensiones(int cantidad);
        unsigned int getCantidadDimensiones() const;
};

#endif // SIMBOLO_H
