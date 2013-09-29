#ifndef __VENTANA_PRINCIPAL_H__
#define __VENTANA_PRINCIPAL_H__

#include <QMainWindow>

#include "EditorCodigo/editor_codigo.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

    public:
        VentanaPrincipal();

    protected:
        void closeEvent(QCloseEvent *evento);

    private slots:
        void nuevoArchivo();
        void abrir();
        bool guardar();
        bool guardarComo();
        void acercaDe();
        void archivoFueModificado();

    private:
        void crearAcciones();
        void crearMenus();
        void crearToolBars();
        void crearBarraEstado();
        void leerConfiguracion();
        void escribirConfiguracion();
        bool preguntarSiGuardar();
        void cargarArchivo(const QString &nombreArchivo);
        bool guardarArchivo(const QString &nombreArchivo);
        void setArchivoActual(const QString &nombreArchivo);
        QString soloNombreArchivo(const QString &nombreCompletoArchivo);


    //Atributos
    private:
        EditorCodigo *textEdit;
        QString archivoActual;

        QMenu *menuArchivo;
        QMenu *menuEditar;
        QMenu *menuAyuda;
        QToolBar *toolBarArchivo;
        QToolBar *toolBarEdicion;
        QAction *accionNuevo;
        QAction *accionAbrir;
        QAction *accionGuardar;
        QAction *accionGuardarComo;
        QAction *accionSalir;
        QAction *accionCortar;
        QAction *accionCopiar;
        QAction *accionPegar;
        QAction *accionAcercaDe;
        QAction *accionAcercaDeQT;
};

#endif //__VENTANA_PRINCIPAL_H__
