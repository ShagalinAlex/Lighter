#ifndef MQTTSETTINGS_H
#define MQTTSETTINGS_H

#include <QObject>

class MQTTSettings
{
    Q_GADGET

    Q_PROPERTY(QString uri READ uri)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString passwd READ passwd)
    Q_PROPERTY(QString topic READ topic)

    QString m_uri;

    QString m_name;

    QString m_passwd;
    bool m_isValid;
    QString m_topic;

public:
    explicit MQTTSettings(bool valid = false);

    QString uri() const;
    QString name() const;
    QString passwd() const;

    bool isValid() const;

    QString topic() const;

signals:

public slots:
    void setUri(QString uri);
    void setName(QString name);
    void setPasswd(QString passwd);
    void setTopic(QString topic);
};

Q_DECLARE_METATYPE(MQTTSettings)

#endif // MQTTSETTINGS_H
