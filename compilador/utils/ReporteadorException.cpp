#include "reportadorerrores.h"

const char* ReporteadorException::what() const throw()
{
    return "No se tiene inicializado un reporteador de errores";
}
