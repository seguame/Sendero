#-------------------------------------------------
#
# Project created by QtCreator 2013-09-28T17:54:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EditorSendero
TEMPLATE = app


SOURCES += main.cpp \
    ui/VentanaPrincipal.cpp \
    ui/EditorCodigo/contador_linea.cpp \
    ui/EditorCodigo/editor_codigo.cpp \
    ui/EditorCodigo/resaltador_sintaxis.cpp \
    compilador/compilador.cpp \
    compilador/simbolo.cpp \
    compilador/tablasimbolos.cpp \
    compilador/utils/conversor.cpp \
    compilador/utils/reportadorerrores.cpp \
    compilador/utils/ReporteadorException.cpp \
    compilador/class_file.cpp \
    compilador/manejador_class.cpp \
    ui/externo/redirect.cpp \
    ui/externo/qconsolewidget.cpp \
    compilador/etiqueta.cpp

HEADERS  += \
    ui/VentanaPrincipal.h \
    ui/EditorCodigo/editor_codigo.h \
    ui/EditorCodigo/contador_linea.h \
    ui/EditorCodigo/resaltador_sintaxis.h \
    compilador/compilador.h \
    compilador/simbolo.h \
    compilador/enumerators.h \
    compilador/tablasimbolos.h \
    compilador/utils/conversor.h \
    compilador/utils/reportadorerrores.h \
    compilador/utils/ReporteadorException.h \
    compilador/class_file.h \
    compilador/manejador_class.h \
    ui/externo/redirect.h \
    ui/externo/qconsolewidget.h \
    compilador/etiqueta.h

RESOURCES += \
    Imagenes.qrc

OTHER_FILES += \
    imagenes/save.png \
    imagenes/paste.png \
    imagenes/open.png \
    imagenes/new.png \
    imagenes/cut.png \
    imagenes/copy.png \
    imagenes/compile.png \
    compilador/codop.csv
