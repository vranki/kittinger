#include "logging.h"

Logging::Logging() :
    QObject(), textEdit(0)
{
}

void Logging::setTextEdit(QTextEdit *te) {
    textEdit = te;
}

void Logging::log(QString text)
{
    textEdit->insertPlainText(sender()->objectName() + ": " + text + "\n");
    textEdit->ensureCursorVisible();
}
