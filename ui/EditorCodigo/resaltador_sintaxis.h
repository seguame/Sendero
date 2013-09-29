#ifndef __RESALTADOR_SINTAXIS_H__
#define __RESALTADOR_SINTAXIS_H__

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class ResaltadorSintaxis : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        ResaltadorSintaxis(QTextDocument *padre = 0);

    protected:
        void highlightBlock(const QString &texto);

    private:
        struct ReglaDeResaltado
        {
            QRegExp patron;
            QTextCharFormat formato;
        };

        QVector<ReglaDeResaltado> reglasDeResaltado;

        QRegExp expresionInicioComentario;
        QRegExp expresionFinComentario;

        QTextCharFormat formatoPalabraReservada;
        QTextCharFormat formatoClase;
        QTextCharFormat formatoComentarioUnaLinea;
        QTextCharFormat formatoComentarioMultiLinea;
        QTextCharFormat formatoEncomillado;
        QTextCharFormat formatoFuncion;
};
#endif //__RESALTADOR_SINTAXIS_H__
