#include <QNetworkInterface>
#include <QDebug>

#include "avaservice.h"

AvaService::AvaService(QObject *parent) : QObject(parent)
{
    foreach(auto address, QNetworkInterface::allAddresses())
    {
         bool ok;
         address.toIPv4Address(&ok);
         if(!ok)
             continue;
         if(address.isLoopback())
             continue;

        //qDebug() << address;
        UdpFinderClient *finderClient = new UdpFinderClient(address);
        m_finderClients.append(finderClient);
        connect(finderClient, SIGNAL(searchDone(ESPDeviceInformation*)), this, SLOT(serverFinded(ESPDeviceInformation*)));
    }
}

QList<QObject*> AvaService::abonents() const
{
    QList<QObject *> abonents;
    foreach (auto abonent, m_abonents.values()) {
        abonents.append(abonent);
    }
    return abonents;
}

void AvaService::resetAbonents()
{
    m_abonents.clear();
    emit abonentsChanged(abonents());
}

void AvaService::serverFinded(ESPDeviceInformation *device)
{
    if(m_abonents.contains(device->address()))
    {
        if(m_abonents[device->address()]->name() == device->name()
                && m_abonents[device->address()]->type() == device->type())
            return;

        m_abonents[device->address()]->setAddress(device->address());
        m_abonents[device->address()]->setName(device->name());
        m_abonents[device->address()]->setType(device->type());
        emit abonentsChanged(abonents());
        return;
    }

    ESPDeviceInformation *newClient = new ESPDeviceInformation();
    newClient->setAddress(device->address());
    newClient->setId(device->id());
    newClient->setName(device->name());
    newClient->setType(device->type());
    m_abonents[device->address()] = newClient;
    emit abonentsChanged(abonents());
}
