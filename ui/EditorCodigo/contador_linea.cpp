#include "contador_linea.h"

ContadorLinea::ContadorLinea(EditorCodigo *editor) : QWidget(editor)
{
    editorCodigo = editor;
}

QSize ContadorLinea::sizeHint() const
{
    return QSize(editorCodigo->anchuraAreaNumeroLinea(), 0);
}

void ContadorLinea::paintEvent(QPaintEvent *evento)
{
    editorCodigo->eventoPintadoAreaNumeroLinea(evento);
}
