#include "moquisttoMqtt.h"

MosquittoMqtt::MosquittoMqtt(QObject *parent) :
    QThread(parent),
    mMosqHandler(nullptr),
    mConnected(false),
    mQos(0),
    mDebug(false)
{
    mosquitto_lib_init();
    mMosqHandler = mosquitto_new(NULL, true, this);
}

MosquittoMqtt::~MosquittoMqtt() {
    mosquitto_destroy(mMosqHandler);
    mosquitto_lib_cleanup();
}

void MosquittoMqtt::run() {
    int rc;
    do {
        rc = mosquitto_loop(mMosqHandler, -1, 1);
    } while(rc == MOSQ_ERR_SUCCESS && mConnected);
}

int MosquittoMqtt::connect(const char * host, int port, int keepalive) {
    int rc;
    rc = mosquitto_connect(mMosqHandler, host, port, keepalive);
    if(rc){
        if (mDebug) {
            if(rc == MOSQ_ERR_ERRNO){
                char err[1024];
                strerror_r(errno, err, 1024);
                fprintf(stderr, "Connect : Error %s\n", err);
            }else{
                fprintf(stderr, "Unable to connect (%d).\n", rc);
            }
        }
        mosquitto_lib_cleanup();
        return rc;
    }
    mosquitto_log_callback_set(mMosqHandler, log_callback);
    mosquitto_connect_callback_set(mMosqHandler, connect_callback);
    mosquitto_disconnect_callback_set(mMosqHandler, disconnect_callback);
    mosquitto_publish_callback_set(mMosqHandler, publish_callback);
    mConnected = true;
    return 0;
}

void MosquittoMqtt::disconnect()
{
    mosquitto_disconnect(this->mMosqHandler);
    mConnected = false;
}

void MosquittoMqtt::setQos(int qos)
{
    mQos = qos;
}

int MosquittoMqtt::publish(const QByteArray &topic, const QByteArray &payload, bool retain)
{
    int rc = mosquitto_publish(this->mMosqHandler,
                           NULL,
                           topic.constData(),
                           payload.size(),
                           payload.constData(),
                           mQos,
                           retain);
    if(rc){
        if (mDebug) {
            switch(rc){
                case MOSQ_ERR_INVAL:
                    fprintf(stderr, "Error: Invalid input. Does your topic contain '+' or '#'?\n");
                    break;
                case MOSQ_ERR_NOMEM:
                    fprintf(stderr, "Error: Out of memory when trying to publish message.\n");
                    break;
                case MOSQ_ERR_NO_CONN:
                    fprintf(stderr, "Error: Client not connected when trying to publish.\n");
                    break;
                case MOSQ_ERR_PROTOCOL:
                    fprintf(stderr, "Error: Protocol error when communicating with broker.\n");
                    break;
                case MOSQ_ERR_PAYLOAD_SIZE:
                    fprintf(stderr, "Error: Message payload is too large.\n");
                    break;
            }
        }
        mosquitto_disconnect(mMosqHandler);
    }
    return rc;
}

bool MosquittoMqtt::debug() const
{
    return mDebug;
}

void MosquittoMqtt::setDebug(bool newDebug)
{
    mDebug = newDebug;
}


void MosquittoMqtt::log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
    Q_UNUSED(mosq)
    MosquittoMqtt * mqtt = static_cast<MosquittoMqtt *>(obj);
    if (mqtt) {
        emit mqtt->log(level, str);
    }
}

void MosquittoMqtt::connect_callback(struct mosquitto *mosq, void *obj, int result)
{
    Q_UNUSED(mosq)
    MosquittoMqtt * mqtt = static_cast<MosquittoMqtt *>(obj);
    if (mqtt) {
        emit mqtt->connected(result);
    }
}

void MosquittoMqtt::disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    Q_UNUSED(mosq)
    Q_UNUSED(rc)
    MosquittoMqtt * mqtt = static_cast<MosquittoMqtt *>(obj);
    if (mqtt) {
        mqtt->mConnected = false;
        emit mqtt->disconnected();
    }
}

void MosquittoMqtt::publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
    Q_UNUSED(mosq)
    MosquittoMqtt * mqtt = static_cast<MosquittoMqtt *>(obj);
    if (mqtt) {
        emit mqtt->published(mid);
    }
}
