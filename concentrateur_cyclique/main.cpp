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
    QByteArray v;

    //c.setParametre(HEURE);
    //configurations.ajouterConfiguration(200,c);  //80 est un sous-sytème
    //    c.setParametre(VEILLE);

    // v.append((char)0);               // car 2 octet pour la durée de veille
    //v.append(60);
    //c.setValeur(v);
    // configurations.ajouterConfiguration(200,c);
    //    c.setParametre(255);            // pour indiquer la fin de paramètrage
    //    v.clear();
    //    c.setValeur(v);                 //0
    //    configurations.ajouterConfiguration(200,c);


    witty.set_config_addr(configurations);
    witty.set_next_startup_in(10,0); //prochain redémarrage du concentrateur
    witty.set_next_shutdown_in(4,0);

    QObject::connect(&serveur, SIGNAL(mesureRecue(Mesure)), &traitement, SLOT(traiterMesure(Mesure)));
    QObject::connect(&serveur, SIGNAL(configurationDemandee(int,Configuration &)), &witty, SLOT(Synchronisation(int, Configuration &)));
    QObject::connect(&serveur, SIGNAL(configurationDemandee(int, Configuration &)), &configurations, SLOT(fournirConfiguration(int, Configuration &)));

    //QObject::connect(&serveur, SIGNAL(configurationDemandee(int,Configuration &)), &configurations, SLOT(fournirConfiguration(int, Configuration &)));
    QObject::connect(&witty, SIGNAL(tensionPret(Mesure)),&traitement,SLOT (traiterMesure(Mesure)));
    
    witty.MesureTension();


    return a.exec();




    /*
    QCoreApplication a(argc, argv);

        QString alerte("Essai de message");
        SimCom7600 simCom;

        QHostAddress brocker("51.75.140.204");
        simCom.setBrokerMQTTAdresse(brocker);
        simCom.setBrokerMQTTPort(1883);
        simCom.setCMNET("free");
        simCom.setPIN("2658");
        if (simCom.init()) {
            simCom.envoyerSMS("0661720601",alerte);
            //simCom.publierMQTT("/test","messageTest");
        }
        simCom.close();
        return a.exec();
    */
}
