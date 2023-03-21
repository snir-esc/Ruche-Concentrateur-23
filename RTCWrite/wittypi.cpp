#include "wittypi.h"

WittyPi::WittyPi(QObject *parent) : QObject(parent)
{

}

void WittyPi::set_startup_time(int s,int m ,int h, int d){   // L'écriture se fait au format BCD Donc chaque quartet 0-9

    vector<int> UTCValeurs = ToUtc(s,m,h,d);                // parti de traiment de l'entrée pour les WILDCARD

    if((h==128)&&(d==128))                                    //je rentre une valeur quand même car pour la convertion en UTC il faut une valeur correcte
    {
        UTCValeurs = ToUtc(0,m,15,15);
        m = UTCValeurs[1] ; h = 128; d = 128;
    }
    else if(d == 128)
    {
        UTCValeurs = ToUtc(0,m,h,15);
        m = UTCValeurs[1] ; h = 128; d = 128;
    }
    else if (h ==128)
    {
        UTCValeurs = ToUtc(0,m,15,d);
        m = UTCValeurs[1] ; h = 128; d = UTCValeurs[3];
    }
    else
    {
        UTCValeurs = ToUtc(0,m,h,d);
        m = UTCValeurs[1] ; h = UTCValeurs[2]; d = UTCValeurs[3];
    }


    int TrameSec = 0b00000000;
    int TrameMin = 0b00000000;
    int TrameHeure = 0b00000000;
    int TrameJour=0b00000000;
    int SecDizaine=0;
    int MinDizaine=0;
    QString hexadecimal;
    QString HexString="0x";



    if(s/10>0){                                       // passage au format BCD Trame Secondes
        SecDizaine = s/10;
        TrameSec = SecDizaine << 4;       // décalage de 4 bits (voir registre RTC)
        TrameSec = TrameSec | (s%10);      // on rajoute le reste
    }else{
        TrameSec = s;                           //sinon on prend que l'unite
    }


    if(m/10>0){                                     //Traitement Trame min format BCD
        MinDizaine = m/10;
        TrameMin = MinDizaine << 4 &0b01110000;
        TrameMin = TrameMin | (m%10);
    }else{
        TrameMin = TrameMin | m;
    }

    if(h==128){
        TrameHeure=128;
    }else if((h/10>0)&& (h/10<2)){                                                 // mode 24 heures quand BIT 6 =0      // Traitement Trame Heures format BCD
        TrameHeure = TrameHeure | 0b00010000;     // BIT 4 pour dizaine
        TrameHeure = TrameHeure | (h%10);         // BIT 0-3 pour unite
    }else if(h/20>0){
        TrameHeure = TrameHeure | 0b00100000;    // BIT 5 pour vingt
        TrameHeure = TrameHeure | (h%20);        // on rajoute le reste
    }else{
        TrameHeure = TrameHeure | h;
    }



    if(d==128){                                           //Traitement Trame Jours format BCD
        TrameJour = 128;                // si d=0 alors ?? (peut importe le jour ["wildcard"] dans le doc tech
    }
    else if(d/10>0){
        TrameJour = (TrameJour | (d/10));
        TrameJour = TrameJour <<4 & 0b01110000;
        TrameJour = TrameJour | (d%10);
    }else{
        TrameJour = TrameJour | d;
    }




    //Write Trame Sec
    hexadecimal.clear();
    hexadecimal.setNum(TrameSec,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x07",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les secondes
    mProcess.waitForFinished();


    //Write Trame Min
    hexadecimal.clear();
    hexadecimal.setNum(TrameMin,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x08",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les minutes
    mProcess.waitForFinished();


    //Write trame Heure
    hexadecimal.clear();
    hexadecimal.setNum(TrameHeure,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x09",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les heures
    mProcess.waitForFinished();



    //Write Trame Jour
    hexadecimal.clear();
    hexadecimal.setNum(TrameJour,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0A",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les heures
    mProcess.waitForFinished();

    qDebug()<<"Date startup UTC:"<<endl<<d<<" "<<h<<":"<<m<<":"<<s;


}




void WittyPi::clear_startup_time(){
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x07","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x08","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x09","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0A","0x00"});
    mProcess.waitForFinished();

}




void WittyPi::set_shutdown_time(int m, int h,int d){
    vector<int> UTCValeurs;

    if((h==128)&&(d==128))
    {
        UTCValeurs = ToUtc(0,m,15,15);              //je rentre une valeur quand même car pour la convertion en UTC il faut une valeur correcte
        m = UTCValeurs[1] ; h = 128; d = 128;
    }
    else if(d == 128)
    {
        UTCValeurs = ToUtc(0,m,h,15);
        m = UTCValeurs[1] ; h = 128; d = 128;
    }
    else if (h ==128)
    {
        UTCValeurs = ToUtc(0,m,15,d);
        m = UTCValeurs[1] ; h = 128; d = UTCValeurs[3];
    }
    else
    {
        UTCValeurs = ToUtc(0,m,h,d);
        m = UTCValeurs[1] ; h = UTCValeurs[2]; d = UTCValeurs[3];
    }



    int TrameMin = 0b00000000;
    int TrameHeure = 0b00000000;
    int TrameJour = 0b00000000;
    int MinDizaine=0;
    QString hexadecimal;
    QString HexString="0x";


    if(m/10>0){                                        // passage au format BCD
        MinDizaine = m/10;
        TrameMin = MinDizaine << 4 &0b01110000;         // décalage de 4 bits (voir registre RTC)
        TrameMin = TrameMin | (m%10);                  // on rajoute le reste
    }else{
        TrameMin = TrameMin | m;                        //sinon on prend que l'unite
    }


    if(h==128){
        TrameHeure=128;
    }else if((h/10>0)&& (h/10<2)){                                               // mode 24 heures quand BIT 6 =0
        TrameHeure = TrameHeure | 0b00010000;     // BIT 4 pour dizaine
        TrameHeure = TrameHeure | (h%10);         // BIT 0-3 pour unite
    }else if(h/20>0){
        TrameHeure = TrameHeure | 0b00100000;    // BIT 5 pour vingt
        TrameHeure = TrameHeure | (h%10);        // on rajoute le reste
    }else{
        TrameHeure = TrameHeure | h;
    }


    if(d==128){                                           //Traitement Trame Jours format BCD
        TrameJour = 128;                // si d=0 alors ?? (peut importe le jour ["wildcard"] dans le doc tech
    }
    else if(d/10>0){
        TrameJour = (TrameJour | (d/10));
        TrameJour = TrameJour <<4 & 0b01110000;
        TrameJour = TrameJour | (d%10);
    }else{
        TrameJour = TrameJour | d;
    }


    hexadecimal.clear();
    hexadecimal.setNum(TrameHeure,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0C",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 2 les heures
    mProcess.waitForFinished();



    hexadecimal.clear();
    hexadecimal.setNum(TrameMin,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0B",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 2 les minutes
    mProcess.waitForFinished();


    hexadecimal.clear();
    hexadecimal.setNum(TrameJour,16);
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0D",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les heures
    mProcess.waitForFinished();

    qDebug()<<"Date shutdown UTC:"<<endl<<d<<" "<<h<<":"<<m;

}


void WittyPi::clear_shutdown_time(){
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0B","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0C","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0D","0x00"});
    mProcess.waitForFinished();

}




void WittyPi::set_next_startup_in(int m, int h){

    QDateTime DT;
    QTime Time;
    QDate Date;
    DT = QDateTime::currentDateTime();
    DT = DT.addSecs(((m*60)+(h*3600)));

    Time = DT.time();
    Date = DT.date();

    set_startup_time(Time.msec()/1000,Time.minute(),Time.hour(),Date.day());
}

void WittyPi::set_next_shutdown_in(int m, int h){

    QDateTime DT;
    QTime Time;
    QDate Date;
    DT = QDateTime::currentDateTime();
    DT = DT.addSecs(((m*60)+(h*3600)));

    Time = DT.time();
    Date = DT.date();

    set_shutdown_time(Time.minute(),Time.hour(),Date.day());
}



vector<int> WittyPi::ToUtc(int s,int m,int h, int d){

    QDate Date;
    QTime time(h,m,s);
    QDateTime DT;
    vector<int> vecteur;

    int year = QDate::currentDate().year();
    int month = QDate::currentDate().month();
    Date.setDate(year,month,d);



    DT.setDate(Date);
    DT.setTime(time);
    qint64 MSecfromepoch = DT.toMSecsSinceEpoch();                           //passage en epoch
    QDateTime dtepoch = QDateTime::fromMSecsSinceEpoch(MSecfromepoch);      // depuis le temps en epoch passage en DateTime     [ le fuseau horaire par defaut est celui de la machine ]
    dtepoch = dtepoch.toUTC();                                              // passage en UTC

    time = dtepoch.time();
    Date = dtepoch.date();

    // qDebug()<<dtepoch.time();
    // qDebug()<<dtepoch.date();


    vecteur.push_back(time.msec()/1000);
    vecteur.push_back(time.minute());
    vecteur.push_back(time.hour());
    vecteur.push_back(Date.day());


    return vecteur;
}


