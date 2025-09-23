#include "rightAngleTool.h"
#include "rightAngleItem.h"
#include <QVector2D>
namespace TIGER_Graphics
{
    CRightAngleTool::CRightAngleTool(QGraphicsItem *parent) : IPositioningToolItem(parent)
    {
        m_pRightAngleItem = new CRightAngleItem;
        m_pRightAngleItem->addKeyPoints({{0, -100}, {0, 0}, {100, 0}});
        this->add(m_pRightAngleItem);
    }

    QPointF CRightAngleTool::getPosition()
    {
        assert(m_child.size() == 1);
        return m_pRightAngleItem->mapToScene(m_pRightAngleItem->getKeyPoints()[CRightAngleItem::ckptRightAngle]);
    }

    void CRightAngleTool::setShapeSize(QPointF center, double w, double h)
    {
        auto points = m_pRightAngleItem->getSceneKeyPoints();
        w *= (points[CRightAngleItem::ckptHorizontal].x() - points[CRightAngleItem::ckptRightAngle].x()) > 0 ? 1 : -1;
        h *= (points[CRightAngleItem::ckptVertical].y() - points[CRightAngleItem::ckptRightAngle].y()) > 0 ? 1 : -1;
        auto rightAngle = QPointF(center.x() - 0.5 * w, center.y() - 0.5 * h);
        m_pRightAngleItem->setKeyPoint(CRightAngleItem::ckptRightAngle, rightAngle);
        m_pRightAngleItem->setKeyPoint(CRightAngleItem::ckptVertical, QPointF(rightAngle.x(), rightAngle.y() + h));
        m_pRightAngleItem->setKeyPoint(CRightAngleItem::ckptHorizontal, QPointF(rightAngle.x() + w, rightAngle.y()));
    }

    QList<QPainterPath> CRightAngleTool::getToolPaths() const
    {
        auto points = m_pRightAngleItem->getSceneKeyPoints();
        QList<QPainterPath> paths;
        QPainterPath path;
        path.moveTo(points[CRightAngleItem::ckptVertical]);
        path.lineTo(points[CRightAngleItem::ckptRightAngle]);
        paths.append(path);
        path = QPainterPath();
        path.moveTo(points[CRightAngleItem::ckptRightAngle]);
        path.lineTo(points[CRightAngleItem::ckptHorizontal]);
        paths.append(path);
        return paths;
    }

    QPainterPath CRightAngleTool::drawShape() const
    {
        auto points = m_pRightAngleItem->getSceneKeyPoints();
        QPointF v1 = points[CRightAngleItem::ckptRightAngle];
        QVector2D xExpend = QVector2D(v1 - points[CRightAngleItem::ckptHorizontal]);
        double minLen = xExpend.length();
        xExpend.normalize();
        QVector2D yExpend = QVector2D(v1 - points[CRightAngleItem::ckptVertical]);
        if (minLen > yExpend.length())
        {
            minLen = yExpend.length();
        }
        yExpend.normalize();
        assert(minLen > 0);
        minLen = qMin(5.0, minLen * 0.1);
        minLen = qMax(1.0, minLen);
        QPainterPath path;
        path.moveTo(v1);
        path.lineTo(v1 + xExpend.toPointF() * minLen);
        path.moveTo(v1);
        path.lineTo(v1 + yExpend.toPointF() * minLen);
        return path;
    }
};