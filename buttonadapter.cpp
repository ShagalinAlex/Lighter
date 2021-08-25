#include <QHostAddress>

#include "buttonadapter.h"

ButtonAdapter::ButtonAdapter(QObject *parent) : AdapterBase(parent)
{
    setDataPort(&m_tcpSocket);
}


void ButtonAdapter::setAddress(QString address)
{
    if (m_address == address)
        return;

    m_tcpSocket.connectToHost(QHostAddress(address), 5555);
    if(m_tcpSocket.waitForConnected(2000))
        getStatus();
    m_address = address;
    emit addressChanged(m_address);
}

void ButtonAdapter::setStatus(ButtonState status)
{
    if (m_status == status)
        return;



    MessageParam message;
    message.type = MESSAGE_SET_PARAM;
    message.paramNb = 0;
    message.ui32Value = static_cast<uint32_t>(status);

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));

        if(answer.result == 1)
        {
            m_status = status;
            emit statusChanged(m_status);
        }
    }
}

void ButtonAdapter::customProcessMessage(QByteArray message)
{
    Q_UNUSED(message)
}

void ButtonAdapter::open(QString portName)
{
    Q_UNUSED(portName)
}

void ButtonAdapter::close()
{

}

QString ButtonAdapter::address() const
{
    return m_address;
}

ButtonAdapter::ButtonState ButtonAdapter::status() const
{
    return m_status;
}

QJSValue ButtonAdapter::getStatus()
{
    QJSValue status;
    MessageCommand message;
    message.type = MESSAGE_GET_PARAM;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0)
        m_errorTransmitCounter++;
    else
        m_errorTransmitCounter = 0;
    setIsConnected(m_errorTransmitCounter < 3);


    if(result.length() > 0) {
        if(result.length() != sizeof(MessageParam))
            return status;
        MessageParam answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageParam));

        status.setProperty("status", answer.ui32Value);
        qDebug() << answer.ui32Value;

        setStatus(answer.ui32Value > 0 ? BUTTON_ON : BUTTON_OFF);

        return status;
    }
    return status;
}
