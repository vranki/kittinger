#include "logging.h"
#include <QDebug>
#include <QtGui/QTextEdit>

Logging::Logging() : QObject(), textEdit(0) {
}

void Logging::setTextEdit(QTextEdit *te) {
    textEdit = te;
}

void Logging::log(QString text)
{
    QString logline = sender()->objectName() + ": " + text;
    if(textEdit) {
        textEdit->insertPlainText(logline + "\n");
        textEdit->ensureCursorVisible();
    }
    qDebug() << Q_FUNC_INFO << logline;
}
