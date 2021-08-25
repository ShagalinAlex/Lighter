#include <QSerialPortInfo>
#include <QDebug>
#include <QTcpSocket>
#include <QJSValue>
#include "adapterbase.h"
#include "message.h"

QIODevice *AdapterBase::dataPort() const
{
    return m_dataPort;
}

void AdapterBase::setDataPort(QIODevice *dataPort)
{
    m_dataPort = dataPort;
}

void AdapterBase::setIsConnected(bool isConnected)
{
    if (m_isConnected == isConnected)
        return;

    m_isConnected = isConnected;
    emit isConnectedChanged(isConnected);
}

QByteArray AdapterBase::packData(QByteArray data)
{
    QByteArray packet;
    packet.append(PACKET_HEADER1);
    packet.append(PACKET_HEADER2);
    quint8 u8Len = static_cast<quint8>(data.length());
    packet.append(*reinterpret_cast<char*>(&u8Len));
    packet.append(data);
    packet.append(PACKET_FINITE);
    return packet;
}

QByteArray AdapterBase::packData(char *message, int length)
{
    QByteArray packet;
    packet.append(PACKET_HEADER1);
    packet.append(PACKET_HEADER2);
    quint8 u8Len = static_cast<quint8>(length);
    packet.append(*reinterpret_cast<char*>(&u8Len));
    packet.append(message, length);
    packet.append(PACKET_FINITE);
    return packet;
}

QByteArray AdapterBase::sendMessage(QByteArray message)
{
    if(m_dataPort == nullptr || !m_dataPort->isOpen())
        return QByteArray();

    QByteArray packedData = packData(message);

    m_ciclicData.clear();
    qDebug() << m_dataPort->bytesAvailable();
    m_dataPort->write(packedData);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000) {
        if(!m_dataPort->waitForReadyRead(100))
            continue;
        QByteArray data = m_dataPort->readAll();


        for(int i = 0; i < data.length(); i++)
            m_ciclicData.append(data[i]);

        QByteArray result = findMessage();
        if(result.length() > 0) {
            return result;
        }
    }

    return QByteArray();
}

QByteArray AdapterBase::sendMessage(char *message, int length)
{
    if(m_dataPort == nullptr || !m_dataPort->isOpen())
        return QByteArray();

    QByteArray packedData = packData(message, length);

    m_ciclicData.clear();
    QByteArray emptydata = m_dataPort->readAll();

    m_dataPort->write(packedData);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000) {
        if(!m_dataPort->waitForReadyRead(100))
            continue;
        QByteArray data = m_dataPort->readAll();

        for(int i = 0; i < data.length(); i++)
            m_ciclicData.append(data[i]);

        QByteArray result = findMessage();
        if(result.length() > 0) {
            return result;
        }
    }

    return QByteArray();
}

void AdapterBase::processMessage(QByteArray message)
{
    if(message.isEmpty())
        return;

    customProcessMessage(message);
}

QByteArray AdapterBase::findMessage()
{
    skipForHeader();

    while(m_ciclicData.length() > MIN_MESSAGE_SIZE) {
        int lenght = *(reinterpret_cast<const quint8*>(&(m_ciclicData.at(MESSAGE_LENGTH_OFFSET))));

        if(m_ciclicData.length() < MIN_MESSAGE_SIZE + lenght)
            break;


        char crcChar = m_ciclicData[PACKET_HEADER_SIZE + lenght];

        if(crcChar == PACKET_FINITE)
        {

            for(int i = 0; i < PACKET_HEADER_SIZE; i++)
                m_ciclicData.dequeue();

            QByteArray message;
            for(int i = 0; i < lenght; i++)
                message.append(m_ciclicData.dequeue());

            return message;
        }
        else
            m_ciclicData.dequeue();
        skipForHeader();
    }
    return QByteArray();
}

QString AdapterBase::arrayToString(QByteArray data)
{
    QString result;
    for(int i = 0; i < data.length(); i++) {
        result += QString::number(data[i] & 0xff, 16).leftJustified(2, '0') + " ";
    }
    return result;
}

AdapterBase::AdapterBase(QObject *parent) : QObject(parent)
{
}




