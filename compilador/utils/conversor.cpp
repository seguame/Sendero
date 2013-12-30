#include "conversor.h"


STR2X_ERROR Conversor::cadena2Entero(int &l, char const *s, int base)
{
    char *fin;
    long  L;
    errno = 0;
    L = strtol(s, &fin, base);

    l = (int)L;

    if ((errno == ERANGE && L == LONG_MAX))
    {
        return OVERFLOW;
    }

    if ((errno == ERANGE && L == LONG_MIN))
    {
        return UNDERFLOW;
    }

    if (*s == '\0' || *fin != '\0')
    {
        return INCONVERTIBLE;
    }

    return EXITO;
}

STR2X_ERROR Conversor::cadena2Booleano(bool &b, char const *s)
{
    if(strcmp("Verdadero", s) == 0)
    {
        b = true;
        return EXITO;
    }

    if(strcmp("Falso", s) == 0)
    {
        b = false;
        return EXITO;
    }

    return INCONVERTIBLE;
}

STR2X_ERROR Conversor::cadena2Real(double &d, char const *s)
{
    char* fin;
    errno = 0;

    char* anterior = setlocale(LC_NUMERIC, "C");
    d = strtod(s, &fin);
    setlocale(LC_NUMERIC, anterior);

    if (errno == ERANGE)
    {
        return OVERFLOW;
    }

    if (*s == '\0' || *fin != '\0')
    {
        return INCONVERTIBLE;
    }

    return EXITO;
}
