#ifndef __VENTANA_PRINCIPAL_H__
#define __VENTANA_PRINCIPAL_H__

#include <QMainWindow>

#include "EditorCodigo/editor_codigo.h"
#include "EditorCodigo/resaltador_sintaxis.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QFont;
class QListWidget;
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
        void compilar();

    private:
        void configurarEditor();
        void crearAcciones();
        void crearMenus();
        void crearToolBars();
        void crearBarraEstado();
        void crearWidgetsMovibles();
        void leerConfiguracion();
        void escribirConfiguracion();
        bool preguntarSiGuardar();
        void cargarArchivo(const QString &nombreArchivo);
        bool guardarArchivo(const QString &nombreArchivo);
        void setArchivoActual(const QString &nombreArchivo);
        QString soloNombreArchivo(const QString &nombreCompletoArchivo);


    //Atributos
    private:
        EditorCodigo *editorTexto;
        ResaltadorSintaxis *resaltador;
        QString archivoActual;

        QMenu *menuArchivo;
        QMenu *menuEditar;
        QMenu *menuAyuda;
        QMenu *menuVista;
        QToolBar *toolBarArchivo;
        QToolBar *toolBarEdicion;
        QAction *accionCompilar;
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
        QListWidget *salidaErrores;
        QListWidget *salidaCompilacion;
        QListWidget *salidaInformativa;
};

#endif //__VENTANA_PRINCIPAL_H__
