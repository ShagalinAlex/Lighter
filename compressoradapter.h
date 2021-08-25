#ifndef COMPRESSORADAPTER_H
#define COMPRESSORADAPTER_H

#include <QObject>
#include <QTcpSocket>

#include "adapterbase.h"


/// Набор глобальных перечислителей
class CompressorAdapterData {
    Q_GADGET
public:
    enum CompressorState{
        ENSTATE_IDLE,
        ENSTATE_STARTING,
        ENSTATE_COMPRESSING,
        ENSTATE_STOPING
    };
    Q_ENUM(CompressorState)

    static void declareQML() {
        qmlRegisterUncreatableType<CompressorAdapterData>("ESPDevice",1, 0,"CompressorAdapterData", "DataAdapter reason");
    }

    explicit CompressorAdapterData() {}
};

class CompressorAdapter :  public AdapterBase
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)

    Q_PROPERTY(float pressure READ pressure NOTIFY pressureChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool oilPressure READ oilPressure NOTIFY oilPressureChanged)
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(float pressureSelected READ pressureSelected WRITE setPressureSelected NOTIFY pressureSelectedChanged)
    Q_PROPERTY(int compressorState READ compressorState NOTIFY compressorStateChanged)


    QTcpSocket m_tcpSocket;

    QString m_address;
    float m_pressure = 0.0f;
    bool m_enabled = false;
    float m_temperature = 0.0f;
    float m_pressureSelected = 2.0f;
    int m_errorTransmitCounter = 0;

    float requestPressure(bool *ok = 0);
    float requestTemperature(bool *ok = 0);
    float requestPressureSelected(bool *ok = 0);
    bool requestState(bool *ok);
    bool requestOilPressure(bool *ok);

    void setPressure(float pressure);
    void setTemperature(float temperature);

    bool m_oilPressure = false;

    int m_compressorState = CompressorAdapterData::ENSTATE_IDLE;

public:
    explicit CompressorAdapter(QObject *parent = nullptr);

    void open(QString portName) override;
    void close() override;
    QString address() const;
    float pressure() const;
    bool enabled() const;
    float temperature() const;
    float pressureSelected() const;
    void setOilPressure(bool oilPressure);

    static void declareQML() {
        qmlRegisterType<CompressorAdapter>("ESPDevice",1, 0,"CompressorAdapter");
        CompressorAdapterData::declareQML();
    }



    bool oilPressure() const;


    int compressorState() const;

    void setCompressorState(int compressorState);

public slots:
    void setAddress(QString address);
    void setEnabled(bool enabled);
    void setPressureSelected(float pressureSelected);
    void updateStatus();

signals:
    void addressChanged(QString address);
    void pressureChanged(float pressure);
    void enabledChanged(bool enabled);
    void temperatureChanged(float temperature);
    void pressureSelectedChanged(float pressureSelected);
    void oilPressureChanged(bool oilPressure);
    void compressorStateChanged(int compressorState);

protected:
    void customProcessMessage(QByteArray message) override;
};

#endif // COMPRESSORADAPTER_H
