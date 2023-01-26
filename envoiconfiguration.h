#ifndef ENVOICONFIGURATION_H
#define ENVOICONFIGURATION_H

#include <QObject>
#include <QMap>
#include <QVector>

#define MAX_PARAMETER_SIZE 16

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
