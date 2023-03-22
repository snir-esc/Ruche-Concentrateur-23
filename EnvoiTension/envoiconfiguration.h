#ifndef ENVOICONFIGURATION_H
#define ENVOICONFIGURATION_H

#include <QObject>
#include <QMap>
#include <QVector>

#define MAX_PARAMETER_SIZE 16

enum parametres {
    /*! \brief Heure */
    HEURE=1,

    /*! \brief Durée veille (en s) */
    VEILLE=2,

    /*! \brief nombre de mesures par cycle */
    CYCLE=3,

    /*! \brief ip ( pour le prochain cycle) */
    IP=4,

    /*! \brief ip concentrateur ( pour le prochain cycle) */
    IP_CONCENTRATEUR=5,

    SSID=6,
    KEY=7

};


class Configuration {
public:
    Configuration();
    const QByteArray &valeur() const;
    void setValeur(const QByteArray &newValeur);
    quint8 parametre() const;
    void setParametre(quint8 newParametre);
private:
    quint8 mParametre;
    QByteArray mValeur;
};



class EnvoiConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit EnvoiConfiguration(QObject *parent = nullptr);

signals:

public slots:
    void fournirConfiguration(int noRuche, Configuration & c);
    void ajouterConfiguration(int noRuche, Configuration c);

private:
    QMap<int,QVector<Configuration>> mListeConfig;
};

#endif // ENVOICONFIGURATION_H
