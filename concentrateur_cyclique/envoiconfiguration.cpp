#include "envoiconfiguration.h"

#include <QDateTime>
#include <QDebug>
Configuration::Configuration() :
    mParametre(255)
{
}

quint8 Configuration::parametre() const
{
    return mParametre;
}

void Configuration::setParametre(quint8 newParametre)
{
    mParametre = newParametre;
}

const QByteArray &Configuration::valeur() const
{
    return mValeur;
}

void Configuration::setValeur(const QByteArray &newValeur)
{
    mValeur = newValeur;
}

//-----------------------------------------------------

EnvoiConfiguration::EnvoiConfiguration(QObject *parent) :
    QObject(parent)
{
}

void EnvoiConfiguration::fournirConfiguration(int noRuche, Configuration &c)
{
    if (mListeConfig.contains(noRuche) && mListeConfig[noRuche].length()>0) {
        c = mListeConfig[noRuche].constFirst();
        mListeConfig[noRuche].removeFirst();
        if (c.parametre()==1) { // définir l'heure
            quint64 epoch = QDateTime::currentSecsSinceEpoch();

            QByteArray t;
            t.push_back( (epoch >> 24) & 0xff);
            t.push_back( (epoch >> 16) & 0xff);
            t.push_back( (epoch >>  8) & 0xff);
            t.push_back( (epoch >>  0) & 0xff);
            c.setValeur(t);
            qDebug()<<epoch<<"s depuis epoch";
        }
    } else {  // marque la fin des envois de configuration
        c.setParametre(255);
        c.setValeur(QByteArray());
    }
}

void EnvoiConfiguration::ajouterConfiguration(int noRuche, Configuration c)
{
    mListeConfig[noRuche].append(c);
}
