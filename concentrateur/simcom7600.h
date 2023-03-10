#ifndef SIMCOM7600_H
#define SIMCOM7600_H

#include <QObject>
#include <QHostAddress>
#include <QSerialPort>

#define EMULATION

#ifdef EMULATION
#include "moquisttoMqtt.h"
#endif

class SimCom7600 : public QObject
{
    Q_OBJECT
public:
    explicit SimCom7600(QObject *parent = nullptr);

    void init();
    void envoyerSMS(QString destinataire, QString message);
    void publierMQTT(QByteArray rubrique, QByteArray message);

    const QByteArray &PIN() const;
    void setPIN(const QByteArray &newPIN);

    const QByteArray &CMNET() const;
    void setCMNET(const QByteArray &newCMNET);

    const QHostAddress &brokerMQTTAdresse() const;
    void setBrokerMQTTAdresse(const QHostAddress &newBrokerMQTTAdresse);

    int brokerMQTTPort() const;
    void setBrokerMQTTPort(int newBrokerMQTTPort);

signals:

private:
    QByteArray mPIN;
    QByteArray mCMNET;
    QSerialPort mPort;
    QHostAddress mBrokerMQTTAdresse;
    int mBrokerMQTTPort;
#ifdef EMULATION
    MosquittoMqtt mMosquitto;
#endif
};

#endif // SIMCOM7600_H
