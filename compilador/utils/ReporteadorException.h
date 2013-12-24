#ifndef REPORTEADOREXCEPTION_H
#define REPORTEADOREXCEPTION_H

#include <exception>

class ReporteadorException: public std::exception
{
    virtual const char* what() const throw();
};

#endif // REPORTEADOREXCEPTION_H
