#ifndef REPORTADORERRORES_H
#define REPORTADORERRORES_H

#include <fstream>
#include <sstream>
#include "ReporteadorException.h"

using namespace std;

class ReportadorErrores
{
public:
    static void Inicializar(const string& ruta);
    static void Inicializar(void);
    static void Terminar(void);
    static ReportadorErrores* ObtenerInstancia(void);

    void SetRuta(const string& ruta);
    void escribirError(int lineaActual, int columna, string lexico, string error, string renglon);
    int getCantidadErrores(void) const;

private:
    ReportadorErrores(const string& ruta);
    ReportadorErrores();
    ~ReportadorErrores();


    static ReportadorErrores* instancia;
    ofstream archivo;
    int cantErrores;
    const int maxErrores;
};

#endif // REPORTADORERRORES_H
