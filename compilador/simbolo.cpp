#include "simbolo.h"

template <typename T>
Simbolo::Simbolo(std::string const& identificador, T const& valor):
    _ptr(new dato<T>(valor)),
    _identificador(identificador),
    _tipo(T_INDEFINIDO),
    _constante(false),
    _dimensionado(false),
    _cantDimensiones(0)
{
}

Simbolo::Simbolo(std::string const& identificador):
    _ptr(new dato<int>(0)), //Si no le doy valor de inicio, caput :/
    _identificador(identificador),
    _tipo(T_INDEFINIDO),
    _constante(false),
    _dimensionado(false),
    _cantDimensiones(0)
{
}

Simbolo::Simbolo(Simbolo const& otro):
    _ptr(otro._ptr->clone()),
    _identificador(otro.getIdentificador()),
    _tipo(otro.getTipo()),
    _constante(otro.esConstante()),
    _dimensionado(otro.esDimensionado()),
    _cantDimensiones(otro.getCantidadDimensiones())
{
}

Simbolo::~Simbolo()
{
    qDebug() << "borrando " << _identificador.c_str();
    delete this->_ptr;
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
T& Simbolo::getValor(void)
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

Tipo Simbolo::getTipo(void) const
{
    return _tipo;
}

std::string Simbolo::getIdentificador(void) const
{
    return _identificador;
}

Simbolo* Simbolo::setTipo(Tipo tipo)
{
    _tipo = tipo;
    return this;
}

Simbolo* Simbolo::setEsDimensionado(void)
{
    _dimensionado = true;
    return this;
}

bool Simbolo::esDimensionado(void) const
{
    return _dimensionado;
}

void Simbolo::setCantidadDimensiones(int cantidad)
{
    _cantDimensiones = cantidad;
}

int Simbolo::getCantidadDimensiones(void) const
{
    return _cantDimensiones;
}

Simbolo* Simbolo::setConstante(void)
{
    _constante = true;
    return this;
}

bool Simbolo::esConstante(void) const
{
    return _constante;
}

std::string Simbolo::toString(void) const
{
    std::stringstream dimensiones;
    dimensiones << _cantDimensiones;
    std::string tipo = (_tipo == T_ENTERO ? "Entero" : _tipo == T_REAL ? "Real" : _tipo == T_BOOLEANO ? "Booleano" : _tipo == T_CADENA ? "Cadena" : "Indefinido");

    return "ID: " + _identificador + " Tipo: " + tipo + " Const: " + (_constante ? "Si" : "No") + " Dimen: " + (_dimensionado ? "Si":"No") + " #: " + dimensiones.str();
}
