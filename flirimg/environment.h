#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QObject>

class environment : public QObject
{
    Q_OBJECT
public:
    explicit environment(QObject *parent = nullptr);
    QString path2convert();
    QString path2exiftool();
signals:

};

#endif // ENVIRONMENT_H
