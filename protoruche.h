#ifndef PROTORUCHE_H
#define PROTORUCHE_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

#include "mesure.h"
#include "envoiconfiguration.h"

class ProtoRuche : public QObject
{
    Q_OBJECT
public:
    ProtoRuche(QObject * parent=0);
    void setTrame(QByteArray TrameTest);

private:
    QUdpSocket * mSocket;
    int mPort;
    bool majHorloge;
    void receptionMesure();
    QByteArray envoiConfiguration(int noRuche);
    void envoyerAcquittement(bool positif);

protected:
    QNetworkDatagram mDatagramme;

public:
signals:
    void mesureRecue(Mesure m);
    void configurationDemandee(int noRuche, Configuration &c);

public slots:
    void recupererDonnees();
};

#endif // PROTORUCHE_H
