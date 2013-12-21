#include "simbolo.h"

template <typename T>
Simbolo::Simbolo(std::string const identificador, T const& valor):
    _ptr(new dato<T>(valor)),
    _identificador(identificador)
{
}

Simbolo::Simbolo(std::string const identificador):
    _ptr(new dato<int>(0)), //Si no le doy valor de inicio, caput :/
    _identificador(identificador)
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
T& Simbolo::getValor()
{
    return dynamic_cast<dato<T>&>(*this->_ptr)._valor;
}

template <typename T>
void Simbolo::setValor(T const& valor)
{
    //un metodo ineficiente pero aun no encuentro el
    //modo de dejar sin inicializar ptr_ y hacer validacio aqui
    //sobre si requiero solo reasignar el valor o debo crearlo
    //este par de lineas serian lo que se usaria para cuando
    //se asigna una nueva cadena
    delete _ptr;
    _ptr = new dato<T>(valor);
}

Tipo Simbolo::getTipo() const
{
    return _tipo;
}

std::string Simbolo::getIdentificador() const
{
    return _identificador;
}

void Simbolo::setTipo(Tipo tipo)
{
    _tipo = tipo;
}

void Simbolo::setEsDimensionado(bool b)
{
    _dimensionado = b;
}

bool Simbolo::esDimensionado() const
{
    return _dimensionado;
}

void Simbolo::setCantidadDimensiones(int cantidad)
{
    _cantDimensiones = cantidad;
}

unsigned int Simbolo::getCantidadDimensiones() const
{
    return _cantDimensiones;
}
