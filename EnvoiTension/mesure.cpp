#include "mesure.h"
#include "time.h"

Mesure::Mesure()
{
}

int Mesure::capteur() const
{
    return mCapteur;
}

void Mesure::setCapteur(int newCapteur)
{
    mCapteur = newCapteur;
}

const QDateTime &Mesure::dateHeure() const
{
    return mDateHeure;
}

void Mesure::setDateHeure(const QDateTime &newDateHeure)
{
    mDateHeure = newDateHeure;
}

void Mesure::setDateHeure(const char *fromFrame)
{
    quint64 epoch =
            (static_cast<quint64>(fromFrame[0]) & 0xff) <<24 |
            (static_cast<quint64>(fromFrame[1]) & 0xff) <<16 |
            (static_cast<quint64>(fromFrame[2]) & 0xff) << 8 |
            (static_cast<quint64>(fromFrame[3]) & 0xff);
    mDateHeure.setSecsSinceEpoch(epoch);
}

float Mesure::valeur() const
{
    return mValeur;
}

void Mesure::setValeur(float newValeur)
{
    mValeur = newValeur;
}

void Mesure::setValeur(const char *fromFrame)
{
    mValeur = 0.1 * ( (fromFrame[0] & 0xff) << 8 |
                      (fromFrame[1] & 0xff)
                    );
}

QString Mesure::toString()
{
    return QString("Capteur : %1   Date : %2  Valeur : %3").arg(mCapteur).arg(mDateHeure.toString("yyyy/MM/dd hh:mm:ss.zzz")).arg(mValeur,0,'f',1);
}