void AdapterBase::requestRestart()
{
    if(m_dataPort == nullptr || !m_dataPort->isOpen())
        return;

    MessageCommand message;
    message.type = MESSAGE_RESTART;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray messageData(tmp, sizeof(message));
    QByteArray packedData = packData(messageData);

    m_dataPort->write(packedData);
}

QVariant AdapterBase::requestSettings()
{
    if(m_dataPort == nullptr || !m_dataPort->isOpen())
        return QVariant();

    MessageCommand message;
    message.type = MESSAGE_GET_FLASHDATAPARAM;

    QByteArray packedData = packData(reinterpret_cast<char*>(&message), sizeof(MessageCommand));

    m_ciclicData.clear();

    m_dataPort->write(packedData);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000) {
        if(!m_dataPort->waitForReadyRead(100))
            continue;
        QByteArray data = m_dataPort->readAll();
        QString text;
        for(int i = 0; i < data.length(); i++) {
            m_ciclicData.append(data[i]);
            text.append(QString::number(data[i] & 0xff, 16)).append(" ");
        }
        qDebug() << text;
        QByteArray result = findMessage();
        if(result.length() > 0) {
            if(result.length() != sizeof(MessageFlashDataParam))
                continue;
            MessageFlashDataParam params;
            memcpy(reinterpret_cast<char*>(&params), result.data(), sizeof(MessageFlashDataParam));

            DeviceSettings currentSettings;
            currentSettings.setDevId(params.id);
            currentSettings.setApName(QString::fromLocal8Bit(reinterpret_cast<const char*>(params.apName), params.apNameSize));
            currentSettings.setApPass(QString::fromLocal8Bit(reinterpret_cast<const char*>(params.apPass), params.apPassSize));
            currentSettings.setDevName(QString::fromUtf8(reinterpret_cast<const char*>(params.deviceName), params.deviceNameSize));
            currentSettings.setApType(params.apType == 1 ?  DeviceSettings::STATION : DeviceSettings::SOFTAP);
            qDebug() << "Settings message done";
            return QVariant::fromValue(currentSettings);
        }
    }

    return QVariant();
}

int AdapterBase::saveSettings(QJSValue settings)
{
    MessageFlashDataParam message;
    memset(&message, 0, sizeof(message));
    message.type = MESSAGE_SET_FLASHDATAPARAM;

    message.id = 0;

    QByteArray devName = settings.property("devName").toString().toUtf8();
    memcpy(message.deviceName, devName, static_cast<uint8_t>(qMin(devName.length(), 32)));
    message.deviceNameSize = static_cast<uint8_t>(devName.length());

    QByteArray apName = settings.property("apName").toString().toLocal8Bit();
    memcpy(message.apName, apName, static_cast<uint8_t>(qMin(apName.length(), 32)));
    message.apNameSize = static_cast<uint8_t>(apName.length());

    QByteArray apPass = settings.property("apPass").toString().toLocal8Bit();
    memcpy(message.apPass, apPass, static_cast<uint8_t>(qMin(apPass.length(), 32)));
    message.apPassSize = static_cast<uint8_t>(apPass.length());
    message.apType = settings.property("apType").toUInt() & 0xff;

    QByteArray packedData = packData(reinterpret_cast<char*>(&message), sizeof(MessageFlashDataParam));

    m_ciclicData.clear();

    m_dataPort->write(packedData);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000) {
        if(!m_dataPort->waitForReadyRead(100))
            continue;
        QByteArray data = m_dataPort->readAll();

        QString text;
        for(int i = 0; i < data.length(); i++) {
            m_ciclicData.append(data[i]);
            text.append(QString::number(data[i] & 0xff, 16)).append(" ");
        }
        qDebug() << text;

        QByteArray result = findMessage();
        if(result.length() > 0) {
            if(result.length() != sizeof(MessageResult))
                continue;
            MessageResult answer;
            memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));
            return answer.result;
        }
    }
    return -1;
}

