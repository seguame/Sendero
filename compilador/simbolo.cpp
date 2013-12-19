#include "simbolo.h"

template <typename T>
Simbolo::Simbolo(T const& valor, std::string const simbolo, Tipo const tipo):
    _ptr(new dato<T>(valor)),
    _simbolo(simbolo),
    _tipo(tipo)
{
}

Simbolo::~Simbolo()
{
    delete this->_ptr;
}

Simbolo::Simbolo(Simbolo const& otro):
    _ptr(otro._ptr->clone()),
    _tipo(otro.getTipo())
{
}

Simbolo& Simbolo::operator=(Simbolo const& otro)
{
    Simbolo(otro).swap(*this);
    return *this;
}

void Simbolo::swap(Simbolo& otro)
{
    std::swap(this->_ptr, otro._ptr);
}

template <typename T>
T& Simbolo::get()
{
    return dynamic_cast<dato<T>&>(*this->_ptr)._valor;
}

Tipo Simbolo::getTipo() const
{
    return _tipo;
}

std::string Simbolo::getSimbolo() const
{
    return _simbolo;
}
