#include <QCoreApplication>
#include "protoruche.h"
#include "traitement.h"
#include "mesure.h"
#include <QObject>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProtoRuche serveur;
    Traitement traitement;
    EnvoiConfiguration configurations;
    Configuration c;
    WittyPi witty;
    c.setParametre(HEURE);                           //
    configurations.ajouterConfiguration(200,c);  //200 est un sous-sytème
    c.setParametre(VEILLE);
    QByteArray v;
    v.append((char)0);
    v.append(60);
    c.setValeur(v);
    configurations.ajouterConfiguration(200,c);
    QObject::connect(&serveur, SIGNAL(mesureRecue(Mesure)), &traitement, SLOT(traiterMesure(Mesure)));
    QObject::connect(&serveur, SIGNAL(configurationDemandee(int,Configuration &)), &configurations, SLOT(fournirConfiguration(int, Configuration &)));
    QObject::connect(&witty, SIGNAL(tensionPret(Mesure)),&traitement,SLOT (traiterMesure(Mesure)));

    witty.MesureTension();
    return a.exec();
}
