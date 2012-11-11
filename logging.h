#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>
#include <QtGui/QTextEdit>

class Logging : public QObject
{
    Q_OBJECT
public:
    explicit Logging(QTextEdit *te);
    void log(QString text);
signals:
    
public slots:
private:
    QTextEdit *textEdit;
};

#endif // LOGGING_H
