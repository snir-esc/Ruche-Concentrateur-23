#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include <QObject>
#include "mesure.h"
#include "simcom7600.h"

class Traitement : public QObject
{
    Q_OBJECT
public:
    explicit Traitement(QObject *parent = nullptr);

signals:

public slots:
    void traiterMesure(Mesure m);

private:
    SimCom7600 mSimCom;
    QString detecterAlerte(Mesure m);
};

#endif // TRAITEMENT_H
