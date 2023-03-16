#include <QCoreApplication>
#include <QDebug>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WittyPi witty;

    witty.ToUtc(0,47,10,5);
  //  witty.set_startup_time(0,47,10,80);
//    witty.set_shutdown_time(0,0,0);
    return a.exec();
}
