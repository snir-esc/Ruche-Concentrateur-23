#ifndef MESURE_H
#define MESURE_H

#include <QDateTime>

class Mesure
{
public:
    Mesure();
    int capteur() const;
    void setCapteur(int newCapteur);

    const QDateTime &dateHeure() const;
    void setDateHeure(const QDateTime &newDateHeure);
    void setDateHeure(const char* fromFrame);

    float valeur() const;
    void setValeur(float newValeur);
    void setValeur(const char* fromFrame);

    QString toString();

private:
    int mCapteur;
    QDateTime mDateHeure;
    float mValeur;
};

#endif // MESURE_H
