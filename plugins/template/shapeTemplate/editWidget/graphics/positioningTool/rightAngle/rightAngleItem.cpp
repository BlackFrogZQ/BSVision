#include "rightAngleItem.h"
namespace TIGER_Graphics
{
    bool CRightAngleItem::isDrawFinished() const
    {
        return m_keyPoints.size() == ckptMax;
    }
    QPainterPath CRightAngleItem::drawShape() const
    {
        QPainterPath path;
        if (m_keyPoints.size() >= ckptMax)
        {
            path.moveTo(m_keyPoints[ckptVertical]);
            path.lineTo(m_keyPoints[ckptRightAngle]);
            path.lineTo(m_keyPoints[ckptHorizontal]);
        }
        return path;
    }

    // QPainterPath CRightAngleItem::nextPointShape(const QPointF &p_point) const
    // {
    //     assert(m_keyPoints.size() <= ckptMax);
    //     QPainterPath path;
    //     if (m_keyPoints.size() > 0)
    //     {
    //         path.moveTo(m_keyPoints[ckptVertical]);
    //         for (size_t i = 1; i < m_keyPoints.size(); i++)
    //         {
    //             path.lineTo(m_keyPoints[i]);
    //         }
    //         path.lineTo(p_point);
    //     }
    //     return path;
    // }
}