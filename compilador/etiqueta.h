#ifndef ETIQUETA_H
#define ETIQUETA_H

#include<string>

using namespace std;

enum Etiqueta_tipo
{
    ETQ_SI, ETQ_DESDE, ETQ_CASO, ETQ_NULL
};

class Etiqueta
{
public:
    Etiqueta(string id, Etiqueta_tipo tipo);
    string getIdentificador(void) const;
    Etiqueta_tipo getTipoEtq(void) const;

private:
    string identificador;
    Etiqueta_tipo tipoEtq;
};

#endif // ETIQUETA_H
