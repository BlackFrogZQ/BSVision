#pragma once
#include "system/myMath.h"

#include "bsBasic.h"

#include <QColor>
namespace TIGER_ProcessTool
{
    struct COutFeatureShape
    {
        QString name;
        QColor color;
        QList<QPointF> points;
        COutFeatureShape(QString p_name = " ", QColor p_color = Qt::red, QList<QPointF> p_points = QList<QPointF>())
            : name(p_name), points(p_points), color(p_color) {}
        void clear() { points.clear(); }
    };
    struct COutInfo
    {
        double score;
        double angle;
        //[col,row]
        CPoint2d point;
        COutInfo(double p_score = -1, double p_angle = -1, double p_col = 0, double p_row = 0)
            : score(p_score), angle(p_angle), point(p_col, p_row) {}
        friend QDebug operator<<(QDebug out, const COutInfo &info)
        {
            out << QString("(%1,%2,%3,%4)").arg(dToStr(info.point.x, 1)).arg(dToStr(info.point.y, 1)).arg(dToStr(info.angle, 2)).arg(dToStr(info.score, 2));
            return out;
        }
    };
};