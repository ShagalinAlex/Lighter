#ifndef ESPDEVICEINFORMATION_H
#define ESPDEVICEINFORMATION_H

#include <QObject>
#include <QQmlEngine>
#include <QHostAddress>


class ESPDeviceInformation : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

    QString m_address;
    int m_id = -1;
    QString m_name;
    int m_type = -1;

public:
    ESPDeviceInformation(QObject *parent = nullptr);

    QString address() const;
    int id() const;
    QString name() const;
    int type() const;

public slots:
    void setAddress(QString address);
    void setId(int id);
    void setName(QString name);
    void setType(int type);

signals:
    void addressChanged(QString address);
    void idChanged(int id);
    void nameChanged(QString name);
    void typeChanged(int type);
};

#endif // ESPDEVICEINFORMATION_H
