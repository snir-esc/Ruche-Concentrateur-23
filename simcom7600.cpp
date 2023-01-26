#include "simcom7600.h"

#ifdef DEBUG_SEUL

SimCom7600::SimCom7600(QObject *parent) : QObject(parent)
{

}

void SimCom7600::envoyerSMS(QString destinataire, QString message)
{
    qDebug() << "envoi de SMS vers " << destinataire;
    qDebug() << "message:" << message;
}

void SimCom7600::publierMQTT(QByteArray rubrique, QByteArray message)
{
    qDebug() << "publication vers le broker " << mBrockerMQTTAdresse.toString() << ":" << mBrockerMQTTPort;
    qDebug() << "rubrique:" << rubrique;
    qDebug() << "message:" << message;
}

#endif
#ifdef EMULATION

SimCom7600::SimCom7600(QObject *parent) :
    QObject(parent),
    mMosquitto(parent)
{
}

void SimCom7600::init()
{
    int keepalive=60;
    mMosquitto.connect(mBrokerMQTTAdresse.toString().toStdString().c_str(), mBrokerMQTTPort, keepalive);
    mMosquitto.start();
}

void SimCom7600::envoyerSMS(QString destinataire, QString message)
{
    qDebug() << "envoi de SMS vers " << destinataire;
    qDebug() << "message:" << message;
    QByteArray rubrique;
    rubrique.append("SMS/").append(destinataire);
    QByteArray mqttMessage;
    mqttMessage.append(message);
    mMosquitto.publish(rubrique,mqttMessage);
}

void SimCom7600::publierMQTT(QByteArray rubrique, QByteArray message)
{
    qDebug() << "publication vers le broker " << mBrokerMQTTAdresse.toString() << ":" << mBrokerMQTTPort;
    qDebug() << "rubrique:" << rubrique;
    qDebug() << "message:" << message;
    mMosquitto.publish(rubrique,message);
}
#endif

const QByteArray &SimCom7600::CMNET() const
{
    return mCMNET;
}

void SimCom7600::setCMNET(const QByteArray &newCMNET)
{
    mCMNET = newCMNET;
}

const QHostAddress &SimCom7600::brokerMQTTAdresse() const
{
    return mBrokerMQTTAdresse;
}

void SimCom7600::setBrokerMQTTAdresse(const QHostAddress &newBrokerMQTTAdresse)
{
    mBrokerMQTTAdresse = newBrokerMQTTAdresse;
}

int SimCom7600::brokerMQTTPort() const
{
    return mBrokerMQTTPort;
}

void SimCom7600::setBrokerMQTTPort(int newBrokerMQTTPort)
{
    mBrokerMQTTPort = newBrokerMQTTPort;
}

const QByteArray &SimCom7600::PIN() const
{
    return mPIN;
}

void SimCom7600::setPIN(const QByteArray &newPIN)
{
    mPIN = newPIN;
}
