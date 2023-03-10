#include "wittypi.h"
#include <QDebug>
WittyPi::WittyPi(QObject *parent)
{

}

float WittyPi::getTension(){
    float Tension=0;
    QString qString;
    QByteArray Tensionqbyte;

    mProcess.start("i2cget", (QStringList){"-y","1","0x69","1"});  // récupération Unité Tension
    mProcess.waitForFinished();                                    //on attend la fin de la commande
    Tensionqbyte = mProcess.readAll();                            // on récupére la sortie, une chaine dans un QByteArray
    qString.append(Tensionqbyte);                                 // Passage en QString pour la conversion
    Tension=(float)qString.toInt(nullptr,16);                         // conversion en Float
    qString.clear();                                              //on supprime tout le contenu de la variable


    mProcess.start("i2cget", (QStringList){"-y","1","0x69","2"});
    mProcess.waitForFinished();
    Tensionqbyte = mProcess.readAll();

    qString.append(Tensionqbyte);                                   //récupération de la partie décimal
    Tension = Tension + (float)(qString.toInt(nullptr,16))/100;


    qDebug()<< Tension;
    return Tension;
}

void WittyPi::MesureTension(){                          // créer un object de type Mesure pour ensuite effectuer le traitement par la classe Traitement
  QDateTime DateHeure= DateHeure.currentDateTime();
  Mesure m;
  m.setCapteur(22);
  m.setValeur(getTension());
  m.setDateHeure(DateHeure);
  emit tensionPret(m);                                  // emet un signal avec l'objet contenant les valeurs nécessaire pour le traitement
}

