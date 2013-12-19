#ifndef SIMBOLO_H
#define SIMBOLO_H

#include <algorithm>
#include <string>
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
        const Tipo _tipo;


    public:
        template <typename T> Simbolo(T const& valor, std::string const simbolo, Tipo const tipo);
        Simbolo(Simbolo const& otro);
        Simbolo& operator= (Simbolo const& otro);
        ~Simbolo();
        void swap(Simbolo& otro);
        template <typename T> T& get();
        Tipo getTipo() const;
        std::string getSimbolo() const;
};

#endif // SIMBOLO_H
