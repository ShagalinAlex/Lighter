#include <QDebug>
#include <QNetworkDatagram>
#include <QNetworkInterface>

#include "udpfinderserver.h"

UdpFinderServer::UdpFinderServer(QObject *parent) : QObject(parent)
{
    startServer(89683);

}

void UdpFinderServer::startServer(quint16 port)
{
    m_udpSocket.bind(port);
    connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(dataRecieved()));
}

QString UdpFinderServer::name() const
{
    return m_name;
}

void UdpFinderServer::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void UdpFinderServer::dataRecieved()
{
    while (m_udpSocket.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = m_udpSocket.receiveDatagram();

        foreach(auto address, QNetworkInterface::allAddresses())
        {
            if(datagram.senderAddress().isEqual(address))
            {
                qDebug() << "Server: local client " << datagram.senderAddress();
                return;
            }
        }

        QNetworkDatagram datagramm;
        datagramm.setDestination(datagram.senderAddress(), 89683);

        QByteArray data;
        for(int i = 0; i < 4; i++)
            data.append(i);

        datagramm.setData(data);
        m_udpSocket.writeDatagram(datagramm);
    }
}
