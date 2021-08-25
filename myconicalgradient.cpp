#include <QPainter>
#include <QDebug>
#include <QGradientStop>
#include "myconicalgradient.h"

MyConicalGradient::MyConicalGradient()
{

}

QColor MyConicalGradient::getColor(int x, int y)
{


    return QColor(m_image.pixel(qMin(x, m_image.width() - 1), qMin(y, m_image.height() - 1)));
}

void MyConicalGradient::paint(QPainter *painter)
{
    m_image = QImage( static_cast<int>(width()), static_cast<int>(height()), QImage::Format_ARGB32);
    QPainter imagePainter(&m_image);

    QConicalGradient gradient( QPointF(m_image.width() / 2, m_image.height() / 2), 360 );

    gradient.setColorAt(0.000, QColor("#ff0000"));
    gradient.setColorAt(0.167, QColor("#ffff00"));
    gradient.setColorAt(0.333, QColor("#00ff00"));
    gradient.setColorAt(0.500, QColor("#00ffff"));
    gradient.setColorAt(0.667, QColor("#0000ff"));
    gradient.setColorAt(0.833, QColor("#ff00ff"));
    gradient.setColorAt(1.000, QColor("#ff0000"));

    QPen pen = imagePainter.pen();
    pen.setWidth(0);
    pen.setColor(QColor("transparent"));
    imagePainter.setPen(pen);
    QBrush brush(gradient);
    brush.setStyle(Qt::SolidPattern);
    imagePainter.setBrush(gradient);

    imagePainter.drawRect(QRect(0,0, static_cast<int>(width()), static_cast<int>(height())));

    painter->drawImage(QRect(0,0, static_cast<int>(width()), static_cast<int>(height())), m_image);
}
