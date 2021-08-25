#include <QHostAddress>

#include "gateadapter.h"

GateAdapter::GateAdapter(QObject *parent) : AdapterBase(parent)
{
    setDataPort(&m_tcpSocket);
}


void GateAdapter::setAddress(QString address)
{
    if (m_address == address)
        return;

    m_tcpSocket.connectToHost(QHostAddress(address), 5555);
    if(m_tcpSocket.waitForConnected(2000))
        getStatus();
    m_address = address;
    emit addressChanged(m_address);
}

void GateAdapter::setLightState(GateAdapter::LightState lightState)
{
    if (m_lightState == lightState)
        return;

    MessageLight message;
    message.type = MESSAGE_SET_LIGHT_STATUS;
    message.state = lightState;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));

        if(answer.result == 1)
        {
            m_lightState = lightState;
            emit lightStateChanged(m_lightState);
        }
    }
}

void GateAdapter::setGateState(GateAdapter::GateState gateState)
{
    if (m_gateState == gateState)
        return;

    MessageGate message;
    message.type = MESSAGE_SET_GATE_STATUS;
    message.status = gateState;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));

        if(answer.result == 1)
        {
            m_gateState = gateState;
            emit gateStateChanged(m_gateState);
        }
    }
}

void GateAdapter::setTemperature(float temperature)
{
    if (qAbs(static_cast<double>(m_temperature) - static_cast<double>(temperature)) < 0.1)
        return;

    m_temperature = temperature;
    emit temperatureChanged(m_temperature);
}

void GateAdapter::setHumidity(float humidity)
{
    if (qAbs(static_cast<double>(m_humidity) - static_cast<double>(humidity)) < 0.1)
        return;

    m_humidity = humidity;
    emit humidityChanged(m_humidity);
}

void GateAdapter::customProcessMessage(QByteArray message)
{
    Q_UNUSED(message)
}

void GateAdapter::open(QString portName)
{
    Q_UNUSED(portName)
}

void GateAdapter::close()
{

}

QString GateAdapter::address() const
{
    return m_address;
}

GateAdapter::LightState GateAdapter::lightState() const
{
    return m_lightState;
}

GateAdapter::GateState GateAdapter::gateState() const
{
    return m_gateState;
}


int GateAdapter::pulseDoor()
{
    MessageCommand message;
    message.type = MESSAGE_PULSE_DOOR;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return -1;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));
        return answer.result;
    }
    return  -1;
}

int GateAdapter::pulseLight()
{
    MessageCommand message;
    message.type = MESSAGE_PULSE_LIGHT;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return -1;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));
        return answer.result;
    }
    return  -1;

}

int GateAdapter::pulseGate()
{
    MessageCommand message;
    message.type = MESSAGE_PULSE_GATE;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));

    if(result.length() > 0) {
        if(result.length() != sizeof(MessageResult))
            return -1;
        MessageResult answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageResult));
        return answer.result;
    }
    return  -1;
}

QJSValue GateAdapter::getStatus()
{
    QJSValue status;
    MessageCommand message;
    message.type = MESSAGE_GET_STATUS;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0)
        m_errorTransmitCounter++;
    else
        m_errorTransmitCounter = 0;
    setIsConnected(m_errorTransmitCounter < 3);


    if(result.length() > 0) {
        if(result.length() != sizeof(MessageStatus))
            return status;
        MessageStatus answer;
        memcpy(reinterpret_cast<char*>(&answer), result.data(), sizeof(MessageStatus));

        status.setProperty("light", answer.lightStatus);
        status.setProperty("gate", answer.gateStatus);
        status.setProperty("temperature", static_cast<double>(answer.temperature));
        status.setProperty("humidity", static_cast<double>(answer.humidity));

        if(m_lightState != static_cast<GateAdapter::LightState>(answer.lightStatus)) {
            m_lightState = static_cast<GateAdapter::LightState>(answer.lightStatus);
            emit lightStateChanged(m_lightState);
        }

        if(m_gateState != static_cast<GateAdapter::GateState>(answer.gateStatus)) {
            m_gateState = static_cast<GateAdapter::GateState>(answer.gateStatus);
            emit gateStateChanged(m_gateState);
        }

        setTemperature(answer.temperature);
        setHumidity(answer.humidity);
        return status;
    }
    return status;
}

float GateAdapter::temperature() const
{
    return m_temperature;
}

float GateAdapter::humidity() const
{
    return m_humidity;
}
