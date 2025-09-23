#include "fitDef.h"
#include <QTransform>
namespace TIGER_Graphics
{
    CFitRect::CFitRect(double p_cx,
                       double p_cy,
                       double p_w,
                       double p_h,
                       double p_angle)
        : cx(p_cx), cy(p_cy), w(p_w), h(p_h), angle(p_angle)
    {
    }

    QPolygonF CFitRect::toPolygon() const
    {
        QRectF rect(-w * 0.5, -h * 0.5, w, h);
        return QTransform().rotate(angle).map(rect).translated(cx, cy);
    }

    CFitRect &CFitRect::moveCenter(double p_x, double p_y)
    {
        cx = p_x;
        cy = p_y;
        return *this;
    };
    QVector<CFitRect> IBasicFitItem::fitRects() const
    {
        return m_fitRects;
    }

    QRectF IBasicFitItem::boundingRect() const
    {
        return m_aabb;
    }

}