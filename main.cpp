#include <QApplication>

#include "VentanaPrincipal.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    app.setOrganizationName("Universidad de Guadalajara");
    app.setApplicationName("Sendero");

    VentanaPrincipal ventana;

    ventana.show();

    return app.exec();
}
