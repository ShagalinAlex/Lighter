#ifndef MYCONICALGRADIENT_H
#define MYCONICALGRADIENT_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QImage>


class MyConicalGradient : public QQuickPaintedItem
{
    Q_OBJECT

    QImage m_image;
public:
    MyConicalGradient();

    Q_INVOKABLE QColor getColor(int x, int y);

    static void declareQML() {
        qmlRegisterType<MyConicalGradient>("ESPDevice",1, 0,"MyConicalGradient");
    }


signals:

public slots:

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);
};

#endif // MYCONICALGRADIENT_H
