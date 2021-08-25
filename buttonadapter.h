#ifndef BUTTONADAPTER_H
#define BUTTONADAPTER_H

#include <QObject>
#include <QTcpSocket>

#include "adapterbase.h"




class ButtonAdapter : public AdapterBase
{
    Q_OBJECT


    QTcpSocket m_tcpSocket;
    QString m_address;
    int m_errorTransmitCounter = 0;


public:
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(ButtonState status READ status WRITE setStatus NOTIFY statusChanged)

    enum ButtonState { BUTTON_OFF, BUTTON_ON };

    explicit ButtonAdapter(QObject *parent = nullptr);
    void open(QString portName) override;
    void close() override;
    QString address() const;
    ButtonState status() const;

    Q_INVOKABLE QJSValue getStatus();


    static void declareQML() {
        qmlRegisterType<ButtonAdapter>("ESPDevice",1, 0,"ButtonAdapter");
    }

signals:
    void addressChanged(QString address);
    void statusChanged(ButtonState status);

public slots:
    void setAddress(QString address);
    void setStatus(ButtonState status);

protected:
    void customProcessMessage(QByteArray message) override;

public:
    Q_ENUM(ButtonState)

private:
    ButtonState m_status;
};

#endif // BUTTONADAPTER_H
