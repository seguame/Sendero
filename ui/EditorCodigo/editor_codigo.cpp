#include <QtGui>

#include "editor_codigo.h"


EditorCodigo::EditorCodigo(QWidget *padre) : QPlainTextEdit(padre)
{
    areaNumeroLinea = new ContadorLinea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(actualizarAnchoAreaNumeroLinea()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(actualizarAreaNumeroLinea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(resaltarLineaActual()));

    actualizarAnchoAreaNumeroLinea();
    resaltarLineaActual();
}



int EditorCodigo::anchuraAreaNumeroLinea()
{
    int digitos = 1;
    int max = qMax(1, blockCount());

    while (max >= 10)
    {
        max /= 10;
        ++digitos;
    }

    int anchura = 3 + fontMetrics().width(QLatin1Char('9')) * digitos;

    return anchura;
}



void EditorCodigo::actualizarAnchoAreaNumeroLinea()
{
    setViewportMargins(anchuraAreaNumeroLinea(), 0, 0, 0);
}


void EditorCodigo::actualizarAreaNumeroLinea(const QRect &rect, int dy)
{
    if (dy)
    {
        areaNumeroLinea->scroll(0, dy);
    }
    else
    {
        areaNumeroLinea->update(0, rect.y(), areaNumeroLinea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        actualizarAnchoAreaNumeroLinea();
    }
}


void EditorCodigo::resizeEvent(QResizeEvent *evento)
{
    QPlainTextEdit::resizeEvent(evento);

    QRect contenido = contentsRect();

    areaNumeroLinea->setGeometry(QRect(contenido.left(),
                                       contenido.top(),
                                       anchuraAreaNumeroLinea(),
                                       contenido.height()));
}


void EditorCodigo::resaltarLineaActual()
{
    QList<QTextEdit::ExtraSelection> seleccionesExtra;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection seleccionada;

        QColor colorLinea = QColor(Qt::yellow).lighter(170);

        seleccionada.format.setBackground(colorLinea);
        seleccionada.format.setProperty(QTextFormat::FullWidthSelection, true);
        seleccionada.cursor = textCursor();
        seleccionada.cursor.clearSelection();
        seleccionesExtra.append(seleccionada);
    }

    setExtraSelections(seleccionesExtra);
}



void EditorCodigo::eventoPintadoAreaNumeroLinea(QPaintEvent *evento)
{
    QPainter pintador(areaNumeroLinea);
    pintador.fillRect(evento->rect(), Qt::lightGray);



    QTextBlock bloqueTexto = firstVisibleBlock();
    int numeroBloque = bloqueTexto.blockNumber();

    int puntoMasAlto = (int) blockBoundingGeometry(bloqueTexto).translated(contentOffset()).top();
    int puntoMasBajo = puntoMasAlto + (int) blockBoundingRect(bloqueTexto).height();



    while (bloqueTexto.isValid() && puntoMasAlto <= evento->rect().bottom())
    {
        if (bloqueTexto.isVisible() && puntoMasBajo >= evento->rect().top())
        {
            QString numero = QString::number(numeroBloque + 1);

            pintador.setPen(Qt::black); //De que color se pintaran los numeros
            pintador.drawText(0,
                              puntoMasAlto,
                              areaNumeroLinea->width(),
                              fontMetrics().height(),
                              Qt::AlignRight,
                              numero);
        }

        bloqueTexto = bloqueTexto.next();
        puntoMasAlto = puntoMasBajo;
        puntoMasBajo = puntoMasAlto + (int) blockBoundingRect(bloqueTexto).height();
        numeroBloque++;
    }
}

