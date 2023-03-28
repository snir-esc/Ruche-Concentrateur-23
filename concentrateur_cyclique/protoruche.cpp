#include "protoruche.h"
#include <stdio.h>
#include <QNetworkDatagram>
#include <QThread>

#include "mesure.h"

ProtoRuche::ProtoRuche(QObject *parent) :
    QObject(parent),
    mPort(20123)
{    
    //On crée la socket
    mSocket = new QUdpSocket(this);

    //signal reçu par socket déclenche slot données reçues
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(recupererDonnees()));

    //On attribue l'adresse et le port à la socket
    bool result = mSocket->bind(QHostAddress::AnyIPv4, mPort);

    if(!result)
    {
        qDebug() << "CONNEXION_FAIL";
        return;
    }
    else qDebug() << "SOCKET_CREEE";
}

void ProtoRuche::setTrame(QByteArray TrameTest)
{
    mDatagramme.setData(TrameTest);
    //emit ProtoRuche::DonneesPretes();
}

void ProtoRuche::receptionMesure()
{
    Mesure m;
    m.setCapteur(mDatagramme.data().at(1));
    m.setDateHeure(mDatagramme.data().constData()+2);
    m.setValeur(mDatagramme.data().constData()+6);
    qDebug() << "Mesure reçue : \n" << m.toString();
    emit mesureRecue(m);

}

QByteArray ProtoRuche::envoiConfiguration(int noRuche)
{
    Configuration config;

    emit configurationDemandee(noRuche,config);
    if (config.valeur().size() > MAX_PARAMETER_SIZE) config.setValeur(QByteArray());
    QByteArray t;
    t.push_back(0xff);
    t.push_back(config.parametre());

    t.push_back(config.valeur().size());
    t.append(config.valeur());

    uint8_t checksum = 0;
    for (int i=0;i<t.size(); i++) checksum += t.at(i);
    t.push_back(checksum);
    return t;
}

void ProtoRuche::envoyerAcquittement(bool positif)
{
    QByteArray trameAcquittement;
    if (positif) trameAcquittement.append((char)0xff);
    else trameAcquittement.append((char)0x00);
    mSocket->writeDatagram(mDatagramme.makeReply(trameAcquittement));
}

void ProtoRuche::recupererDonnees()
{
    while (mSocket->hasPendingDatagrams()) {
        qDebug() << "Récupération de la trame";
        mDatagramme = mSocket->receiveDatagram();
        qDebug() << "Trame : ";
        qDebug() << mDatagramme.data().toHex(' ');
        if (mDatagramme.data().at(0) != char(0xff)) {
            qDebug() << "Erreur de format : octet 0 = " << mDatagramme.data().at(0) << " ne vaut pas 0xff";
            envoyerAcquittement(false);
            continue;
        }

        uint8_t checksum = 0;
        for (int i=0 ; i<mDatagramme.data().size(); i++) checksum += mDatagramme.data().at(i);
        if (mDatagramme.data().at(0)!=char(0xff)) {
            qDebug() << "Erreur de somme de contrôle : reçu = " << mDatagramme.data().end() << " / calculé = " << checksum;
            envoyerAcquittement(false);
            continue;
        }

        if (mDatagramme.data().at(1)==0) {
            qDebug() << "Envoi de configuration";
            int noRuche = mDatagramme.senderAddress().toIPv4Address() & 0xff;
            QByteArray t = envoiConfiguration(noRuche);
            QNetworkDatagram reply = mDatagramme.makeReply(t);
            qDebug() << "Envoi à " << mDatagramme.senderAddress().toString() << ":" << mDatagramme.senderPort()
                     << " de : " << t.toHex(' ');
            mSocket->writeDatagram(reply);
        } else {
            qDebug() << "Réception de mesures";
            receptionMesure();
            envoyerAcquittement(true);
        }
    }
}
