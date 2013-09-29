#include <QtGui>

#include "VentanaPrincipal.h"
#include "EditorCodigo/editor_codigo.h"
#include "EditorCodigo/resaltador_sintaxis.h"

VentanaPrincipal::VentanaPrincipal()
{
    configurarEditor();



    crearAcciones();
    crearMenus();
    crearToolBars();
    crearBarraEstado();
    crearWidgetsMovibles();

    leerConfiguracion();

    connect(editorTexto->document(),
            SIGNAL(contentsChanged()),
            this,
            SLOT(archivoFueModificado()));

    setArchivoActual("");
    setUnifiedTitleAndToolBarOnMac(true);
}



void VentanaPrincipal::closeEvent(QCloseEvent *evento)
{
    if (preguntarSiGuardar())
    {
        escribirConfiguracion();
        evento->accept();
    }
    else
    {
        evento->ignore();
    }
}

void VentanaPrincipal::configurarEditor()
{
    QFont fuente;
    fuente.setFamily("Courier");
    fuente.setFixedPitch(true);
    fuente.setPointSize(14);

    editorTexto = new EditorCodigo;
    editorTexto->setFont(fuente);

    resaltador = new ResaltadorSintaxis(editorTexto->document());

    setCentralWidget(editorTexto);
}


void VentanaPrincipal::nuevoArchivo()
{
    if (preguntarSiGuardar())
    {
        editorTexto->clear();
        setArchivoActual("");
    }
}


void VentanaPrincipal::abrir()
{
    if (preguntarSiGuardar())
    {
        QString nombreArchivo = QFileDialog::getOpenFileName(this,
                                                             tr("Abrir archivo"),
                                                             "",
                                                             "Cualquiera (*.*);; Ir Files (*.ir);; C++ Files (*.cpp *.h)");

        if (!nombreArchivo.isEmpty())
        {
            cargarArchivo(nombreArchivo);
        }
    }
}


bool VentanaPrincipal::guardar()
{
    if (archivoActual.isEmpty())
    {
        return guardarComo();
    }
    else
    {
        return guardarArchivo(archivoActual);
    }
}



bool VentanaPrincipal::guardarComo()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this);

    if (nombreArchivo.isEmpty())
    {
        return false;
    }

    return guardarArchivo(nombreArchivo);
}



void VentanaPrincipal::acercaDe()
{
   QMessageBox::about(this, tr("Acerca de Sendero"),
            tr("Editor para escribir codigo en el lenguaje IR\n"
               "Trabajo para el taller de compiladores 2013-B\n"
               "Miguel Seguame Reyes "));
}


void VentanaPrincipal::archivoFueModificado()
{
    setWindowModified(editorTexto->document()->isModified());
}



