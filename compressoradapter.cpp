#include <QHostAddress>

#include "compressoradapter.h"

CompressorAdapter::CompressorAdapter(QObject *parent) : AdapterBase(parent)
{
    setDataPort(&m_tcpSocket);
}

void CompressorAdapter::open(QString portName)
{
    Q_UNUSED(portName)

}

void CompressorAdapter::close()
{

}

QString CompressorAdapter::address() const
{
    return m_address;
}

float CompressorAdapter::pressure() const
{
    return m_pressure;
}

bool CompressorAdapter::enabled() const
{
    return m_enabled;
}

float CompressorAdapter::temperature() const
{
    return m_temperature;
}

float CompressorAdapter::pressureSelected() const
{
    return m_pressureSelected;
}

bool CompressorAdapter::oilPressure() const
{
    return m_oilPressure;
}

int CompressorAdapter::compressorState() const
{
    return m_compressorState;
}

void CompressorAdapter::setAddress(QString address)
{
    if (m_address == address)
        return;

    m_tcpSocket.connectToHost(QHostAddress(address), 5555);
    if(m_tcpSocket.waitForConnected(2000))
        updateStatus();
    m_address = address;
    emit addressChanged(m_address);
}

void CompressorAdapter::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    MessageParam message;
    message.type = MESSAGE_SET_PARAM;
    message.paramNb = 0;
    message.ui32Value = enabled;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        qDebug() << "Ошибка отправки параметров";
        return;
    }

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void CompressorAdapter::setPressureSelected(float pressureSelected)
{
    if (qAbs(m_pressureSelected - pressureSelected) < 0.1)
        return;
    qDebug() << pressureSelected;
    MessageParam message;
    message.type = MESSAGE_SET_PARAM;
    message.paramNb = 1;
    message.fValue = pressureSelected;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        qDebug() << "Ошибка отправки параметров";
        return;
    }

    m_pressureSelected = pressureSelected;
    emit pressureSelectedChanged(m_pressureSelected);
}

void CompressorAdapter::customProcessMessage(QByteArray message)
{
    Q_UNUSED(message)
}

void CompressorAdapter::setTemperature(float temperature)
{
    if (qAbs(m_temperature - temperature) < 0.1)
        return;

    m_temperature = temperature;
    emit temperatureChanged(m_pressure);
}

void CompressorAdapter::setCompressorState(int compressorState)
{
    m_compressorState = compressorState;
}

void CompressorAdapter::setOilPressure(bool oilPressure)
{
    if(m_oilPressure == oilPressure)
        return;

    m_oilPressure = oilPressure;
    emit oilPressureChanged(oilPressure);
}

float CompressorAdapter::requestPressure(bool *ok) {
    MessageParamNb message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 2;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }

    if(result.length() != sizeof(MessageParam)) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }
    MessageParam *answer = reinterpret_cast<MessageParam *>(result.data());
    if(answer->paramNb != 2) {
        if(ok != nullptr) {
            *ok = false;
        }
        return 0.0;
    }
    *ok = true;
    return answer->fValue;
}

float CompressorAdapter::requestTemperature(bool *ok) {
    MessageParamNb message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 3;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }

    if(result.length() != sizeof(MessageParam)) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }
    MessageParam *answer = reinterpret_cast<MessageParam *>(result.data());
    if(answer->paramNb != 3) {
        if(ok != nullptr)
            *ok = false;
        return 0.0;
    }
    *ok = true;
    return answer->fValue;
}

float CompressorAdapter::requestPressureSelected(bool *ok) {
    MessageParamNb message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 1;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }

    if(result.length() != sizeof(MessageParam)) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }
    MessageParam *answer = reinterpret_cast<MessageParam *>(result.data());
    if(answer->paramNb != 1) {
        if(ok != nullptr)
            *ok = false;
        return 0.0;
    }
    *ok = true;
    return answer->fValue;
}

bool CompressorAdapter::requestState(bool *ok) {
    MessageParamNb message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 0;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }

    if(result.length() != sizeof(MessageParam)) {
        if(ok != nullptr)
            *ok = false;
        return 0.0f;
    }
    MessageParam *answer = reinterpret_cast<MessageParam *>(result.data());
    if(answer->paramNb != 0) {
        if(ok != nullptr)
            *ok = false;
        return false;
    }
    *ok = true;

    if(m_enabled != (answer->ui8Value[0] & 0x01)) {
        m_enabled = answer->ui8Value[0] & 0x01;
        emit enabledChanged(m_enabled);
    }

    if(m_oilPressure != ((answer->ui8Value[0] >> 1) & 0x01)) {
        m_oilPressure = ((answer->ui8Value[0] >> 1) & 0x01);
        emit oilPressureChanged(m_oilPressure);
    }

    if(m_compressorState != ((answer->ui8Value[0] >> 4) & 0x0f)) {
        m_compressorState = ((answer->ui8Value[0] >> 4) & 0x0f);
        emit compressorStateChanged(m_compressorState);
    }

    float pressureSelected = (float)(answer->ui8Value[1]) / 10;
    if(pressureSelected != m_pressureSelected) {
        m_pressureSelected = pressureSelected;
        emit pressureSelectedChanged(m_pressureSelected);
    }

    float pressure = (float)(answer->ui8Value[2]) / 10;
    setPressure(pressure);

    float temperature = (float)(answer->ui8Value[3]) / 10;
    setTemperature(temperature);

    return answer->ui32Value;
}

bool CompressorAdapter::requestOilPressure(bool *ok)
{
    MessageParamNb message;
    message.type = MESSAGE_GET_PARAM;
    message.paramNb = 4;

    char *tmp = reinterpret_cast<char*>(&message);
    QByteArray result = sendMessage(tmp, sizeof(message));
    if(result.length() == 0) {
        if(ok != nullptr)
            *ok = false;
        return false;
    }

    if(result.length() != sizeof(MessageParam)) {
        if(ok != nullptr)
            *ok = false;
        return false;
    }
    MessageParam *answer = reinterpret_cast<MessageParam *>(result.data());
    if(answer->paramNb != 4) {
        if(ok != nullptr)
            *ok = false;
        return false;
    }
    *ok = true;
    return answer->ui32Value > 0;
}

void CompressorAdapter::setPressure(float pressure)
{
    if (qAbs(m_pressure - pressure) < 0.1)
        return;

    m_pressure = pressure;
    emit pressureChanged(m_pressure);
}


void CompressorAdapter::updateStatus()
{
    bool ok;
//    float pressure = requestPressure(&ok);
//    if(ok)
//        setPressure(pressure);

//    float temperature = requestTemperature(&ok);
//    if(ok)
//        setTemperature(temperature);

//    float pressureSelected = requestPressureSelected(&ok);
//    if(ok) {
//        m_pressureSelected = pressureSelected;
//        emit pressureSelectedChanged(m_pressureSelected);
//    }

    requestState(&ok);
//    bool enabled = requestState(&ok);
//    if(ok) {
//        m_enabled = enabled;
//        emit enabledChanged(m_enabled);
//    }
}
