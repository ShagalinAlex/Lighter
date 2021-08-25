#include <QSerialPortInfo>
#include <QDebug>
#include "serialadapter.h"

SerialAdapter::SerialAdapter(QObject *parent) : AdapterBase(parent)
{
    for(QSerialPortInfo serial: QSerialPortInfo::availablePorts())
        m_serials.append(serial.portName());

    QSerialPort *port = new QSerialPort();
    port->setBaudRate(QSerialPort::Baud9600);
    setDataPort(port);

    connect(&m_serialFindTimer, SIGNAL(timeout()), this, SLOT(serialFindTimerTimeout()));
    m_serialFindTimer.start(1000);
}

void SerialAdapter::open(QString portName)
{
    if(dataPort() == nullptr)
        return;

    QSerialPort *port = qobject_cast<QSerialPort*>(dataPort());
    if(port == nullptr)
        return;

    port->setPortName(portName);
    if(dataPort()->open(QIODevice::ReadWrite))
    {
        m_serialFindTimer.stop();

        emit openedChanged(true);
    }
}

void SerialAdapter::close()
{
    if(dataPort() == nullptr)
        return;

    dataPort()->close();
    m_serialFindTimer.start(1000);
    emit openedChanged(false);
}



QStringList SerialAdapter::serials() const
{
    return m_serials;
}



void SerialAdapter::setSerials(QStringList serials)
{
    if (m_serials == serials)
        return;

    m_serials = serials;
    emit serialsChanged(m_serials);
}



void SerialAdapter::serialFindTimerTimeout()
{
    QStringList serials;
    for(QSerialPortInfo serial: QSerialPortInfo::availablePorts())
        serials.append(serial.portName());
    setSerials(serials);
}

void SerialAdapter::portErrorHandled()
{
//    m_serialPort.close();
    m_serialFindTimer.start(1000);
    emit openedChanged(false);
}

void SerialAdapter::customProcessMessage(QByteArray message)
{
    Q_UNUSED(message)
}


