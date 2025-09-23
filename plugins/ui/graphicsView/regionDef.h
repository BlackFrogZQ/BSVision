#pragma once
#include <QString>
#include <QColor>
#include <QPointF>
namespace TIGER_Graphics
{
    struct CGraphicsRegion
    {
        QString name;
        QColor color;
        QList<QPointF> points;
        CGraphicsRegion(QString p_name = " ", QColor p_color = Qt::red, QList<QPointF> p_points = QList<QPointF>())
            : name(p_name), points(p_points), color(p_color) {}
        void clear() { points.clear(); }
    };
}