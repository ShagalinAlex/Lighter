#ifndef GATEADAPTER_H
#define GATEADAPTER_H

#include <QObject>
#include <QTcpSocket>

#include "adapterbase.h"




class GateAdapter : public AdapterBase
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(LightState lightState READ lightState WRITE setLightState NOTIFY lightStateChanged)
    Q_PROPERTY(GateState gateState READ gateState WRITE setGateState NOTIFY gateStateChanged)
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY humidityChanged)

    QTcpSocket m_tcpSocket;
    QString m_address;
    int m_errorTransmitCounter = 0;


public:
    enum LightState { LIGHT_OFF, LIGHT_ON };
    enum GateState { GATE_IDLE, GATE_OPEN, GATE_CLOSE };

    explicit GateAdapter(QObject *parent = nullptr);
    void open(QString portName) override;
    void close() override;
    QString address() const;
    LightState lightState() const;

    Q_INVOKABLE int pulseDoor();
    Q_INVOKABLE int pulseLight();
    Q_INVOKABLE int pulseGate();
    Q_INVOKABLE QJSValue getStatus();

    float temperature() const;
    float humidity() const;

    static void declareQML() {
        qmlRegisterType<GateAdapter>("ESPDevice",1, 0,"GateAdapter");
    }

signals:

    void addressChanged(QString address);

    void lightStateChanged(LightState lightState);

    void gateStateChanged(GateState gateState);

    void temperatureChanged(float temperature);

    void humidityChanged(float humidity);

public slots:
    void setAddress(QString address);
    void setLightState(LightState lightState);
    void setGateState(GateState gateState);
    void setTemperature(float temperature);
    void setHumidity(float humidity);

protected:
    void customProcessMessage(QByteArray message) override;

public:
    Q_ENUM(LightState)
    Q_ENUM(GateState)

    GateState gateState() const;



private:
    LightState m_lightState = LIGHT_OFF;
    GateState m_gateState = GATE_IDLE;
    float m_temperature;
    float m_humidity;
};

#endif // GATEADAPTER_H
