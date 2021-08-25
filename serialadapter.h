#ifndef SERIALADAPTER_H
#define SERIALADAPTER_H

#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QTimer>
#include <QColor>
#include <QQueue>

#include "adapterbase.h"

class SerialAdapter : public AdapterBase
{
    Q_OBJECT
    Q_PROPERTY(QStringList serials READ serials WRITE setSerials NOTIFY serialsChanged)

    QStringList m_serials;
    QTimer m_serialFindTimer;

public:
    explicit SerialAdapter(QObject *parent = nullptr);

    static void declareQML() {
        qmlRegisterType<SerialAdapter>("ESPDevice",1, 0,"SerialAdapter");
    }


    void open(QString portName) override;
    void close() override;

    QStringList serials() const;


signals:
    void serialsChanged(QStringList serials);

public slots:
    void setSerials(QStringList serials);


private slots:
    void serialFindTimerTimeout();
    void portErrorHandled();


    // AdapterBase interface
protected:
    void customProcessMessage(QByteArray message) override;
};

#endif // SERIALADAPTER_H
