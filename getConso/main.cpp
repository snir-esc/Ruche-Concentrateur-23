#include <QCoreApplication>
#include <QDebug>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WittyPi witty;

    witty.getTension();
    return a.exec();
}
