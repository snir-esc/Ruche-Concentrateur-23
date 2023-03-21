#include "simcom7600.h"
#include "qcoreapplication.h"
#include <QTimer>

#if defined(DEBUG_SEUL)

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

#elif defined(EMULATION)

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
#else

SimCom7600::SimCom7600(QObject *parent) : QObject(parent),
    mPIN("0000"),
    mCMNET("free/orange/xxx")
{

}

bool SimCom7600::init()
{
    QString cmd;
    int reponse;

    if (this->mBrokerMQTTAdresse.toString()=="") return false;

    mPort.setBaudRate(115200);
    mPort.setPortName("ttyS0"); // definir le type de port
    mPort.open(QIODevice::ReadWrite); // On ouvre la connexion


    QStringList errorOrOk;
    errorOrOk.append("ERROR\r\n");
    errorOrOk.append("OK\r\n");

    envoieCommande("AT\r"); // on ecrit le message avec la méthode write
    verifieReponse("OK\r\n"); // On verfie que la carte renvoie bien le message OK

    envoieCommande("ATE0\r"); // supprimer l'echo
    verifieReponse("OK\r\n");

    // vérifier le besoin de d'envoyer le PIN
    envoieCommande("AT+CPIN?\r\n");

    QStringList lr;
    lr.append("+CPIN: SIM PIN\r\n");  //0
    lr.append("+CPIN: READY\r\n");    //1
    lr.append("OK\r\n");              //2

    reponse = verifieReponse(lr);
    if (reponse<1) {
        // On écrit la commande AT+CPIN=XXXX qui permet activer la carte SIM qui se trouve dans la carte 4G HAT
        cmd = QString("AT+CPIN=%1\r").arg(QString(mPIN));
        envoieCommande(cmd.toStdString().c_str());
        //réponses possibles :
        // +CME ERROR: incorrect password\r\n
        // OK
        verifieReponse("OK\r\n");// On vérifie qu'on reçoit bien OK sinon on reçoit Erreur
        verifieReponse("+CPIN: READY\r\n");// On vérifie qu'on reçoit bien +CPIN: READY sinon on reçoit Erreur ( +CPIN: READY veut dire que la carte SIM est prêt
        verifieReponse("SMS DONE\r\n",30000); //On vérifie qu'on reçoit bien SMS DONE ; prend du temps !
        verifieReponse("PB DONE\r\n",30000);  //On vérifie qu'on reçoit bien PB DONE
    }

    //verifie la connexion
    //AT+CREG?
    //renvoie
    //+CGREG: 0,1


   //definit le CMNET de l'opérateur de la carte SIM

    //cmd = QString("AT+CGSOCKCONT=1,\"IP\",\"%1\"\r").arg(QString(mCMNET));
    cmd = QString("AT+CGDCONT=1,\"IP\",\"%1\"\r").arg(QString(mCMNET));
    envoieCommande(cmd);
    reponse = verifieReponse(errorOrOk);  //on vérifie si on reçoit bien OK
    if ( reponse<1 ) return false;

    envoieCommande("AT+NETOPEN\r");
    QStringList reponsesNetOpen;
    reponsesNetOpen.append("ERROR\r\n");  //0
    reponsesNetOpen.append("OK\r\n");  //1
    reponsesNetOpen.append("+NETOPEN: 0\r\n");  //2
    reponse = verifieReponse(reponsesNetOpen,10000);  //on vérifie si on reçoit bien OK
    if ( reponse<1 ) return false;

    int nbTentative=0;
    do {
        nbTentative++;
        envoieCommande("AT+CMQTTSTART\r"); //On démarre le service MQTT avec la commande AT+CMQTTSTART

        QStringList reponsesMqttStart;
        reponsesMqttStart.append("ERROR\r\n");  //0
        reponsesMqttStart.append("OK\r\n");  //1
        reponsesMqttStart.append("+CMQTTSTART: 0\r\n");  //2

        reponse = verifieReponse(reponsesMqttStart,60000);// On vérifie si on reçoit bien +CMQTTSTART : 0

        if (reponse<0) return false;
        if (reponse==0) {
            // On tente d'arrêter le service MQTT pour le redémarrer
            envoieCommande("AT+CMQTTSTOP\r");
            verifieReponse("+CMQTTSTOP: 0\r\n",10000);// On vérifie si on reçoit bien +CMQTTSTOP : 0
        }
    } while (reponse<1 && nbTentative<2);

    envoieCommande("AT+CMQTTACCQ=0,\"clientN1\"\r"); // On acquiert un client
    verifieReponse("OK\r\n"); // On vérifie si on reçoit bien OK sinon on reçoit Erreur

    // On se connecte au serveur MQTT
    cmd = QString("AT+CMQTTCONNECT=0,\"tcp://%1:%2\",60,1\r")
            .arg(mBrokerMQTTAdresse.toString())
            .arg(mBrokerMQTTPort);
    envoieCommande(cmd.toStdString().c_str());
    verifieReponse("OK\r\n"); // On vérifie si on reçoit bien OK sinon on reçoit Erreur
    //verifieReponse("+CMQTTCONNECT: 0,0\r\n"); // On vérifie si on reçoit bien +CMQTTCONNECT: 0,0 sinon on reçoit Erreur
    return true;
}

