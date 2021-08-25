#include <QHostAddress>

#include "bme280adapter.h"

void BME280Adapter::requestTemperature()
{
    MessageParam message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 0;
    message.ui32Value = 0;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0)
        m_errorTransmitCounter++;
    else
        m_errorTransmitCounter = 0;
    setIsConnected(m_errorTransmitCounter < 10);

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageParam))
            return;

        MessageParam *answer = reinterpret_cast<MessageParam*>(result.data());
        setTemperature(answer->fValue);
        return;
    }
    return;
}

void BME280Adapter::requestHumidity()
{
    MessageParam message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 1;
    message.ui32Value = 0;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0)
        m_errorTransmitCounter++;
    else
        m_errorTransmitCounter = 0;
    setIsConnected(m_errorTransmitCounter < 10);


    if(result.length() > 0) {
        if(result.length() != sizeof(MessageParam))
            return;

        MessageParam *answer = reinterpret_cast<MessageParam*>(result.data());
        setHumidity(answer->fValue);
        return;
    }
    return;
}

void BME280Adapter::requestPressure()
{
    MessageParam message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 2;
    message.ui32Value = 0;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0)
        m_errorTransmitCounter++;
    else
        m_errorTransmitCounter = 0;
    setIsConnected(m_errorTransmitCounter < 10);


    if(result.length() > 0) {
        if(result.length() != sizeof(MessageParam))
            return;
        MessageParam *answer = reinterpret_cast<MessageParam*>(result.data());
        setPressure(answer->fValue);
        return;
    }
    return;}

BME280Adapter::BME280Adapter(QObject *parent) : AdapterBase(parent)
{
    setDataPort(&m_tcpSocket);
    connect(&m_requestDataTimer, SIGNAL(timeout()), this, SLOT(updateData()));

}


void BME280Adapter::setAddress(QString address)
{
    if (m_address == address)
        return;

    m_tcpSocket.connectToHost(QHostAddress(address), 5555);
    if(m_tcpSocket.waitForConnected(2000)) {
        updateData();
        m_requestDataTimer.start(5000);
    }
    m_address = address;
    emit addressChanged(m_address);
}

void BME280Adapter::setTemperature(float temperature)
{
    if (qAbs(static_cast<double>(m_temperature) - static_cast<double>(temperature)) < 0.1)
        return;

    m_temperature = temperature;
    emit temperatureChanged(m_temperature);
}

void BME280Adapter::setHumidity(float humidity)
{
    if (qAbs(static_cast<double>(m_humidity) - static_cast<double>(humidity)) < 0.1)
        return;

    m_humidity = humidity;
    emit humidityChanged(m_humidity);
}

void BME280Adapter::setPressure(float pressure)
{
    if (qAbs(static_cast<double>(m_pressure) - static_cast<double>(pressure)) < 0.1)
        return;

    m_pressure = pressure;
    emit humidityChanged(m_pressure);
}

void BME280Adapter::customProcessMessage(QByteArray message)
{
    Q_UNUSED(message)
}

void BME280Adapter::open(QString portName)
{
    Q_UNUSED(portName)
}

void BME280Adapter::close()
{

}

QString BME280Adapter::address() const
{
    return m_address;
}

void BME280Adapter::updateData()
{
   requestTemperature();
   requestPressure();
   requestHumidity();
}

float BME280Adapter::temperature() const
{
    return m_temperature;
}

float BME280Adapter::humidity() const
{
    return m_humidity;
}

float BME280Adapter::pressure() const
{
    return m_pressure;
}
