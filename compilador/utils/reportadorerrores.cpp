#include "reportadorerrores.h"


ReportadorErrores* ReportadorErrores::instancia = NULL;

ReportadorErrores::ReportadorErrores(const string& ruta):
    maxErrores(10)
{
    archivo.open((ruta+".errores").c_str());
}

ReportadorErrores::~ReportadorErrores()
{
    if(archivo.is_open())
    {
        archivo.close();
    }
}

void ReportadorErrores::Inicializar(const string &ruta)
{
    ReportadorErrores::Terminar();

    instancia = new ReportadorErrores(ruta);
}

void ReportadorErrores::Terminar(void)
{
    if(instancia != NULL)
    {
        delete instancia;
        instancia = NULL;
    }
}


ReportadorErrores* ReportadorErrores::ObtenerInstancia(void)
{
    if(instancia == NULL)
    {
        throw new ReporteadorException;
    }

    return instancia;
}

void ReportadorErrores::escribirError(int lineaActual, int columna, string lexico, string error, string renglon)
{
    stringstream linea;
    stringstream colum;

    linea << (lineaActual + 1);
    archivo << linea.str() << ",,,";
    colum << columna;
    archivo << colum.str() << ",,,";
    archivo << lexico << ",,,";

    archivo << error << ",,," << renglon << endl;
}
