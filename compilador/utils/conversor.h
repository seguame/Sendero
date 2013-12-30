#ifndef CONVERSOR_H
#define CONVERSOR_H

#include <cstdlib>
#include <climits>
#include <cstring>
#include <cerrno>
#include <clocale>

enum STR2X_ERROR { EXITO, OVERFLOW, UNDERFLOW, INCONVERTIBLE };

class Conversor
{
public:
    static STR2X_ERROR cadena2Real(double &d, char const *s);
    static STR2X_ERROR cadena2Entero(int &i, char const *s, int base = 0);
    static STR2X_ERROR cadena2Booleano(bool &b, char const *s);
};

#endif // CONVERSOR_H
