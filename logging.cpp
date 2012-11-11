#include "logging.h"

Logging::Logging(QTextEdit *te) :
    QObject(), textEdit(te)
{
}

void Logging::log(QString text)
{
    textEdit->insertPlainText(text + "\n");
}
