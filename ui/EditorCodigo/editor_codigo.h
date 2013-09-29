#ifndef __EDITOR_CODIGO_H__
#define __EDITOR_CODIGO_H__

#include <QPlainTextEdit>
#include <QObject>

#include "contador_linea.h"

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

    private slots:
        void actualizarAnchoAreaNumeroLinea();
        void resaltarLineaActual();
        void actualizarAreaNumeroLinea(const QRect &, int);

    private:
        QWidget *areaNumeroLinea;
};

#endif // __EDITOR_CODIGO_H__
