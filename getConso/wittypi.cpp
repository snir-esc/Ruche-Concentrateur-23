#include "wittypi.h"

WittyPi::WittyPi(QObject *parent) : QObject(parent)
{

}
float WittyPi::getTension(){
    float Tension=0;
    float Courant=0;

    QString qString;
    QByteArray Tensionqbyte;
     QByteArray Courantbyte;

    mProcess.start("i2cget", (QStringList){"-y","1","0x69","3"});  // récupération Unité Tension
    mProcess.waitForFinished();                                    //on attend la fin de la commande
    Tensionqbyte = mProcess.readAll();                            // on récupére la sortie, une chaine dans un QByteArray
    qString.append(Tensionqbyte);                                 // Passage en QString pour la conversion
    Tension=(float)qString.toInt(nullptr,16);                         // conversion en Float
    qString.clear();                                              //on supprime tout le contenu de la variable


    mProcess.start("i2cget", (QStringList){"-y","1","0x69","4"});
    mProcess.waitForFinished();
    Tensionqbyte = mProcess.readAll();

    qString.append(Tensionqbyte);                                   //récupération de la partie décimal
    Tension = Tension + (float)(qString.toInt(nullptr,16))/100;





    mProcess.start("i2cget", (QStringList){"-y","1","0x69","5"});  // récupération Unité Tension
    mProcess.waitForFinished();                                    //on attend la fin de la commande
    Courantbyte = mProcess.readAll();                            // on récupére la sortie, une chaine dans un QByteArray
    qString.append(Courantbyte);                                 // Passage en QString pour la conversion
    Courant=(float)qString.toInt(nullptr,16);                         // conversion en Float
    qString.clear();                                              //on supprime tout le contenu de la variable


    mProcess.start("i2cget", (QStringList){"-y","1","0x69","6"});
    mProcess.waitForFinished();
    Courantbyte = mProcess.readAll();

    qString.append(Courantbyte);                                   //récupération de la partie décimal
    Courant = Courant + (float)(qString.toInt(nullptr,16))/100;





    qDebug()<< "Tension"<<Tension<<"V";
    qDebug()<< "Courant"<<Courant<<"A";
    qDebug()<<"Puissance"<<Tension*Courant<<"W";

    return Tension;
}
