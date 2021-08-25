#ifndef RGBADAPTER_H
#define RGBADAPTER_H

#include <QObject>
#include <QTcpSocket>

#include "adapterbase.h"

class RGBAdapter : public AdapterBase
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)

    QTcpSocket m_tcpSocket;
    QString m_address;
    qreal m_brightness = 1.0;
    QColor m_color;

public:
    explicit RGBAdapter(QObject *parent = nullptr);

    void open(QString portName) override;
    void close() override;

    static void declareQML() {
        qmlRegisterType<RGBAdapter>("ESPDevice",1, 0,"RBGControl");
    }

    qreal brightness() const;
    QColor color() const;
    QString address() const;

signals:

    void addressChanged(QString address);
    void colorChanged(QColor color);
    void brightnessChanged(qreal brightness);


public slots:
    void setAddress(QString address);
    void setBrightness(qreal brightness);
    void setColor(QColor color);



    // AdapterBase interface
protected:
    void customProcessMessage(QByteArray message) override;
};

#endif // RGBADAPTER_H
