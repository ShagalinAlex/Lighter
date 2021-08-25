#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#include <QObject>
#include <QQmlEngine>

class DeviceSettings
{
    Q_GADGET

    Q_PROPERTY(QString apName READ apName WRITE setApName)
    Q_PROPERTY(QString apPass READ apPass WRITE setApPass)
    Q_PROPERTY(quint8 devId READ devId WRITE setDevId)
    Q_PROPERTY(QString devName READ devName WRITE setDevName)
    Q_PROPERTY(ApType apType READ apType WRITE setApType)
    Q_PROPERTY(bool isValid READ isValid)

    QString m_apName;
    QString m_apPass;
    quint8 m_devId;
    QString m_devName;


public:
    enum ApType {
        STATION = 1,
        SOFTAP
    };
    Q_ENUM(ApType)

    DeviceSettings(bool valid = false);
    QString apName() const;
    QString apPass() const;
    quint8 devId() const;
    QString devName() const;
    ApType apType() const;

    bool isValid() const;

public slots:
    void setApName(QString apName);
    void setApPass(QString apPass);
    void setDevId(quint8 devId);
    void setDevName(QString devName);
    void setApType(ApType apType);

private:
    ApType m_apType;
    bool m_isValid;
};

Q_DECLARE_METATYPE(DeviceSettings)

#endif // DEVICESETTINGS_H
