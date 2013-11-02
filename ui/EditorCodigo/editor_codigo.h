#ifndef __EDITOR_CODIGO_H__
#define __EDITOR_CODIGO_H__

#include <QPlainTextEdit>
#include <QObject>

#include "contador_linea.h"
#include "ui/EditorCodigo/resaltador_sintaxis.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE



class EditorCodigo : public QPlainTextEdit
{
    Q_OBJECT

    public:
        EditorCodigo(QWidget *padre = 0);

        void eventoPintadoAreaNumeroLinea(QPaintEvent *evento);
        int anchuraAreaNumeroLinea();

    protected:
        void resizeEvent(QResizeEvent *evento);

    private:
        void configurarEditor();

    private slots:
        void actualizarAnchoAreaNumeroLinea();
        void resaltarLineaActual();
        void actualizarAreaNumeroLinea(const QRect &, int);

    private:
        QWidget *areaNumeroLinea;
        QFont fuente;

        ResaltadorSintaxis *resaltador;
};

#endif // __EDITOR_CODIGO_H__
