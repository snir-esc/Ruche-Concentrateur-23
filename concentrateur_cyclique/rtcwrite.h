#ifndef RTCWrite_H
#define RTCWrite_H
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QDate>
#include <iostream>

using namespace std;

enum wildcard{
     wildcard = 128,
};


class RTCWrite
{

private:
    QProcess mProcess;
    vector<int> ToUtc(int s,int m,int h, int d);


public:
    RTCWrite();
    void clear_startup_time();
    void clear_shutdown_time();
    void set_startup_time(int s,int m, int h,int d);
    void set_shutdown_time(int m,int h, int d);
    void set_next_startup_in(int m, int h);
    void set_next_shutdown_in(int m, int h);
    QDateTime get_startup_time();



};

#endif // RTCWrite_H
