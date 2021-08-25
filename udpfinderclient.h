#ifndef UDPFINDERCLIENT_H
#define UDPFINDERCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "espdeviceinformation.h"

#pragma pack(1)
typedef struct _udpSendData {
    uint8_t id;
    uint8_t type;
    uint8_t deviceNameSize;
    int8_t deviceName[32];
} UdpSendData;
#pragma pack()

class UdpFinderClient : public QObject
{
    Q_OBJECT

    QUdpSocket m_udpSocket;
    QTimer m_timer;
    QHostAddress m_myAddress;

public:
    explicit UdpFinderClient(QHostAddress address, QObject *parent = nullptr);

signals:
    void searchDone(ESPDeviceInformation*);

public slots:

private slots:
    void timerElapsed();
    void dataRecieved();
};

#endif // UDPFINDERCLIENT_H
