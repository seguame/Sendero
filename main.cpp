#include <QApplication>

#include "ui/VentanaPrincipal.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Imagenes);

    QApplication app(argc, argv);
    app.setOrganizationName("Universidad de Guadalajara");
    app.setApplicationName("Sendero");

    VentanaPrincipal ventana;

    ventana.show();

    return app.exec();
}