void SimCom7600::close()
{
    envoieCommande("AT+CMQTTDISC=0,120\r"); // On se déconnecte du serveur
    verifieReponse("+CMQTTDISC: 0,0\r\n");
    verifieReponse("OK\r\n");

    envoieCommande("AT+CMQTTREL=0\r"); // On libère le client
    verifieReponse("OK\r\n");

    envoieCommande("AT+CMQTTSTOP\r"); // On arrête le service MQTT
    verifieReponse("+CMQTTSTOP: 0\r\n");
    verifieReponse("OK\r\n");

    mPort.close();
}

void SimCom7600::envoyerSMS(QString destinataire, QString message)
{
    qDebug() << "envoi de SMS vers " << destinataire;
    qDebug() << "message:" << message;

    envoieCommande("AT\r");
    verifieReponse("OK\r\n");

    envoieCommande("AT+CSCS=\"GSM\"\r");// La commande AT permet de définir le type de message
    verifieReponse("OK\r\n");

    //On écrit la commande AT+CMGS="numero de téléphone"
    // Cette commande permet d'envoyer un message au numéro qui se trouve entre ""
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    QString cmd = QString("AT+CMGS=\"%1\"\r").arg(destinataire);
    envoieCommande(cmd.toStdString().c_str());

    attendPrompt("> "); //On vérifie qu'on reçoit bien le prompt >+espace

    // On écrit le message à envoyer
    envoieCommande(message.toStdString().c_str());
    envoieCommande("\x1A"); // Termine le message avec le code ASCII SUB (=CTRL+Z)
    verifieReponse("+CMGS : \r\n"); //On vérifie qu'on reçoit bien +CMGS: sinon on reçoit Erreur
    verifieReponse("OK\r\n");
}

void SimCom7600::publierMQTT(QByteArray rubrique, QByteArray message)
{
    qDebug() << "publication vers le broker " << mBrokerMQTTAdresse.toString() << ":" << mBrokerMQTTPort;
    qDebug() << "rubrique:" << rubrique;
    qDebug() << "message:" << message;

    QString cmd;

    cmd = QString("AT+CMQTTTOPIC=0,%1\r").arg(rubrique.length());
    envoieCommande(cmd.toStdString().c_str());
    attendPrompt("> "); //On vérifie qu'on reçoit bien >+espace sinon on reçoit Erreur
    envoieCommande(rubrique);
    //envoieCommande("\x03");
    verifieReponse("OK\r\n");

    // On définit le message a envoyer
    cmd = QString("AT+CMQTTPAYLOAD=0,%1\r").arg(message.length());
    envoieCommande(cmd.toStdString().c_str());
    attendPrompt("> "); //On vérifie qu'on reçoit bien >+espace sinon on reçoit Erreur
    envoieCommande(message);
    //envoieCommande("\x03");
    verifieReponse("OK\r\n"); // On vérifie si on reçoit bien OK sinon on reçois Erreur

    envoieCommande("AT+CMQTTPUB=0,1,60\r"); // La commande AT+CMQTTPUB permet de publier le message sur le serveur
    verifieReponse("OK\r\n"); // On vérifie si on reçoit bien OK sinon on reçois Erreur
    verifieReponse("+CMQTTPUB: 0,0\r\n"); //On vérifie si on reçois bien +CMQTTPUB: 0,0 sinon on reçoit Erreur
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

void SimCom7600::envoieCommande(QString cmd)
{
    qDebug() << QString(">>") << cmd;
    mPort.write(cmd.toStdString().c_str());
}


int SimCom7600::verifieReponse(QStringList reponsesAttendues, int timeout)
{
    QByteArray resultatLecture;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.start(timeout);

    while( (! reponsesAttendues.contains(resultatLecture)) && timeoutTimer.remainingTime()>0) {
        while( (!mPort.canReadLine())  && timeoutTimer.remainingTime()>0 ) {
            QCoreApplication::processEvents();
        }

        resultatLecture = mPort.readLine(); // La méthode readLine permet écrire quelque chose sur le port série

        if (resultatLecture.length()>0) { // si la longueur est > 0 alors on affiche le résultat
            qDebug() << QString(resultatLecture); // on affiche le resultat
        }

        if (reponsesAttendues.contains(resultatLecture)) {
            qDebug() <<  QString("<<Détecté option n°%1").arg(reponsesAttendues.indexOf(resultatLecture)); // On affiche le résultat quand la commande est correcte
            return reponsesAttendues.indexOf(resultatLecture);
        }
        QCoreApplication::processEvents();
    }
    qDebug() << QString("<<Timeout !");
    return -1;
}

int SimCom7600::verifieReponse(QString reponseAttendue, int timeout)
{
    return this->verifieReponse(QStringList(reponseAttendue),timeout);
}

bool SimCom7600::attendPrompt(QString prompt, int timeout)
{
    QString resultatLecture;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.start(timeout);

    while(!resultatLecture.endsWith(prompt) && timeoutTimer.remainingTime()>0) {

        int size = prompt.length();
        if (size>mPort.bytesAvailable()) size=mPort.bytesAvailable();
        resultatLecture.append(mPort.read(size));

        if (size>0) {
            qDebug() << QString(resultatLecture); // on affiche le resultat
        }

        if (resultatLecture.endsWith(prompt)) {
            qDebug() << QString("<<Prompt reçu");// On affiche le résultat c'est bon quand la commande est correcte
            return true;
        }
        QCoreApplication::processEvents();
    }
    qDebug() << QString("<<Prompt non reçu !");
    return false;
}

const QByteArray &SimCom7600::PIN() const
{
    return mPIN;
}

void SimCom7600::setPIN(const QByteArray &newPIN)
{
    mPIN = newPIN;
}
