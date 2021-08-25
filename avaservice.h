#ifndef AVASERVICE_H
#define AVASERVICE_H

#include <QObject>
#include <QMap>

#include "udpfinderclient.h"
#include "espdeviceinformation.h"

class AvaService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> abonents READ abonents NOTIFY abonentsChanged)


    QList<UdpFinderClient*> m_finderClients;

    QMap<QString, ESPDeviceInformation*> m_abonents;

public:
    explicit AvaService(QObject *parent = nullptr);

    QList<QObject *> abonents() const;
    Q_INVOKABLE void resetAbonents();

    static void declareQML() {
        qmlRegisterType<AvaService>("Lighter",1, 0,"AvaService");
    }



signals:
    void abonentsChanged(QList<QObject *> abonents);

private slots:
    void serverFinded(ESPDeviceInformation *device);
};

#endif // AVASERVICE_H
