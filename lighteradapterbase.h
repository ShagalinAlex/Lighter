#ifndef LIGHTERADAPTERBASE_H
#define LIGHTERADAPTERBASE_H

#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QTimer>
#include <QColor>
#include <QQueue>

#include "message.h"

/// Набор глобальных перечислителей
class LighterAdapter {
public:
    enum ApType {
        APNULL,
        STATION,
        SOFTAP
    };
    Q_ENUM(ApType)
    enum Messages {
        MessageColor_Request,
        MessageFlashDataParam_Request,
        MessageRestart_Request,

        MessageResult_Request,
        Timeout_Request,
        None_Request
    };
    Q_ENUM(Messages)

    static void declareQML() {
        qmlRegisterUncreatableMetaObject(LighterAdapter::staticMetaObject , "Lighter",1, 0,"LighterAdapter", "LighterAdapter reason");
    }

    Q_GADGET
    LighterAdapter() = delete;
};

class LighterAdapterBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool serviceMode READ serviceMode WRITE setServiceMode NOTIFY serviceModeChanged)

    QIODevice *m_dataPort = nullptr;


    QQueue<char> m_ciclicData;
    void processMessage(QByteArray message);


protected:
    QByteArray packData(QByteArray data);


public:
    explicit LighterAdapterBase(QObject *parent = nullptr);

    Q_INVOKABLE virtual void open(QString portName) = 0;
    Q_INVOKABLE virtual void close() = 0;
    Q_INVOKABLE void requestRestart();

    bool opened() const;

signals:

    void openedChanged(bool opened);
    void serviceModeChanged(bool serviceMode);

public slots:
    void setServiceMode(bool serviceMode);
    void setDeviceName(QString deviceName);

private slots:
    void requestTimeout();
    void skipForHeader();
    void dataReady();

public:
    int id() const;
    bool serviceMode() const;

protected:
    QIODevice *dataPort() const;
    void setDataPort(QIODevice *dataPort);

private:
    int m_id = 0;
    bool m_serviceMode = false;
};

#endif // LIGHTERADAPTERBASE_H
