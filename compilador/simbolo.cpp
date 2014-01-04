#include "simbolo.h"

Simbolo::Simbolo(Simbolo const& otro):
    _ptr(otro._ptr->clone()),
    _identificador(otro.getIdentificador()),
    _tipo(otro.getTipo()),
    _constante(otro.esConstante()),
    _cantDimensiones(otro.getCantidadDimensiones()),
    _inicializado(otro.estaInicializado())
{
}

Simbolo::~Simbolo()
{
    qDebug() << "borrando " << _identificador.c_str();
    delete this->_ptr;
    delete _tamDimension;
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
            _tipo == T_CARACTER ? "Caracter" :
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

Simbolo* Simbolo::setCantidadDimensiones(unsigned int cantidad)
{
    _cantDimensiones = cantidad;

    if(cantidad > 0)
    {
        //Las variables con dimension ya se inicializan en bytecode
        setInicializado();
    }

    return this;
}

unsigned int Simbolo::getCantidadDimensiones(void) const
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

Simbolo* Simbolo::setTipoRetorno(Tipo t)
{
    _retorno = t;
    return this;
}

Tipo Simbolo::getTipoRetorno(void) const
{
    return _retorno;
}

Simbolo* Simbolo::setTemporal(void)
{
    _temporal = true;
    return this;
}

bool Simbolo::esTemporal(void) const
{
    return _temporal;
}

Simbolo* Simbolo::setFirmaFuncion(const string& s)
{
    _firma = s;
    return this;
}

string Simbolo::getFirmaFuncion(void) const
{
    return _firma;
}

Simbolo* Simbolo::addTamanioDimension(int tam)
{
    _tamDimension->push_back(tam);
    return this;
}

int Simbolo::getTamanioDimension(int pos) const
{
    if((unsigned int)pos < _tamDimension->size())
    {
        return _tamDimension->at(pos);
    }
    else
    {
        qDebug() << "Intentando obtener valor en una dimension de variable inexistente";
        return 0;
    }
}

Simbolo* Simbolo::setAlias(const string& alias)
{
    _alias = alias;
    return this;
}

string Simbolo::getAlias(void) const
{
    return _alias;
}

std::string Simbolo::toString(void) const
{
    std::stringstream dimensiones;
    dimensiones << _cantDimensiones;
    std::string tipo = getStringTipo();
    std::stringstream tRetorno;

    std::stringstream valor;

    if(_tipo == T_FUNCION)
    {
        valor << "FUNCION";
        switch(_retorno)
        {
            case T_ENTERO:
                tRetorno << "Entero";
                break;
            case T_REAL:
                tRetorno << "Real";
                break;
            case T_BOOLEANO:
                tRetorno << "Logico";
                break;
            case T_CADENA:
                tRetorno << "Alfabetico";
                break;
            case T_CARACTER:
                tRetorno << "Caracter";
                break;
            default:
                tRetorno << "Void";
                break;
        }
    }
    else if(_ptr == NULL || !estaInicializado())
    {
        valor << "NULO";
    }
    else
    {
        switch(_tipo)
        {
            case T_ENTERO:
                valor << this->getValor<int>();
                break;
            case T_REAL:
                valor << this->getValor<double>();
                break;
            case T_BOOLEANO:
                valor << this->getValor<bool>();
                break;
            case T_CADENA:
                valor << (this->getValor<std::string>());
                break;
            case T_CARACTER:
                valor << this->getValor<char>();
                break;
            default:
                valor << "NULO";
                break;
        }
    }

    return "ID: " + _identificador + " Tipo: " + tipo + " Const: " + (_constante ? "Si" : "No") + " Temp: " + (_temporal? "Si":"No") +" Dimen: " + dimensiones.str() +
            " Inicializado: " + (_inicializado ? "Si":"No") + " Valor: " + valor.str() + " Retorno: " + tRetorno.str() + " Firma: " + _firma + " Alias: " + _alias;
}
