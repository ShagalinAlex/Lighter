#ifndef ADAPTERBASE_H
#define ADAPTERBASE_H

#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QTimer>
#include <QColor>
#include <QQueue>

#include "message.h"
#include "devicesettings.h"
#include "mqttsettings.h"

/// Набор глобальных перечислителей
class DataAdapter {
    Q_GADGET
public:
    enum ApType {
        APNULL,
        STATION,
        SOFTAP
    };
    Q_ENUM(ApType)

    enum DeviceTypes{
        DEV_TYPE_LIGHTER,
        DEV_TYPE_GATE,
        DEV_TYPE_ONOFF,
        DEV_TYPE_SENSOR_BME280,
        DEV_TYPE_COMPRESSOR
    };
    Q_ENUM(DeviceTypes)

    static void declareQML() {
        qmlRegisterUncreatableType<DataAdapter>("ESPDevice",1, 0,"DataAdapter", "DataAdapter reason");
    }

    explicit DataAdapter() {}
};

class AdapterBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

    QIODevice *m_dataPort = nullptr;

    QQueue<char> m_ciclicData;
    void processMessage(QByteArray message);
    QByteArray findMessage();

    QString arrayToString(QByteArray data);

protected:
    QByteArray packData(QByteArray data);
    QByteArray packData(char* message, int length);
    virtual void customProcessMessage(QByteArray message) = 0;
    QByteArray sendMessage(QByteArray message);
    QByteArray sendMessage(char* message, int length);

public:
    explicit AdapterBase(QObject *parent = nullptr);

    Q_INVOKABLE virtual void open(QString portName) = 0;
    Q_INVOKABLE virtual void close() = 0;
    Q_INVOKABLE void requestRestart();
    Q_INVOKABLE QVariant requestSettings();
    Q_INVOKABLE int saveSettings(QJSValue settings);
    Q_INVOKABLE QVariant requestMQTTSettings();
    Q_INVOKABLE int saveMQTTSettings(QJSValue settings);


    bool opened() const;

signals:
    void openedChanged(bool opened);
    void deviceNameChanged(QString deviceName);
    void idChanged(int id);
    void isConnectedChanged(bool isConnected);

public slots:
    void setId(int id);
    void setDeviceName(QString deviceName);



private slots:
    void skipForHeader();
    void dataReady();

protected:
    QIODevice *dataPort() const;
    void setDataPort(QIODevice *dataPort);

    void setIsConnected(bool isConnected);
private:
    int m_id = 0;
    QString m_deviceName;
    bool m_isConnented;
    bool m_isConnected;

public:
    int id() const;
    QString deviceName() const;

    bool isConnected() const;

};

#endif // ADAPTERBASE_H
