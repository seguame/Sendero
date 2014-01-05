#include "etiqueta.h"

Etiqueta::Etiqueta(string id, Etiqueta_tipo tipo)
{
    identificador = id;
    tipoEtq = tipo;
}


string Etiqueta::getIdentificador(void) const
{
    return identificador;
}

Etiqueta_tipo Etiqueta::getTipoEtq(void) const
{
    return tipoEtq;
}
