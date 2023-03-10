#ifndef WITTYPI_H
#define WITTYPI_H
#include <QProcess>
#include "mesure.h"
#include <QObject>


class WittyPi : public QObject
{
    Q_OBJECT
private:
    QProcess mProcess;
public:
    WittyPi(QObject *parent = nullptr);
    void MesureTension();
    float getTension();

signals:
    void tensionPret(Mesure);

};

#endif // WITTYPI_H
