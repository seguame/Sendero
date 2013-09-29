#ifndef __CONTADORLINEA_H__
#define __CONTADORLINEA_H__

#include <QWidget>

#include "editor_codigo.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE

class EditorCodigo;

class ContadorLinea : public QWidget
{
    Q_OBJECT
    public:
        ContadorLinea(EditorCodigo *editor);
        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *evento);

    private:
        EditorCodigo *editorCodigo;
};

#endif // __CONTADORLINEA_H__
