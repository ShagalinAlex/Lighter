#ifndef BME280ADAPTER_H
#define BME280ADAPTER_H

#include <QObject>
#include <QTcpSocket>

#include "adapterbase.h"

class BME280Adapter : public AdapterBase
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY humidityChanged)
    Q_PROPERTY(float pressure READ pressure NOTIFY pressureChanged)

    QTcpSocket m_tcpSocket;
    QString m_address;
    int m_errorTransmitCounter = 0;

    QTimer m_requestDataTimer;

    void requestTemperature();
    void requestHumidity();
    void requestPressure();


public:
    explicit BME280Adapter(QObject *parent = nullptr);
    void open(QString portName) override;
    void close() override;
    QString address() const;


    float temperature() const;
    float humidity() const;
    float pressure() const;

    static void declareQML() {
        qmlRegisterType<BME280Adapter>("ESPDevice",1, 0,"BME280Adapter");
    }

signals:
    void addressChanged(QString address);
    void temperatureChanged(float temperature);
    void humidityChanged(float humidity);
    void pressureChanged(float pressure);

public slots:
    void setAddress(QString address);
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setPressure(float pressure);

    void updateData();


protected:
    void customProcessMessage(QByteArray message) override;

private:
    float m_temperature;
    float m_humidity;
    float m_pressure;
};

#endif // BME280ADAPTER_H
