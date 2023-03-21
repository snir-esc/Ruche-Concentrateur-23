#include <QCoreApplication>
#include <QDebug>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WittyPi witty;


    witty.set_next_startup_in(2,0);
    witty.set_next_shutdown_in(1,0);
    return a.exec();
}
