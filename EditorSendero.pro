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
    compilador/compilador.cpp

HEADERS  += \
    ui/VentanaPrincipal.h \
    ui/EditorCodigo/editor_codigo.h \
    ui/EditorCodigo/contador_linea.h \
    ui/EditorCodigo/resaltador_sintaxis.h \
    compilador/compilador.h

RESOURCES += \
    Imagenes.qrc

OTHER_FILES += \
    imagenes/save.png \
    imagenes/paste.png \
    imagenes/open.png \
    imagenes/new.png \
    imagenes/cut.png \
    imagenes/copy.png \
    imagenes/compile.png
