#include <QCoreApplication>
#include "protoruche.h"
#include "traitement.h"
#include "mesure.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProtoRuche serveur;
    Traitement traitement;
    EnvoiConfiguration configurations;
    Configuration c;
    c.setParametre(1);
    configurations.ajouterConfiguration(200,c);
    c.setParametre(2);
    QByteArray v;
    v.append((char)0);
    v.append(60);
    c.setValeur(v);
    configurations.ajouterConfiguration(200,c);
    QObject::connect(&serveur, SIGNAL(mesureRecue(Mesure)), &traitement, SLOT(traiterMesure(Mesure)));
    QObject::connect(&serveur, SIGNAL(configurationDemandee(int,Configuration &)), &configurations, SLOT(fournirConfiguration(int, Configuration &)));
    return a.exec();
}
