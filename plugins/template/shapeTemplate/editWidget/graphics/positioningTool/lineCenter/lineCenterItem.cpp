#include "lineCenterItem.h"
namespace TIGER_Graphics
{
    bool CLineCenterItem::isDrawFinished() const
    {
        return m_keyPoints.size() == ckptMax;
    }

    QPainterPath CLineCenterItem::drawShape() const
    {
        QPainterPath path;
        if (m_keyPoints.size() >= ckptMax)
        {
            path.moveTo(m_keyPoints[ckptVertical]);
            path.lineTo(m_keyPoints[ckptRightAngle]);
            path.lineTo(m_keyPoints[ckptRightAngle2]);
            path.lineTo(m_keyPoints[ckptVertical2]);
        }
        return path;
    }
};