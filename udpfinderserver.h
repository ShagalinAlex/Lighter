#ifndef UDPFINDERSERVER_H
#define UDPFINDERSERVER_H

#include <QObject>
#include <QUdpSocket>

class UdpFinderServer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)


    QUdpSocket m_udpSocket;

    QString m_name;

public:
    explicit UdpFinderServer(QObject *parent = nullptr);
    void startServer(quint16 port);

    QString name() const;

signals:

    void nameChanged(QString name);

public slots:

void setName(QString name);

private slots:
    void dataRecieved();
};

#endif // UDPFINDERSERVER_H
