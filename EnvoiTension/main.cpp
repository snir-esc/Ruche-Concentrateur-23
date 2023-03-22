#include <QCoreApplication>
#include "protoruche.h"
#include "traitement.h"
#include "mesure.h"
#include <QObject>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Traitement traitement;
    WittyPi witty;

    QObject::connect(&witty, SIGNAL(tensionPret(Mesure)),&traitement,SLOT (traiterMesure(Mesure)));

    witty.MesureTension();
    return a.exec();
}
