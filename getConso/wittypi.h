#ifndef WITTYPI_H
#define WITTYPI_H
#include <QObject>
#include <QProcess>
#include <QDebug>

class WittyPi : public QObject
{
    Q_OBJECT
private:
    QProcess mProcess;
public:
    WittyPi(QObject *parent = nullptr);
    float getTension();


};

#endif // WITTYPI_H
