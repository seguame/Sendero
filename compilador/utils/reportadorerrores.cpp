#include "reportadorerrores.h"


ReportadorErrores* ReportadorErrores::instancia = NULL;

ReportadorErrores::ReportadorErrores(const string& ruta):
    cantErrores(0),
    maxErrores(5)
{
    archivo.open((ruta+".errores").c_str());
}

ReportadorErrores::ReportadorErrores():
    cantErrores(0),
    maxErrores(5)
{

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

void ReportadorErrores::Inicializar(void)
{
    ReportadorErrores::Terminar();

    instancia = new ReportadorErrores();
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
        throw ReporteadorException();
    }

    return instancia;
}

void ReportadorErrores::SetRuta(const string& ruta)
{
    if(instancia == NULL)
    {
        throw ReporteadorException();
    }

    archivo.open((ruta+".errores").c_str());
}

void ReportadorErrores::escribirError(int lineaActual, int columna, string lexico, string error, string renglon)
{
    if(cantErrores < maxErrores)
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

    ++cantErrores;
}

int ReportadorErrores::getCantidadErrores(void) const
{
    return cantErrores;
}
