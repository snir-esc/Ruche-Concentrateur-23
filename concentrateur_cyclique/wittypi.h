#ifndef WITTYPI_H
#define WITTYPI_H
#include <QProcess>
#include "mesure.h"
#include <QObject>
#include "rtcwrite.h"
#include "envoiconfiguration.h"

class WittyPi : public QObject, public RTCWrite
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
    void Synchrofaite();

public slots:

    void Synchronisation();
    
};

#endif // WITTYPI_H
