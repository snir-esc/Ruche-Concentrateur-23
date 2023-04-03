#ifndef WITTYPI_H
#define WITTYPI_H
#include <QProcess>
#include "mesure.h"
#include <QObject>
#include "rtcwrite.h"
#include "envoiconfiguration.h"
#include <QMap>

class WittyPi : public QObject, public RTCWrite
{
    Q_OBJECT
private:
    QProcess mProcess;
    EnvoiConfiguration * mConfiguration;
    QMap<int,bool> mNoRucheLastconfig;

public:
    WittyPi(QObject *parent = nullptr);
    void MesureTension();
    float getTension();
    void set_config_addr(EnvoiConfiguration &configuration);

signals:
    void tensionPret(Mesure);

public slots:

    void Synchronisation(int noRuche, Configuration &c);
    
};

#endif // WITTYPI_H
