#include <QHostAddress>
#include <QDebug>

#include "rgbdapter.h"

RGBAdapter::RGBAdapter(QObject *parent) : AdapterBase(parent)
{
    m_color = QColor("#0d495c");


    setDataPort(&m_tcpSocket);
}

void RGBAdapter::open(QString portName)
{
    const quint16 port = 5555;
    m_tcpSocket.connectToHost(QHostAddress(portName), port);
    qDebug() << "connected";

}

void RGBAdapter::close()
{
    m_tcpSocket.close();
}

qreal RGBAdapter::brightness() const
{
    return m_brightness;
}

QColor RGBAdapter::color() const
{
    return m_color;
}

QString RGBAdapter::address() const
{
    return m_address;
}

void RGBAdapter::setAddress(QString address)
{
    qDebug() << address;

    if (m_address == address)
        return;

    m_address = address;

    open(address);

    emit addressChanged(m_address);
}

void RGBAdapter::setBrightness(qreal brightness)
{
    if (qAbs(m_brightness - brightness) < 0.01)
        return;

    qDebug() << brightness;

    MessageParam message;
    message.type = MESSAGE_SET_PARAM;
    message.paramNb = 0;
    message.ui32Value = 0;

    message.ui32Value |= static_cast<quint32>(m_color.blue() * brightness) & 0xff;
    message.ui32Value |= (static_cast<quint32>(m_color.green() * brightness) << 8) & 0xff00;
    message.ui32Value |= (static_cast<quint32>(m_color.red() * brightness) << 16) & 0xff0000;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));

        if(answer.result == 1)
        {
            m_brightness = brightness;
            emit brightnessChanged(m_brightness);
        }
    }

}

void RGBAdapter::setColor(QColor color)
{
    //qDebug() << "Set color: " << color;
    if (m_color == color)
        return;

    MessageParam message;
    message.type = MESSAGE_SET_PARAM;
    message.paramNb = 0;
    message.ui32Value = 0;

    message.ui32Value |= static_cast<quint32>(color.blue() * m_brightness) & 0x000000ff;
    message.ui32Value |= (static_cast<quint32>(color.green() * m_brightness) << 8) & 0x0000ff00;
    message.ui32Value |= (static_cast<quint32>(color.red() * m_brightness) << 16) & 0x00ff0000;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));

        if(answer.result == 1)
        {
            //qDebug() << "Set color: " << QString::number(message.ui32Value, 16);
            m_color = color;
            emit colorChanged(m_color);
        }
    }
}

void RGBAdapter::customProcessMessage(QByteArray message)
{
    Q_UNUSED(message)
}
