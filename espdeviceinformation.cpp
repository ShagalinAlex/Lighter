#include "espdeviceinformation.h"

ESPDeviceInformation::ESPDeviceInformation(QObject *parent) : QObject(parent)
{

}

QString ESPDeviceInformation::address() const
{
    return m_address;
}

int ESPDeviceInformation::id() const
{
    return m_id;
}

QString ESPDeviceInformation::name() const
{
    return m_name;
}

int ESPDeviceInformation::type() const
{
    return m_type;
}

void ESPDeviceInformation::setAddress(QString address)
{
    if (m_address == address)
        return;

    m_address = address;
    emit addressChanged(m_address);
}

void ESPDeviceInformation::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void ESPDeviceInformation::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void ESPDeviceInformation::setType(int type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}
