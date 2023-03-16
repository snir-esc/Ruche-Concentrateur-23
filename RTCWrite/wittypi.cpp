#include "wittypi.h"

WittyPi::WittyPi(QObject *parent) : QObject(parent)
{

}

void WittyPi::set_startup_time(int s,int m ,int h, int d){   // L'écriture se fait au format BCD Donc chaque quartet 0-9

    int TrameSec = 0b00000000;
    int TrameMin = 0b00000000;
    int TrameHeure = 0b00000000;
    int TrameJour=0b00000000;
    int SecDizaine=0;
    int MinDizaine=0;
    QString hexadecimal;
    QString HexString="0x";

    if(s/10>0){                                       // passage au format BCD
        SecDizaine = s/10;
        TrameSec = SecDizaine << 4;       // décalage de 4 bits (voir registre RTC)
        TrameSec = TrameSec | (s%10);      // on rajoute le reste
    }else{
        TrameSec = s;                           //sinon on prend que l'unite
    }


    if(m/10>0){
        MinDizaine = m/10;
        TrameMin = MinDizaine << 4 &0b01110000;
        TrameMin = TrameMin | (m%10);
    }else{
        TrameMin = TrameMin | m;
    }



    if((h/10>0)&& (h/10<2)){                                               // mode 24 heures quand BIT 6 =0
        TrameHeure = TrameHeure | 0b00010000;     // BIT 4 pour dizaine
        TrameHeure = TrameHeure | (h%10);         // BIT 0-3 pour unite
    }else if(h/20>0){
        TrameHeure = TrameHeure | 0b00100000;    // BIT 5 pour vingt
        TrameHeure = TrameHeure | (h%20);        // on rajoute le reste
    }else{
        TrameHeure = TrameHeure | h;
    }


    if(d==0){
        TrameJour = 128;
    }
    else if(d/10>0){
        TrameJour = (TrameJour | (d/10));
        TrameJour = TrameJour <<4 & 0b01110000;
        TrameJour = TrameJour | (d%10);
    }else{
        TrameJour = TrameJour | d;
    }

    qDebug()<<TrameSec;
    qDebug()<<TrameMin;
    qDebug()<<TrameHeure;
    qDebug()<<TrameJour;


    //Write Trame Sec
    hexadecimal.clear();
    hexadecimal.setNum(TrameSec,16);
    qDebug()<<"hexa Trame Sec"<<HexString+hexadecimal;
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x07",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les secondes
    mProcess.waitForFinished();


    //Write Trame Min
    hexadecimal.clear();
    hexadecimal.setNum(TrameMin,16);
    qDebug()<<"hexa Trame Min"<<HexString+hexadecimal;
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x08",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les minutes
    mProcess.waitForFinished();


    //Write trame Heure
    hexadecimal.clear();
    hexadecimal.setNum(TrameHeure,16);
    qDebug()<<"hexa Trame Heure"<<HexString+hexadecimal;
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x09",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les heures
    mProcess.waitForFinished();



    //Write Trame Jour
    hexadecimal.clear();
    hexadecimal.setNum(TrameJour,16);
    qDebug()<<"hexa Trame Jour"<<HexString+hexadecimal;
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0A",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les heures
    mProcess.waitForFinished();

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



    if((h/10>0)&& (h/10<2)){                                               // mode 24 heures quand BIT 6 =0
        TrameHeure = TrameHeure | 0b00010000;     // BIT 4 pour dizaine
        TrameHeure = TrameHeure | (h%10);         // BIT 0-3 pour unite
    }else if(h/20>0){
        TrameHeure = TrameHeure | 0b00100000;    // BIT 5 pour vingt
        TrameHeure = TrameHeure | (h%10);        // on rajoute le reste
    }else{
        TrameHeure = TrameHeure | h;
    }



    if(d/10>0){
        TrameJour = (TrameJour | (d/10));
        TrameJour = TrameJour <<4 & 0b01110000;
        TrameJour = TrameJour | (d%10);
    }else{
        TrameJour = TrameJour | d;
    }



        hexadecimal.clear();
        hexadecimal.setNum(TrameHeure,16);
        mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0B",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 2 les heures
        mProcess.waitForFinished();




        hexadecimal.clear();
        hexadecimal.setNum(TrameMin,16);
        mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0C",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 2 les minutes
        mProcess.waitForFinished();



        hexadecimal.clear();
        hexadecimal.setNum(TrameJour,16);
        mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0D",HexString+hexadecimal});  // écriture sur le repertoire de l'alarme 1 les heures
        mProcess.waitForFinished();



}


void WittyPi::clear_shutdown_time(){
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0B","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0C","0x00"});
    mProcess.waitForFinished();
    mProcess.start("i2cset", (QStringList){"-y","1","0x68","0x0D","0x00"});
    mProcess.waitForFinished();

}


