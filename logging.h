#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>

class QTextEdit;

class Logging : public QObject
{
    Q_OBJECT
public:
    explicit Logging();
    void setTextEdit(QTextEdit *te);
signals:
public slots:
    void log(QString text);
private:
    QTextEdit *textEdit;
};

#endif // LOGGING_H
