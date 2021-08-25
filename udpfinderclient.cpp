#include <QNetworkDatagram>
#include <QDebug>
#include <QNetworkInterface>
#include <QNetworkConfiguration>

#include "udpfinderclient.h"

UdpFinderClient::UdpFinderClient(QHostAddress address, QObject *parent) : QObject(parent)
{
    m_udpSocket.bind(address, 5556);
    m_myAddress = address;

    connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(dataRecieved()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerElapsed()));
    m_timer.start(2000);
    timerElapsed();
}

void UdpFinderClient::timerElapsed()
{
    QNetworkDatagram datagramm;
    datagramm.setSender(m_myAddress, 5556);
    datagramm.setDestination(QHostAddress::Broadcast, 5555);
    datagramm.setData("hello");
    m_udpSocket.writeDatagram(datagramm);
}

void UdpFinderClient::dataRecieved()
{
    while (m_udpSocket.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = m_udpSocket.receiveDatagram();
        QByteArray data = datagram.data();
        //qDebug() << "Client: find server " << datagram.senderAddress().toString() << data.length();
        if(data.length() != static_cast<int>(sizeof(UdpSendData)))
            continue;
        UdpSendData *structData = reinterpret_cast<UdpSendData *>(data.data());

        ESPDeviceInformation deviceInformation;
        deviceInformation.setAddress(datagram.senderAddress().toString());
        deviceInformation.setId(structData->id);
        deviceInformation.setType(structData->type);
        deviceInformation.setName(QString::fromUtf8(reinterpret_cast<const char*>(structData->deviceName), structData->deviceNameSize));

        emit searchDone(&deviceInformation);

//        qDebug() << "Client: find server " << datagram.senderAddress().toString() << data.length();
    }
}
