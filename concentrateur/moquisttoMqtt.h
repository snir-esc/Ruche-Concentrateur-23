#ifndef MOSQUITTOMQTT_H
#define MOSQUITTOMQTT_H
#include <QThread>
#include <mosquitto.h>

class MosquittoMqtt : public QThread
{
    Q_OBJECT
public:
    MosquittoMqtt(QObject *parent = NULL);
    virtual ~MosquittoMqtt();

    void run() override;
    int connect(const char * host, int port, int keepalive);
    void disconnect();
    void setQos(int qos);
    int publish(const QByteArray &topic, const QByteArray &payload, bool retain = false);

    bool debug() const;
    void setDebug(bool newDebug);

signals:
    void log(int level, const char *str);
    void connected(int result);
    void disconnected();
    void published(int mid);

private:
    struct mosquitto *mMosqHandler;
    bool mConnected;
    int mQos;
    bool mDebug;

    static void log_callback(struct mosquitto *mosq, void *obj, int level, const char *str);
    static void connect_callback(struct mosquitto *mosq, void *obj, int result);
    static void disconnect_callback(struct mosquitto *mosq, void *obj, int rc);
    static void publish_callback(struct mosquitto *mosq, void *obj, int mid);
};
#endif // MOSQUITTO_H
