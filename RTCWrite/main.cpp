#include <QCoreApplication>
#include <QDebug>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WittyPi witty;

    witty.set_startup_time(0,45,5,2);
//    witty.set_shutdown_time(0,0,0);
    return a.exec();
}
