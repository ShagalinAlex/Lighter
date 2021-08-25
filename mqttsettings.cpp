#include "mqttsettings.h"

MQTTSettings::MQTTSettings(bool valid)
{
    m_isValid = valid;
}

QString MQTTSettings::uri() const
{
    return m_uri;
}

QString MQTTSettings::name() const
{
    return m_name;
}

QString MQTTSettings::passwd() const
{
    return m_passwd;
}

bool MQTTSettings::isValid() const
{
    return m_isValid;
}

QString MQTTSettings::topic() const
{
    return m_topic;
}

void MQTTSettings::setUri(QString uri)
{
    m_uri = uri;
}

void MQTTSettings::setName(QString name)
{
    m_name = name;
}

void MQTTSettings::setPasswd(QString passwd)
{
    m_passwd = passwd;
}

void MQTTSettings::setTopic(QString topic)
{
    m_topic = topic;
}
