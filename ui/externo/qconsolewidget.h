/*
 * Extraido de https://code.google.com/p/qterminalwidget/
 */

#ifndef QCONSOLEWIDGET_H
#define QCONSOLEWIDGET_H

#include <QTextEdit>
#include <QDir>

#include "redirect.h"

class QConsoleWidget : public QTextEdit
{
    Q_OBJECT
public:
    QConsoleWidget(QWidget *parent = 0);
    ~QConsoleWidget();
private:
    int fixedPosition;
    CRedirect *redirect;

protected:
    void keyPressEvent (QKeyEvent * event);
public slots:
    void OnChildStarted();
    void OnChildStdOutWrite(QString szOutput);
    void OnChildStdErrWrite(QString szOutput);
    void OnChildTerminate();

    void cursorPositionChanged();
};

#endif // QCONSOLEWIDGET_H