void VentanaPrincipal::crearAcciones()
{
    accionNuevo = new QAction(QIcon(":/imagenes/new.png"), tr("Nuevo"), this);
    accionNuevo->setShortcuts(QKeySequence::New);
    accionNuevo->setStatusTip(tr("Crear un nuevo archivo"));
    connect(accionNuevo, SIGNAL(triggered()), this, SLOT(nuevoArchivo()));

    accionAbrir = new QAction(QIcon(":/imagenes/open.png"), tr("Abrir..."), this);
    accionAbrir->setShortcuts(QKeySequence::Open);
    accionAbrir->setStatusTip(tr("Abrir un archivo existente"));
    connect(accionAbrir, SIGNAL(triggered()), this, SLOT(abrir()));

    accionGuardar = new QAction(QIcon(":/imagenes/save.png"), tr("Guardar"), this);
    accionGuardar->setShortcuts(QKeySequence::Save);
    accionGuardar->setStatusTip(tr("Guardar cambios del archivo"));
    connect(accionGuardar, SIGNAL(triggered()), this, SLOT(guardar()));

    accionGuardarComo = new QAction(tr("Guardar como..."), this);
    accionGuardarComo->setShortcuts(QKeySequence::SaveAs);
    accionGuardarComo->setStatusTip(tr("Guardar archivo con un nuevo nombre"));
    connect(accionGuardarComo, SIGNAL(triggered()), this, SLOT(guardarComo()));


    accionSalir = new QAction(tr("Salir"), this);
    accionSalir->setShortcuts(QKeySequence::Quit);

    accionSalir->setStatusTip(tr("Salir de la aplicacion"));
    connect(accionSalir, SIGNAL(triggered()), this, SLOT(close()));


    accionCortar = new QAction(QIcon(":/imagenes/cut.png"), tr("Cortar"), this);

    accionCortar->setShortcuts(QKeySequence::Cut);
    accionCortar->setStatusTip(tr("Cortar el contenido seleccionado al portapapeles"));
    connect(accionCortar, SIGNAL(triggered()), editorTexto, SLOT(cut()));

    accionCopiar = new QAction(QIcon(":/imagenes/copy.png"), tr("Copiar"), this);
    accionCopiar->setShortcuts(QKeySequence::Copy);
    accionCopiar->setStatusTip(tr("Copiar el contenido seleccionado al portapapeles"));
    connect(accionCopiar, SIGNAL(triggered()), editorTexto, SLOT(copy()));

    accionPegar = new QAction(QIcon(":/imagenes/paste.png"), tr("Pegar"), this);
    accionPegar->setShortcuts(QKeySequence::Paste);
    accionPegar->setStatusTip(tr("Pegar el contenido del portapapeles en el lugar seleccionado"));
    connect(accionPegar, SIGNAL(triggered()), editorTexto, SLOT(paste()));

    accionAcercaDe = new QAction(tr("Acerca de"), this);
    accionAcercaDe->setStatusTip(tr("Mostrar información de la aplicación"));
    connect(accionAcercaDe, SIGNAL(triggered()), this, SLOT(acercaDe()));


    accionAcercaDeQT = new QAction(tr("Acerca de Qt"), this);
    accionAcercaDeQT->setStatusTip(tr("Mostrar informacion de QT"));
    connect(accionAcercaDeQT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


    //Dejar que la disponibilidad de estas acciones se manejen por los eventos
    accionCortar->setEnabled(false);
    accionCopiar->setEnabled(false);

    connect(editorTexto,
            SIGNAL(copyAvailable(bool)),
            accionCortar,
            SLOT(setEnabled(bool)));

    connect(editorTexto,
            SIGNAL(copyAvailable(bool)),
            accionCopiar,
            SLOT(setEnabled(bool)));
}



void VentanaPrincipal::crearMenus()
{
    menuArchivo = menuBar()->addMenu(tr("Archivo"));

    menuArchivo->addAction(accionNuevo);
    menuArchivo->addAction(accionAbrir);
    menuArchivo->addAction(accionGuardar);
    menuArchivo->addAction(accionGuardarComo);
    menuArchivo->addSeparator();
    menuArchivo->addAction(accionSalir);

    menuEditar = menuBar()->addMenu(tr("Edicion"));
    menuEditar->addAction(accionCortar);
    menuEditar->addAction(accionCopiar);
    menuEditar->addAction(accionPegar);

    menuVista = menuBar()->addMenu(tr("Vista"));

    menuBar()->addSeparator();

    menuAyuda = menuBar()->addMenu(tr("Ayuda"));
    menuAyuda->addAction(accionAcercaDe);
    menuAyuda->addAction(accionAcercaDeQT);
}



void VentanaPrincipal::crearToolBars()
{
    toolBarArchivo = addToolBar(tr("Archivo"));

    toolBarArchivo->addAction(accionNuevo);
    toolBarArchivo->addAction(accionAbrir);
    toolBarArchivo->addAction(accionGuardar);

    toolBarEdicion = addToolBar(tr("Edicion"));
    toolBarEdicion->addAction(accionCortar);
    toolBarEdicion->addAction(accionCopiar);
    toolBarEdicion->addAction(accionPegar);
}



void VentanaPrincipal::crearBarraEstado()
{
    statusBar()->showMessage(tr("Preparado"));
}

void VentanaPrincipal::crearWidgetsMovibles()
{
    QDockWidget *dock = new QDockWidget(tr("Problemas"), this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    salidaErrores = new QListWidget(dock);
    salidaErrores->addItems(QStringList()
            << "Testing");
    dock->setWidget(salidaErrores);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    menuVista->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Compilador"), this);
    salidaCompilacion = new QListWidget(dock);
    salidaCompilacion->addItems(QStringList()
            << "Aun no sabes compilar :B");
    dock->setWidget(salidaCompilacion);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    menuVista->addAction(dock->toggleViewAction());
}


void VentanaPrincipal::leerConfiguracion()
{
    QSettings settings("Universidad de Guadalajara", "Sendero");
    QPoint posicion = settings.value("posicion", QPoint(200, 200)).toPoint();
    QSize tamanio = settings.value("tamanio", QSize(640, 480)).toSize();
    resize(tamanio);
    move(posicion);
}



void VentanaPrincipal::escribirConfiguracion()
{
    QSettings settings("Universidad de Guadalajara", "Sendero");
    settings.setValue("posicion", pos());
    settings.setValue("tamanio", size());
}


bool VentanaPrincipal::preguntarSiGuardar()
{
    if (editorTexto->document()->isModified())
    {
        QMessageBox::StandardButton interaccion;

        interaccion = QMessageBox::warning(this,
                                   tr("Sendero"),
                                   tr("Se modifico el documento.\nQuieres guardar los cambios?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (interaccion == QMessageBox::Save)
        {
            return guardar();
        }
        else if (interaccion == QMessageBox::Cancel)
        {
            return false;
        }
    }

    return true;
}


void VentanaPrincipal::cargarArchivo(const QString &nombreArchivo)
{
    QFile archivo(nombreArchivo);

    if (!archivo.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Sendero"),
                             tr("No se pudo abrir el archivo %1:\n%2.")
                             .arg(nombreArchivo)
                             .arg(archivo.errorString()));
        return;
    }

    QTextStream flujoEntrada(&archivo);

    #ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    editorTexto->setPlainText(flujoEntrada.readAll());

    #ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
    #endif

    setArchivoActual(nombreArchivo);
    statusBar()->showMessage(tr("Archivo cargado"), 3000);

    archivo.close();
}


bool VentanaPrincipal::guardarArchivo(const QString &nombreArchivo)
{
    QFile archivo(nombreArchivo);

    if (!archivo.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Sendero"),
                             tr("No se pudo guardar %1:\n%2.")
                             .arg(nombreArchivo)
                             .arg(archivo.errorString()));
        return false;
    }

    QTextStream flujoSalida(&archivo);

    #ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    flujoSalida << (editorTexto->toPlainText());

    #ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
    #endif

    setArchivoActual(nombreArchivo);
    statusBar()->showMessage(tr("Archivo guardado"), 2000);

    archivo.close();

    return true;
}


void VentanaPrincipal::setArchivoActual(const QString &nombreArchivo)
{
    archivoActual = nombreArchivo;

    editorTexto->document()->setModified(false);
    setWindowModified(false);

    QString nombreaAMostrar = archivoActual;

    if (archivoActual.isEmpty())
    {
        nombreaAMostrar = "Archivo sin nombre.txt";
    }

    setWindowFilePath(nombreaAMostrar);
}


QString VentanaPrincipal::soloNombreArchivo(const QString &nombreCompletoArchivo)
{
    return QFileInfo(nombreCompletoArchivo).fileName();
}
