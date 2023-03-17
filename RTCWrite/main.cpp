#include <QCoreApplication>
#include <QDebug>
#include "wittypi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    WittyPi witty;

    witty.set_startup_time(00,41,wildcard,wildcard);
    witty.set_shutdown_time(40,wildcard,wildcard);
    return a.exec();
}
