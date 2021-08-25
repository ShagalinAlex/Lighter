#include "devicesettings.h"

void DeviceSettings::setApName(QString apName)
{
    if (m_apName == apName)
        return;

    m_apName = apName;
}

void DeviceSettings::setApPass(QString apPass)
{
    if (m_apPass == apPass)
        return;

    m_apPass = apPass;
}

void DeviceSettings::setDevId(quint8 devId)
{
    if (m_devId == devId)
        return;

    m_devId = devId;
}

void DeviceSettings::setDevName(QString devName)
{
    if (m_devName == devName)
        return;

    m_devName = devName;
}

void DeviceSettings::setApType(DeviceSettings::ApType apType)
{
    if (m_apType == apType)
        return;

    m_apType = apType;
}

DeviceSettings::DeviceSettings(bool valid)
{
    m_isValid = valid;
}

QString DeviceSettings::apName() const
{
    return m_apName;
}

QString DeviceSettings::apPass() const
{
    return m_apPass;
}

quint8 DeviceSettings::devId() const
{
    return m_devId;
}

QString DeviceSettings::devName() const
{
    return m_devName;
}

DeviceSettings::ApType DeviceSettings::apType() const
{
    return m_apType;
}

bool DeviceSettings::isValid() const
{
    return m_isValid;
}
