#include "simbolo.h"

Simbolo::Simbolo(Simbolo const& otro):
    _ptr(otro._ptr->clone()),
    _identificador(otro.getIdentificador()),
    _tipo(otro.getTipo()),
    _constante(otro.esConstante()),
    _dimensionado(otro.esDimensionado()),
    _cantDimensiones(otro.getCantidadDimensiones()),
    _inicializado(otro.estaInicializado())
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

std::string Simbolo::getStringTipo(void) const
{
    return (_tipo == T_ENTERO ? "Entero" :
            _tipo == T_REAL ? "Real" :
            _tipo == T_BOOLEANO ? "Logico" :
            _tipo == T_CADENA ? "Alfabetico" :
            _tipo == T_FUNCION ? "Funcion" :
            "Indefinido");
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

Simbolo* Simbolo::setInicializado(void)
{
    _inicializado = true;
    return this;
}

bool Simbolo::estaInicializado(void) const
{
    return _inicializado;
}

std::string Simbolo::toString(void) const
{
    std::stringstream dimensiones;
    dimensiones << _cantDimensiones;
    std::string tipo = getStringTipo();


    std::stringstream valor;

    if(_ptr == NULL || (!estaInicializado() && _tipo != T_FUNCION))
    {
        valor << "NULO";
    }
    else
    {
        switch(_tipo)
        {
            case T_ENTERO:
            qDebug() << "Entero";
                valor << this->getValor<long>();
                break;
            case T_REAL:
            qDebug() << "real";
                valor << this->getValor<double>();
                break;
            case T_BOOLEANO:
            qDebug() << "bool";
                valor << this->getValor<bool>();
                break;
            case T_CADENA:
            qDebug() << "cadena";
                valor << (this->getValor<std::string>());
                break;
            case T_FUNCION:
            qDebug() << "funcion";
                valor << "FUNCION";
                break;
            default:
                valor << "NULO";
                break;
        }
    }
    qDebug() << "pasamos";

    return "ID: " + _identificador + " Tipo: " + tipo + " Const: " + (_constante ? "Si" : "No") + " Dimen: " + (_dimensionado ? "Si":"No") + " #: " + dimensiones.str() +
            " Inicializado: " + (_inicializado ? "Si":"No") + " Valor: " + valor.str();
}
