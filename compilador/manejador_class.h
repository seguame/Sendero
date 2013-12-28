#ifndef MANEJADOR_CLASS_H
#define MANEJADOR_CLASS_H

#include <fstream>
#include <sstream>
#include <vector>

#include <QDebug>

using namespace std;

class ManejadorClass
{
public:
    void escribirCabecera(const string& nombre);
    void aniadirInstruccion(const string& operacion,const string& parametro);
    void escribirArchivoParaEnsamblar(const string& ruta);

    static void Inicializar(void);
    static ManejadorClass* ObtenerInstancia(void);
    static void Terminar(void);
    static int Ensamblar(const string& ruta, const string& archivo);

private:
    ManejadorClass();
    ofstream archivo;
    vector< pair<string, string> > instrucciones;


    static ManejadorClass* instancia;
};

#endif // MANEJADOR_CLASS_H
