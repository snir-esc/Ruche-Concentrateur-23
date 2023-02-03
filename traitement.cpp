#include "traitement.h"
#include <QDebug>

Traitement::Traitement(QObject *parent) : QObject(parent)
{
    QHostAddress ad("192.168.61.231");
    mSimCom.setBrokerMQTTAdresse(ad);
    mSimCom.setBrokerMQTTPort(1883);
    mSimCom.init();
}

void Traitement::traiterMesure(Mesure m)
{
     qDebug() << "Mesure traitée : \n" << m.toString();

     QByteArray codeCapteur;
     switch (m.capteur()) {
     case 1:
         codeCapteur="BIN";  // Batterie intérieure
             break;
     case 2:
         codeCapteur="BPE";  // Batterie pesée
             break;
     case 3:
         codeCapteur="THC";  // Température hors couvain
         break;
     case 4:
         codeCapteur="HYG";  // Batterie intérieure
         break;
     case 5:
         codeCapteur="MAS";  // Poids de la ruche
         break;
     case 6:
         codeCapteur="I1A";  //  Intercadre
         break;
     case 7:
         codeCapteur="I1B";  //  Intercadre
         break;
     case 8:
         codeCapteur="I2A";  //  Intercadre
         break;
     case 9:
         codeCapteur="I2B";  //  Intercadre
         break;
     case 10:
         codeCapteur="I3A";  //  Intercadre
         break;
     case 11:
         codeCapteur="I3B";  //  Intercadre
         break;
     case 12:
         codeCapteur="I4A";  //  Intercadre
         break;
     case 13:
         codeCapteur="I4B";  //  Intercadre
         break;
     case 14:
         codeCapteur="I5A";  //  Intercadre
         break;
     case 15:
         codeCapteur="I5B";  //  Intercadre
         break;
     case 16:
         codeCapteur="I6A";  //  Intercadre
         break;
     case 17:
         codeCapteur="I6B";  //  Intercadre
         break;
     case 18:
         codeCapteur="I7A";  //  Intercadre
         break;
     case 19:
         codeCapteur="I7B";  //  Intercadre
         break;
     case 20:
         codeCapteur="I8A";  //  Intercadre
         break;
     case 21:
         codeCapteur="I8B";  //  Intercadre
         break;
     default:
         codeCapteur="ERR";
         break;
     }
     QByteArray rubrique;
     rubrique.append("rucher/");
     rubrique.append("1");
     rubrique.append("/");
     rubrique.append(codeCapteur);

     QByteArray message;
     // format du message MQTT :
     // YYYYMMDD-HHMMSS:valeur
     message.append(m.dateHeure().toString("yyyyMMdd-hhmmss"));
     message.append(":");
     message.append(QString("%1").arg(m.valeur()));
     mSimCom.publierMQTT(rubrique,message);

     QString alerte = detecterAlerte(m);
     if (alerte.size() > 0) {
         mSimCom.envoyerSMS("06xxxxxxxx",alerte);
     }
}

QString Traitement::detecterAlerte(Mesure m)
{
    switch(m.capteur()) {
    case 1: // batterie
        if (m.valeur()<3) {
            return QString("Batterie intérieure faible");
        }
        break;
    case 3: // température hors couvain
        if (m.valeur()<0) {
            return QString("Température hors couvain négative");
        }
        break;
    }
    return QString("");
}