QVariant AdapterBase::requestMQTTSettings()
{
    if(m_dataPort == nullptr || !m_dataPort->isOpen())
        return QVariant();

    MessageCommand message;
    message.type = MESSAGE_GET_MQTTDATAPARAM;

    QByteArray packedData = packData(reinterpret_cast<char*>(&message), sizeof(MessageCommand));

    m_ciclicData.clear();

    m_dataPort->write(packedData);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 2000) {
        if(!m_dataPort->waitForReadyRead(100))
            continue;
        QByteArray data = m_dataPort->readAll();
        QString text;
        for(int i = 0; i < data.length(); i++) {
            m_ciclicData.append(data[i]);
            text.append(QString::number(data[i] & 0xff, 16)).append(" ");
        }
        qDebug() << text;
        QByteArray result = findMessage();
        qDebug() << m_ciclicData.length();

        if(result.length() > 0) {

            if(result.length() != sizeof(MessageMQTTDataParam))
                continue;
            MessageMQTTDataParam *params = reinterpret_cast<MessageMQTTDataParam*>(result.data());
            MQTTSettings settings;
            settings.setUri(QString::fromUtf8(reinterpret_cast<const char*>(params->serverUri)));
            settings.setName(QString::fromUtf8(reinterpret_cast<const char*>(params->name)));
            settings.setPasswd(QString::fromUtf8(reinterpret_cast<const char*>(params->pass)));
            settings.setTopic(QString::fromUtf8(reinterpret_cast<const char*>(params->topic)));

            return QVariant::fromValue(settings);
        }
    }

    return  QVariant();
}

int AdapterBase::saveMQTTSettings(QJSValue settings)
{
    MessageMQTTDataParam message;
    memset(&message, 0, sizeof(message));
    message.type = MESSAGE_SET_MQTTDATAPARAM;

    QByteArray devName = settings.property("name").toString().toUtf8();
    memcpy(message.name, devName, static_cast<uint8_t>(qMin(devName.length(), 32)));

    QByteArray devPasswd = settings.property("passwd").toString().toUtf8();
    memcpy(message.pass, devPasswd, static_cast<uint8_t>(qMin(devPasswd.length(), 32)));

    QByteArray devUri = settings.property("uri").toString().toUtf8();
    memcpy(message.serverUri, devUri, static_cast<uint8_t>(qMin(devUri.length(), 64)));

    QByteArray devTopic = settings.property("topic").toString().toUtf8();
    memcpy(message.topic, devTopic, static_cast<uint8_t>(qMin(devTopic.length(), 64)));

    QByteArray packedData = packData(reinterpret_cast<char*>(&message), sizeof(MessageMQTTDataParam));

    m_ciclicData.clear();

    m_dataPort->write(packedData);
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000) {
        if(!m_dataPort->waitForReadyRead(50))
            continue;
        QByteArray data = m_dataPort->readAll();

        QString text;
        for(int i = 0; i < data.length(); i++) {
            m_ciclicData.append(data[i]);
            text.append(QString::number(data[i] & 0xff, 16)).append(" ");
        }
//        qDebug() << text;

        QByteArray result = findMessage();
        if(result.length() > 0) {
            if(result.length() != sizeof(MessageResult))
                continue;
            MessageResult *answer = reinterpret_cast<MessageResult*>(result.data());
            return answer->result;
        }
    }
    return -1;
}

bool AdapterBase::opened() const
{
    return m_dataPort != nullptr && m_dataPort->isOpen();
}

void AdapterBase::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void AdapterBase::setDeviceName(QString deviceName)
{
    if (m_deviceName == deviceName)
        return;

    m_deviceName = deviceName;
    emit deviceNameChanged(m_deviceName);
}

void AdapterBase::skipForHeader()
{
    while(m_ciclicData.length() > MIN_MESSAGE_SIZE
          && m_ciclicData[0] != PACKET_HEADER1
          && m_ciclicData[1] != PACKET_HEADER2) //Ищем первый заголовок
    {
        m_ciclicData.dequeue();
    }
}

void AdapterBase::dataReady()
{
    if(m_dataPort == nullptr)
        return;

    QByteArray data = m_dataPort->readAll();
    for(int i = 0; i < data.length(); i++)
        m_ciclicData.append(data[i]);

    QByteArray message = findMessage();
    while(message.length() > 0)
    {
        processMessage(message);
        message = findMessage();
    }
    return;
}

int AdapterBase::id() const
{
    return m_id;
}

QString AdapterBase::deviceName() const
{
    return m_deviceName;
}

bool AdapterBase::isConnected() const
{
    return m_isConnected;
}
