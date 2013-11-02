#ifndef __VENTANA_PRINCIPAL_H__
#define __VENTANA_PRINCIPAL_H__

#include <QMainWindow>

#include "EditorCodigo/editor_codigo.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QFont;
class QListWidget;
class QTreeView;
class QAbstractItemModel;
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

        void actualizarVistas();
        void actualizarVistaErrores(QString ruta);
        void actualizarVistaInformativa(QString ruta);


    //Atributos
    private:
        EditorCodigo *editorTexto;
        QString archivoActual;
        QString rutaActual;
        QString archivoSinExtencion;

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

        QTreeView *vistaErrores;
        QTreeView *vistaInformativa;
        QAbstractItemModel *modeloErrores;
        QAbstractItemModel *modeloInformativa;
};

#endif //__VENTANA_PRINCIPAL_H__
